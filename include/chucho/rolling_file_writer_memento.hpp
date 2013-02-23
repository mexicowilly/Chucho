#if !defined(CHUCHO_ROLLING_FILE_WRITER_MEMENTO_HPP__)
#define CHUCHO_ROLLING_FILE_WRITER_MEMENTO_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/file_writer_memento.hpp>
#include <chucho/file_roller.hpp>
#include <chucho/file_roll_trigger.hpp>

namespace chucho
{

class rolling_file_writer_memento : public file_writer_memento
{
public:
    rolling_file_writer_memento(const configurator& cfg);

    std::shared_ptr<file_roller> get_file_roller() const;
    std::shared_ptr<file_roll_trigger> get_file_roll_trigger() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    std::shared_ptr<file_roller> roller_;
    std::shared_ptr<file_roll_trigger> trigger_;
};

inline std::shared_ptr<file_roller> rolling_file_writer_memento::get_file_roller() const
{
    return roller_;
}

inline std::shared_ptr<file_roll_trigger> rolling_file_writer_memento::get_file_roll_trigger() const
{
    return trigger_;
}

}

#endif
