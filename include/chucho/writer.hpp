#if !defined(CHUCHO_WRITER_HPP__)
#define CHUCHO_WRITER_HPP__

#include <chucho/filter.hpp>
#include <chucho/formatter.hpp>
#include <chucho/configurable.hpp>
#include <vector>
#include <mutex>

namespace chucho
{

/**
 * @defgroup writers 
 * Writers write log events to destinations. 
 */

/**
 * @class writer writer.hpp chucho/writer.hpp
 * A writer writes events to whatever destination it is designed 
 * to use. A @ref logger can have as many writers attached to it
 * as it needs. Writers also provide a further means of 
 * filtering events, as each writer has a collection of 
 * @ref filter "filters". The filters are visited in the order 
 * in which they are added.
 *  
 * @ingroup writers 
 */
class CHUCHO_EXPORT writer : public status_reporter,
                             public configurable
{
public:
    /**
     * @name Constructors
     */
    //@{
    /**
     * Construct a writer with the given formatter. All events will
     * be formatted using the formatter before being written. 
     *
     * @param fmt the formatter
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     */
    writer(std::shared_ptr<formatter> fmt);
    /**
     * Unimplemented copy constructor.
     */
    writer(const writer&) = delete;
    //@}

    /**
     * @name Operator
     */
    //@{
    /**
     * Unimplemented assignment operator.
     */
    writer& operator= (const writer&) = delete;
    //@}

    /**
     * Add a filter. Filters are visited in the order in which they 
     * are added. 
     * 
     * @param flt the filter
     */
    void add_filter(std::shared_ptr<filter> flt);
    /**
     * Remove all filters.
     */
    void clear_filters();
    /**
     * Receive the collection of all filters.
     * @return the filters
     */
    std::vector<std::shared_ptr<filter>> get_filters();
    /**
     * Write an event. This non-virtual method takes care of all the 
     * common housekeeping that writers must undertake when writing 
     * an event. A check is made to see whether the current thread 
     * is already writing something, the presence of the formatter 
     * is verified, and the filters are evaluated. If the event can 
     * actually be written, then the virtual method @ref write_impl 
     * is called. 
     * 
     * @param evt the event to write
     */
    void write(const event& evt);

protected:
    /**
     * Write the event. This virtual method is meant to handle the 
     * mechanics of actually writing to the destination. The calling 
     * @ref write method has already taken care of the housekeeping.
     * 
     * @param evt the event to write
     */
    virtual void write_impl(const event& evt) = 0;

    /**
     * The formatter used to turn events into text.
     */
    std::shared_ptr<formatter> formatter_;

private:
    /**
     * @pre guard_ must be locked
     * @param evt the event to evaluate
     * @return true if this writer can write the event
     */
    CHUCHO_NO_EXPORT bool permits(const event& evt);

    std::vector<std::shared_ptr<filter>> filters_;
    std::mutex guard_;
};

}

#endif
