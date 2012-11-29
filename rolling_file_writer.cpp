#include <chucho/rolling_file_writer.hpp>

namespace chucho
{

rolling_file_writer::rolling_file_writer(on_start start,
                                         std::shared_ptr<file_roller> roller,
                                         std::shared_ptr<file_roll_trigger> trigger)
    : file_writer(start),
      roller_(roller),
      trigger_(trigger)
{
    if (!trigger_)
        trigger_ = std::dynamic_pointer_cast<file_roll_trigger>(roller);
    if (!trigger_)
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
