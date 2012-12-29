#if !defined(CHUCHO_WRITER_HPP__)
#define CHUCHO_WRITER_HPP__

#include <chucho/filter.hpp>
#include <chucho/formatter.hpp>
#include <chucho/configurable.hpp>
#include <vector>
#include <mutex>

namespace chucho
{

class CHUCHO_EXPORT writer : public status_reporter,
                             public configurable
{
public:
    writer(std::shared_ptr<formatter> fmt);
    writer(const writer&) = delete;

    writer& operator= (const writer&) = delete;

    void add_filter(std::shared_ptr<filter> flt);
    void clear_filters();
    void write(const event& evt);

protected:
    virtual void write_impl(const event& evt) = 0;

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
