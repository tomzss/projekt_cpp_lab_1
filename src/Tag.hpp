#pragma once

#include <string_view>
#include <string>
#include "Nodiscard.hpp"

class Tag {
public:
    ND std::string_view getName() const {
        return name;
    }

private:
    std::string name;
};
