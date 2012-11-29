#if !defined(CHUCHO_ROLLING_FILE_WRITER_HPP__)
#define CHUCHO_ROLLING_FILE_WRITER_HPP__

#include <chucho/file_writer.hpp>
#include <chucho/file_roller.hpp>
#include <chucho/file_roll_trigger.hpp>

namespace chucho
{

class rolling_file_writer : public file_writer
{
public:
    rolling_file_writer(on_start start,
                        bool flush,
                        std::shared_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::shared_ptr<file_roller> roller_;
    std::shared_ptr<file_roll_trigger> trigger_;
};

}

#endif
