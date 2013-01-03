#if !defined(CHUCHO_LOGGER_HPP__)
#define CHUCHO_LOGGER_HPP__

#include <chucho/writer.hpp>
#include <vector>

namespace chucho
{

class CHUCHO_EXPORT logger : public std::enable_shared_from_this<logger>,
                             public status_reporter,
                             public configurable
{
public:
    static std::shared_ptr<logger> get(const std::string& name);
    static std::vector<std::shared_ptr<logger>> get_existing_loggers();
    static void remove_unused_loggers();

    logger(const logger&) = delete;
    logger& operator= (const logger&) = delete;

    void add_writer(std::shared_ptr<writer> wrt);
    std::shared_ptr<level> get_effective_level() const;
    std::shared_ptr<level> get_level() const;
    const std::string& get_name() const;
    bool permits(std::shared_ptr<level> lvl) const;
    void set_level(std::shared_ptr<level> lvl);
    void set_writes_to_ancestors(bool val);
    void write(const event& evt);
    bool writes_to_ancestors() const;

private:
    static CHUCHO_NO_EXPORT std::shared_ptr<logger> get_impl(const std::string& name);
    static CHUCHO_NO_EXPORT void static_init();

    CHUCHO_NO_EXPORT logger(const std::string& name, std::shared_ptr<level> lvl = std::shared_ptr<level>());

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

inline bool logger::permits(std::shared_ptr<level> lvl) const
{
    return *lvl <= *get_effective_level();
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
