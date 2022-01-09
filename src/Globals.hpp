#pragma once

#include <cmath>

#define ND [[nodiscard]]

template<typename T>
T divRoundUp(std::integral auto a, std::integral auto b) {
    static_assert(std::is_integral_v<T>);
    int result = a / b;
    if (a % b == 0)
        return result;
    return result + 1;
}
