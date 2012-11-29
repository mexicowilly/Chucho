#include <chucho/level_threshold_filter.hpp>
#include <chucho/logger.hpp>

namespace chucho
{

filter::result level_threshold_filter::evaluate(const event& evt)
{
    return (!level_ || evt.get_logger()->get_level() >= level_) ?
        result::NEUTRAL : result:: DENY;
}

}
