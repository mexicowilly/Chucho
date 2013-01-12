#include <chucho/file_roll_trigger.hpp>

namespace chucho
{

class CHUCHO_EXPORT size_file_roll_trigger : public file_roll_trigger
{
public:
    size_file_roll_trigger(unsigned long long max_size);

    unsigned long long get_max_size() const;
    virtual bool is_triggered(const std::string& active_file, const event& e) override;

private:
    unsigned long long max_size_;
};

inline unsigned long long size_file_roll_trigger::get_max_size() const
{
    return max_size_;
}

}
