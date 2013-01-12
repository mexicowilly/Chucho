#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__

#include <chucho/filter.hpp>
#include <chucho/level.hpp>

namespace chucho
{

class CHUCHO_EXPORT level_threshold_filter : public filter
{
public:
    level_threshold_filter(std::shared_ptr<level> lvl);

    virtual result evaluate(const event& evt) override;
    std::shared_ptr<level> get_level() const;

private:
    std::shared_ptr<level> level_;
};

inline std::shared_ptr<level> level_threshold_filter::get_level() const
{
    return level_;
}

}

#endif
