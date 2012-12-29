#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__

#include <chucho/filter.hpp>
#include <chucho/level.hpp>

namespace chucho
{

class level_threshold_filter_memento;

class CHUCHO_EXPORT level_threshold_filter : public filter
{
public:
    level_threshold_filter(std::shared_ptr<level> lvl);
    level_threshold_filter(const level_threshold_filter_memento& mnto);

    virtual result evaluate(const event& evt) override;

private:
    std::shared_ptr<level> level_;
};

}

#endif
