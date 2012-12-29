#include <chucho/level_threshold_filter.hpp>
#include <chucho/logger.hpp>
#include <chucho/exception.hpp>
#include <chucho/level_threshold_filter_memento.hpp>

namespace chucho
{

level_threshold_filter::level_threshold_filter(std::shared_ptr<level> lvl)
    : level_(lvl)
{
    set_status_origin("level_threshold_filter");
    if (!level_)
        throw exception("The level is invalid");
}

level_threshold_filter::level_threshold_filter(const level_threshold_filter_memento& mnto)
    : level_threshold_filter(mnto.get_level())
{
}

filter::result level_threshold_filter::evaluate(const event& evt)
{
    return (!level_ || *evt.get_level() >= *level_) ?
        result::NEUTRAL : result:: DENY;
}

}
