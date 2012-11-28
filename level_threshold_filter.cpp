#include <chucho/level_threshold_filter.hpp>

namespace chucho
{

filter::result level_threshold_filter::permits(const event& evt)
{
    return (!level_ || evt.get_logger()->get_level() >= level_) ?
        result::NEUTRAL : result:: DENY;
}

}
