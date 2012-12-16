#include <chucho/level_threshold_filter.hpp>
#include <chucho/logger.hpp>

namespace chucho
{

level_threshold_filter::level_threshold_filter(std::shared_ptr<level> lvl)
    : level_(lvl)
{
    set_status_origin("level_threshold_filter");
}

filter::result level_threshold_filter::evaluate(const event& evt)
{
    return (!level_ || *evt.get_level() >= *level_) ?
        result::NEUTRAL : result:: DENY;
}

}
