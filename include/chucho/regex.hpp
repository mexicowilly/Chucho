/*
 * Copyright 2013-2014 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#if !defined(CHUCHO_REGEX_HPP__)
#define CHUCHO_REGEX_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/prefix.hpp>
#include <chucho/export.hpp>
#include <chucho/non_copyable.hpp>
#include <memory>
#include <string>
#include <iterator>
#include <vector>

namespace chucho
{

namespace regex
{

struct expression_impl;

struct CHUCHO_PRIV_EXPORT expression : non_copyable
{
    expression(const std::string& re);
    ~expression();

    expression_impl* pimpl_;
};

class CHUCHO_PRIV_EXPORT sub_match
{
public:
    sub_match(int begin, std::size_t length);

    int begin() const;
    std::size_t length() const;

private:
    int begin_;
    std::size_t length_;
};

class CHUCHO_PRIV_EXPORT match
{
public:
    const sub_match& operator[] (unsigned idx) const;
    std::size_t size() const;

protected:
    friend class iterator;
    friend CHUCHO_PRIV_EXPORT bool search(const std::string& text, expression& re, match& mch);

private:
    std::vector<sub_match> subs_;
};

struct iterator_impl;

class CHUCHO_PRIV_EXPORT iterator : public std::iterator<std::forward_iterator_tag, match>
{
public:
    iterator();
    iterator(const std::string& text, expression& re);
    iterator(const iterator& it);
    ~iterator();

    iterator& operator= (const iterator& it);
    bool operator== (const iterator& it) const;
    bool operator!= (const iterator& it) const;
    iterator& operator++ ();
    iterator operator++ (int);
    reference operator* ();

private:
    std::string text_;
    match match_;
    iterator_impl* pimpl_;
};

CHUCHO_PRIV_EXPORT std::string replace(const std::string& text, expression& re, const std::string& rep);
CHUCHO_PRIV_EXPORT bool search(const std::string& text, expression& re);
CHUCHO_PRIV_EXPORT bool search(const std::string& text, expression& re, match& mch);

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

#include <chucho/suffix.hpp>

#endif
