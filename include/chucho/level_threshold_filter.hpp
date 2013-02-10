#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__

#include <chucho/filter.hpp>
#include <chucho/level.hpp>

namespace chucho
{

/**
 * @class level_threshold_filter level_threshold_filter.hpp chucho/level_threshold_filter.hpp 
 * A filter that tests the log @ref level. The exact same check 
 * on the @ref level that is performed by a @ref logger when it 
 * is determining whether an event is filtered or not is 
 * performed by this filter. This allows further refinement. A 
 * @ref logger can have one @ref writer filtering at one @ref 
 * level and another @ref writer filtering at another @ref 
 * level.
 * 
 * @ingroup filters
 */
class CHUCHO_EXPORT level_threshold_filter : public filter
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a level_threshold_filter.
     * 
     * @param lvl the log level at which to filter
     * @throw std::invalid_argument if the level is an uninitialized 
     *        std::shared_ptr
     */
    level_threshold_filter(std::shared_ptr<level> lvl);
    //@}

    /**
     * Evaluate the event. If the log level of the event is greater 
     * than the log level of this filter, then return 
     * @ref result "result::NEUTRAL". Otherwise, return 
     * @ref result "result::DENY".
     * 
     * @param evt the event to evaluate
     * @return @ref result "result::NEUTRAL" if the log level of the
     *         event is greater than or equal to the level of this
     *         filter. Otherwise, return @ref result "result::DENY".
     */
    virtual result evaluate(const event& evt) override;
    /**
     * Return the level being used by this filter.
     * 
     * @return the level
     */
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
