#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__

#include <chucho/filter.hpp>
#include <chucho/level.hpp>

namespace chucho
{

class CHUCHO_EXPORT level_threshold_filter : public filter
{
public:
    virtual result permits(const event& evt) override;

    void set_level(std::shared_ptr<level> lvl);

private:
    std::shared_ptr<level> level_;
};

inline void level_threshold_filter::set_level(std::shared_ptr<level> lvl)
{
    level_ = lvl;
}

}

#endif
