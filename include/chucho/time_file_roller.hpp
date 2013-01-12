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
    time_file_roller(const std::string file_name_pattern,
                     std::size_t max_history);

    virtual std::string get_active_file_name() override;
    const std::string& get_file_name_pattern() const;
    std::size_t get_max_history() const;
    virtual bool is_triggered(const std::string& active_file, const event& e) override;
    virtual void roll() override;

private:
    typedef std::chrono::system_clock clock_type;
    typedef std::chrono::time_point<clock_type> time_type;

    enum class period : std::size_t
    {
        UNKNOWN,
        MINUTE = 60,
        HOUR = MINUTE * 60,
        DAY = HOUR * 24,
        WEEK = DAY * 7,
        MONTH,
        YEAR
    };

    class CHUCHO_NO_EXPORT cleaner : public status_reporter
    {
    public:
        cleaner(time_file_roller& roller);

        void clean(const time_type& now);

    private:
        void clean_one(const time_type& t, int period_offset);
        std::size_t periods_elapsed(const time_type& first, const time_type& last);
        std::size_t periods_since_last(const time_type& now);
        time_type relative(const time_type& t, int period_offset);

        time_file_roller& roller_;
        int oldest_period_offset_;
        std::unique_ptr<time_type> last_clean_;
    };

    CHUCHO_NO_EXPORT void compute_next_roll(const time_type& now);
    CHUCHO_NO_EXPORT std::string find_time_spec(const std::string& str,
                                                std::size_t& start,
                                                std::size_t& end);
    CHUCHO_NO_EXPORT std::string format(const struct std::tm& cal, const std::string& spec);
    CHUCHO_NO_EXPORT std::string resolve_file_name(const time_type& tm);
    CHUCHO_NO_EXPORT void set_period();

    time_type next_roll_;
    std::size_t max_history_;
    std::string file_name_pattern_;
    period period_;
    std::unique_ptr<cleaner> cleaner_;
};

inline const std::string& time_file_roller::get_file_name_pattern() const
{
    return file_name_pattern_;
}

inline std::size_t time_file_roller::get_max_history() const
{
    return max_history_;
}

}

#endif
