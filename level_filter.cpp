#include <chucho/level_filter.hpp>

namespace chucho
{

level_filter::level_filter(std::shared_ptr<level>lvl,
                           result on_match,
                           result on_mismatch)
    : level_(lvl),
      on_match_(on_match),
      on_mismatch_(on_mismatch)
{
}

filter::result level_filter::evaluate(const event& evt)
{
    return (*evt.get_level() == *level_) ? on_match_ : on_mismatch_;
}

}
