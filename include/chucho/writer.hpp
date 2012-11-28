#if !defined(CHUCHO_WRITER_HPP__)
#define CHUCHO_WRITER_HPP__

#include <chucho/filter.hpp>
#include <vector>

namespace chucho
{

class CHUCHO_EXPORT writer
{
public:
    writer();
    virtual ~writer();

    void add_filter(std::shared_ptr<filter> flt);
    void clear_filters();
    virtual void write(const event& evt) = 0;

private:
    std::vector<std::shared_ptr<filter>> filters_;
};

inline void writer::add_filter(std::shared_ptr<filter> flt)
{
    filters_.push_back(flt);
}

inline void writer::clear_filters()
{
    filters_.clear();
}

}

#endif
