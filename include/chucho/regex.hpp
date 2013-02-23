#if !defined(CHUCHO_REGEX_HPP__)
#define CHUCHO_REGEX_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/export.hpp>
#include <memory>
#include <string>
#include <iterator>
#include <vector>

struct TRex;

namespace chucho
{

namespace regex
{

struct CHUCHO_EXPORT expression
{
public:
    expression(const std::string& re);
    expression(const expression& ex) = delete;
    ~expression();

    expression& operator= (const expression& ex) = delete;

    TRex* trex_;
};

class CHUCHO_EXPORT sub_match
{
public:
    sub_match(int begin, std::size_t length);

    int begin() const;
    std::size_t length() const;

private:
    int begin_;
    std::size_t length_;
};

class CHUCHO_EXPORT match
{
public:
    const sub_match& operator[] (unsigned idx) const;
    std::size_t size() const;

protected:
    friend class iterator;

private:
    std::vector<sub_match> subs_;
};

class CHUCHO_EXPORT iterator : public std::iterator<std::forward_iterator_tag, match>
{
public:
    iterator();
    iterator(const std::string& text, expression& re);

    bool operator== (const iterator& it) const;
    bool operator!= (const iterator& it) const;
    iterator& operator++ ();
    iterator operator++ (int);
    reference operator* ();

private:
    expression* re_;
    std::string text_;
    std::size_t offset_;
    match match_;
};

CHUCHO_EXPORT std::string replace(const std::string& text, expression& re, const std::string& rep);
CHUCHO_EXPORT bool search(const std::string& text, expression& re);

inline sub_match::sub_match(int begin, std::size_t length)
    : begin_(begin),
      length_(length)
{
}

inline int sub_match::begin() const
{
    return begin_;
}

inline std::size_t sub_match::length() const
{
    return length_;
}

inline const sub_match& match::operator[] (unsigned idx) const
{
    return subs_[idx];
}

inline std::size_t match::size() const
{
    return subs_.size();
}

inline bool iterator::operator!= (const iterator& it) const
{
    return !operator==(it);
}

inline iterator iterator::operator++ (int)
{
    iterator pre = *this;
    operator++();
    return pre;
}

inline iterator::reference iterator::operator* ()
{
    return match_;
}

}

}

#endif
