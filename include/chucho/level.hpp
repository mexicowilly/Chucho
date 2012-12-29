#if !defined(CHUCHO_LEVEL_HPP__)
#define CHUCHO_LEVEL_HPP__

#include <chucho/export.hpp>
#include <ostream>

namespace chucho
{

class level;

extern CHUCHO_EXPORT std::shared_ptr<level> TRACE_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> DEBUG_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> INFO_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> WARN_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> ERROR_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> FATAL_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> OFF_LEVEL;

class CHUCHO_EXPORT level
{
public:
    static std::shared_ptr<level> from_text(const std::string& text);

    virtual ~level();

    bool operator== (const level& other) const;
    bool operator!= (const level& other) const;
    bool operator< (const level& other) const;
    bool operator<= (const level& other) const;
    bool operator> (const level& other) const;
    bool operator>= (const level& other) const;

    virtual const char* get_name() const = 0;
    virtual int get_value() const = 0;
};

CHUCHO_EXPORT std::ostream& operator<< (std::ostream& stream, const level& lvl);

class CHUCHO_EXPORT trace_level : public level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class CHUCHO_EXPORT debug_level : public level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class CHUCHO_EXPORT info_level : public level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class CHUCHO_EXPORT warn_level : public level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class CHUCHO_EXPORT error_level : public level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class CHUCHO_EXPORT fatal_level : public level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

class CHUCHO_EXPORT off_level : public level
{
public:
    virtual const char* get_name() const override;
    virtual int get_value() const override;
};

inline bool level::operator== (const level& other) const
{
    return get_value() == other.get_value();
}

inline bool level::operator!= (const level& other) const
{
    return get_value() != other.get_value();
}

inline bool level::operator< (const level& other) const
{
    return get_value() < other.get_value();
}

inline bool level::operator<= (const level& other) const
{
    return get_value() <= other.get_value();
}

inline bool level::operator> (const level& other) const
{
    return get_value() > other.get_value();
}

inline bool level::operator>= (const level& other) const
{
    return get_value() >= other.get_value();
}

}

#endif
