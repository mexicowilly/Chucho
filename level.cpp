#include <chucho/level.hpp>
#include <chucho/exception.hpp>
#include <limits>
#include <vector>
#include <algorithm>

namespace
{

class trace : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class debug : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class info : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class warn : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class error : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class fatal : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class off : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

const char* trace::get_name() const
{
    return "TRACE";
}

int trace::get_value() const
{
    return 0;
}

const char* debug::get_name() const
{
    return "DEBUG";
}

int debug::get_value() const
{
    return 10000;
}

const char* info::get_name() const
{
    return "INFO";
}

int info::get_value() const
{
    return 20000;
}

const char* warn::get_name() const
{
    return "WARN";
}

int warn::get_value() const
{
    return 30000;
}

const char* error::get_name() const
{
    return "ERROR";
}

int error::get_value() const
{
    return 40000;
}

const char* fatal::get_name() const
{
    return "FATAL";
}

int fatal::get_value() const
{
    return 50000;
}

const char* off::get_name() const
{
    return "OFF";
}

int off::get_value() const
{
    return std::numeric_limits<int>::max();
}

}

namespace chucho
{

std::shared_ptr<level> level::TRACE(new trace());
std::shared_ptr<level> level::DEBUG(new debug());
std::shared_ptr<level> level::INFO(new info());
std::shared_ptr<level> level::WARN(new warn());
std::shared_ptr<level> level::ERROR(new error());
std::shared_ptr<level> level::FATAL(new fatal());
std::shared_ptr<level> level::OFF(new off());

std::ostream& operator<< (std::ostream& stream, const level& lvl)
{
    stream << lvl.get_name();
    return stream;
}

level::~level()
{
}

std::shared_ptr<level> level::from_text(const std::string& text)
{
    std::string up;
    std::transform(text.begin(),
                   text.end(),
                   std::back_inserter(up),
                   (int(*)(int))std::toupper);
    std::vector<std::shared_ptr<level>> lvls =
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        OFF
    };
    for (std::shared_ptr<level> lvl : lvls)
    {
        if (up == lvl->get_name())
            return lvl;
    }
    throw exception("The text " + text + " does not describe a valid log level");
}

}
