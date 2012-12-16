#include <chucho/rolling_file_writer.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

rolling_file_writer::rolling_file_writer(std::shared_ptr<formatter> fmt,
                                         std::unique_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : rolling_file_writer(fmt,
                          file_writer::on_start::APPEND,
                          true,
                          std::move(roller),
                          trigger)
{
}

rolling_file_writer::rolling_file_writer(std::shared_ptr<formatter> fmt,
                                         const std::string& file_name,
                                         std::unique_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : rolling_file_writer(fmt,
                          file_name,
                          file_writer::on_start::APPEND,
                          true,
                          std::move(roller),
                          trigger)
{
}

rolling_file_writer::rolling_file_writer(std::shared_ptr<formatter> fmt,
                                         const std::string& file_name,
                                         on_start start,
                                         bool flush,
                                         std::unique_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : file_writer(fmt, file_name, start, flush),
      roller_(std::move(roller)),
      shared_trigger_(trigger)
{
    init();
}

rolling_file_writer::rolling_file_writer(std::shared_ptr<formatter> fmt,
                                         on_start start,
                                         bool flush,
                                         std::unique_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : file_writer(fmt, start, flush),
      roller_(std::move(roller)),
      shared_trigger_(trigger)
{
    init();
    open(roller_->get_active_file_name());
}

void rolling_file_writer::init()
{
    set_status_origin("rolling_file_writer");
    trigger_ = shared_trigger_ ?
        shared_trigger_.get() : dynamic_cast<file_roll_trigger*>(roller_.get());
    if (trigger_ == nullptr)
        throw file_exception("The rolling_file_writer has no file_roll_trigger");
    roller_->set_file_writer(this);
}

void rolling_file_writer::write_impl(const event& evt)
{
    if (trigger_->is_triggered(get_file_name(), evt))
    {
        close();
        roller_->roll();
        open(roller_->get_active_file_name());
    }
    file_writer::write_impl(evt);
}

}
