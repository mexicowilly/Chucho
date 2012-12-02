#include <chucho/file_roll_trigger.hpp>

namespace chucho
{

class CHUCHO_EXPORT size_file_roll_trigger : public file_roll_trigger
{
public:
    size_file_roll_trigger(unsigned long long max_size);

    virtual bool is_triggered(const std::string& active_file, const event& e) override;

private:
    unsigned long long max_size_;
};

}
