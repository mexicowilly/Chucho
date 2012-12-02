#if !defined(CHUCHO_TIME_FILE_ROLLER_HPP__)
#define CHUCHO_TIME_FILE_ROLLER_HPP__

#include <chucho/file_roller.hpp>
#include <chucho/file_roll_trigger.hpp>
#include <chrono>

namespace chucho
{

class CHUCHO_EXPORT time_file_roller : public file_roller,
                                       public file_roll_trigger
{
public:
    virtual std::string get_active_file_name() override;
    virtual bool is_triggered(const std::string& active_file, const event& e) override;
    virtual void roll() override;

private:
    typedef std::chrono::system_clock clock_type;
    typedef std::chrono::time_point<clock_type> time_type;

    enum class period
    {
        UNKNOWN,
        HOUR,
        DAY,
        MONTH,
        YEAR
    };

    CHUCHO_NO_EXPORT void compute_next_roll(const time_type& now);
    CHUCHO_NO_EXPORT std::string resolve_file_name(const time_type& tm);
    CHUCHO_NO_EXPORT void set_period();

    time_type next_roll_;
    unsigned max_history_;
    std::string file_name_pattern_;
    period period_;
};

}

#endif
