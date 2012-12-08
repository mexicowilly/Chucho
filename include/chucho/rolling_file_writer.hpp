#if !defined(CHUCHO_ROLLING_FILE_WRITER_HPP__)
#define CHUCHO_ROLLING_FILE_WRITER_HPP__

#include <chucho/file_writer.hpp>
#include <chucho/file_roller.hpp>
#include <chucho/file_roll_trigger.hpp>

namespace chucho
{

class CHUCHO_EXPORT rolling_file_writer : public file_writer
{
public:
    rolling_file_writer(std::unique_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());
    rolling_file_writer(const std::string& file_name,
                        std::unique_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());
    rolling_file_writer(const std::string& file_name,
                        on_start start,
                        bool flush,
                        std::unique_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());
    rolling_file_writer(on_start start,
                        bool flush,
                        std::unique_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());

protected:
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT void init();

    std::unique_ptr<file_roller> roller_;
    std::shared_ptr<file_roll_trigger> shared_trigger_;
    /**
     * You don't own this pointer. It is just a settable reference.
     * The trigger will either be what's in shared_trigger_ or 
     * what's in roller_. If shared_trigger_ has no pointer, then 
     * roller_ is checked to see if it is also a file_roll_trigger. 
     * If so, then it's pointer is used for trigger_. 
     */
    file_roll_trigger* trigger_;
};

}

#endif
