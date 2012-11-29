#if !defined(CHUCHO_LOGGER_HPP__)
#define CHUCHO_LOGGER_HPP__

#include <chucho/writer.hpp>
#include <vector>

namespace chucho
{

class CHUCHO_EXPORT logger : public std::enable_shared_from_this<logger>
{
public:
    static std::vector<std::shared_ptr<logger>> get_existing_loggers();
    static std::shared_ptr<logger> get_logger(const std::string& name);

    logger(const logger&) = delete;
    logger& operator= (const logger&) = delete;

    std::shared_ptr<level> get_effective_level() const;
    std::shared_ptr<level> get_level() const;
    const std::string& get_name() const;
    void set_level(std::shared_ptr<level> lvl);
    void set_writes_to_ancestors(bool val);
    void write(const event& evt);
    bool writes_to_ancestors() const;

private:
    static std::shared_ptr<logger> get_logger_impl(const std::string& name);
    static void static_init();

    logger(const std::string& name, std::shared_ptr<level> lvl = std::shared_ptr<level>());

    std::shared_ptr<logger> parent_;
    std::string name_;
    std::shared_ptr<level> level_;
    std::vector<std::shared_ptr<writer>> writers_;
    std::mutex writers_guard_;
    bool writes_to_ancestors_;
};

inline const std::string& logger::get_name() const
{
    return name_;
}

inline void logger::set_writes_to_ancestors(bool val)
{
    writes_to_ancestors_ = val;
}

inline bool logger::writes_to_ancestors() const
{
    return writes_to_ancestors_;
}

}

#endif
