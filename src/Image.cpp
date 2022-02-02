#include "Image.hpp"

#include <chrono>
#include <list>
#include <mutex>
#include <utility>

struct ImageData {
    ImageData(fsys::path path_, sf::Texture const &texture_, Image::State state_) :
            path{std::move(path_)},
            texture{texture_},
            state{state_} {
        texture.setSmooth(true);
    }

    fsys::path path;
    sf::Texture texture;
    Image::State state = Image::State::loading;
};

std::optional<sf::Texture> tryLoad(fsys::path path) {
    auto sfImage = sf::Image{};
    if (not Image::isFormatSupported(path.extension().string()))
        return std::nullopt;
    if (not sfImage.loadFromFile(path.string()))
        return std::nullopt;
    if (sfImage.getSize().x > sf::Texture::getMaximumSize() or sfImage.getSize().y > sf::Texture::getMaximumSize())
        return std::nullopt;

    auto texture = sf::Texture{};
    texture.loadFromImage(sfImage);
    return texture;
}

class TextureLoader {
    using List = std::list<std::shared_ptr<ImageData>>;
public:
    ND std::shared_ptr<ImageData const> getTextureData(fsys::path path) {
        // was it already loaded?
        auto iterDone = std::ranges::find_if(texturesDone, [&](auto &i) { return i->path == path; });

        if (iterDone != texturesDone.end())
            return *iterDone; // yeah, we have it here

        // we don't know it, it's a new one

        // select list currently unused by the loading thread
        List *texturesWaiting;
        if (firstListLoading)
            texturesWaiting = &texturesWaiting2;
        else
            texturesWaiting = &texturesWaiting1;

        // put it on list to load
        return texturesWaiting->emplace_front(std::make_shared<ImageData>(path, sf::Texture{}, Image::State::loading));
    }

    void threadFunction() {
        while (doLoaderExist) {
            List *list;

            if (firstListLoading)
                list = &texturesWaiting2;
            else
                list = &texturesWaiting1;
            firstListLoading = !firstListLoading;

            if (list->empty())
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

            loading_function(*list);
        }
    }

    void loading_function(List &texturesWaiting) {
        if (texturesWaiting.empty())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        for (auto iter = texturesWaiting.begin(); iter != texturesWaiting.end(); ++iter) {
            // was it already loaded?
            auto iterWaiting = std::ranges::find_if(texturesDone, [&](auto &i) { return i->path == (**iter).path; });
            if (iterWaiting != texturesDone.end()) { // skip if already loaded
                auto &textureData = **iter;
                if (auto optTexture = tryLoad(textureData.path)) {
                    textureData.texture = *optTexture;
                    textureData.state = Image::State::ready;
                } else {
                    textureData.state = Image::State::invalid;
                }
                texturesDone.emplace_back(std::move(*iter));
            }
            texturesWaiting.erase(iter);
        }
    }

    TextureLoader() :
            loadingThread{[this] {
                doLoaderExist = true;
                return [this] { loading_function(texturesWaiting1); };
            }} {
    }

    ~TextureLoader() {
        doLoaderExist = false;
    }

private:
    List texturesWaiting1;
    List texturesWaiting2;
    std::list<std::shared_ptr<ImageData const>> texturesDone;
    std::atomic<bool> firstListLoading;
    std::thread loadingThread;
    static std::atomic<bool> doLoaderExist;
};

std::atomic<bool> TextureLoader::doLoaderExist;

auto &makeTextureLoader() {
    static TextureLoader textureLoader;
    return textureLoader;
}

Image Image::loadFromFile(fsys::path path) {
    return Image{makeTextureLoader().getTextureData(std::move(path))};
}

bool Image::isFormatSupported(std::string_view format) {
    return std::ranges::find(supportedFormats, format) != supportedFormats.end();
}

fsys::path Image::getPath() const {
    if (data == nullptr)
        return "";
    return data->path;
}

std::optional<std::reference_wrapper<sf::Texture const>> Image::getTexture() const {
    if (getState() != State::ready)
        return std::nullopt;
    if (data == nullptr)
        return std::nullopt;
    return std::ref(data->texture);
}

Image::State Image::getState() const {
    if (data == nullptr)
        return State::invalid;
    return data->state;
}

Image::Image(std::shared_ptr<ImageData const> data_) :
        data{std::move(data_)} {
}
