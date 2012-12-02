#include <chucho/writer.hpp>
#include <chucho/level_threshold_filter.hpp>

namespace chucho
{

void writer::add_filter(std::shared_ptr<filter> flt)
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    filters_.push_back(flt);
}

void writer::clear_filters()
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    filters_.clear();
}

bool writer::permits(const event& evt)
{
    for (std::shared_ptr<filter> f : filters_)
    {
        filter::result res = f->evaluate(evt);
        if (res == filter::result::DENY)
            return false;
        if (res == filter::result::ACCEPT)
            return true;
    }
    return true;
}

void writer::set_formatter(std::shared_ptr<formatter> fmt)
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    formatter_ = fmt;
}

void writer::write(const event& evt)
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    if (formatter_ && permits(evt))
        write_impl(evt);
}

}
