#include <chucho/clock_util.hpp>
#include <chrono>

namespace chucho
{

namespace clock_util
{

const bool system_clock_supports_milliseconds =
    (static_cast<double>(std::chrono::system_clock::period::num) / static_cast<double>(std::chrono::system_clock::period::den))
    <=
    (1.0 / 1000.0);

}

}
