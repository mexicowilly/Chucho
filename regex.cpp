/*
 * Copyright 2013 Will Mason
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

#include <chucho/regex.hpp>
#include <chucho/regex_exception.hpp>
#include <limits>

extern "C"
{

#include "trex.h"

}

namespace chucho
{

namespace regex
{

expression::expression(const std::string& re)
{
    const char* err = nullptr;
    trex_ = trex_compile(re.c_str(), &err);
    if (err != nullptr && err[0] != 0)
        throw chucho::regex_exception(std::string(err));
}

expression::~expression()
{
    trex_free(trex_);
}

iterator::iterator()
    : re_(nullptr),
      offset_(0)
{
}

iterator::iterator(const std::string& text, expression& re)
    : re_(&re),
      text_(text),
      offset_(0)
{
    operator++();
}

bool iterator::operator== (const iterator& it) const
{
    if (!re_ && !it.re_)
        return true;
    if (!re_)
        return it.offset_ == std::numeric_limits<std::size_t>::max();
    if (!it.re_)
        return offset_ == std::numeric_limits<std::size_t>::max();
    return re_ == it.re_ &&
           text_ == it.text_ &&
           offset_ == it.offset_;
}

iterator& iterator::operator++ ()
{
    if (re_ && offset_ < text_.length())
    {
        match_.subs_.clear();
        const char* found;
        const char* end;
        const char* cur = text_.c_str() + offset_;
        if (trex_search(re_->trex_, cur, &found, &end))
        {
            TRexMatch m;
            for (int i = 0; i < trex_getsubexpcount(re_->trex_); i++)
            {
                if (trex_getsubexp(re_->trex_, i, &m))
                {
                    if (m.len == 0)
                        match_.subs_.push_back(sub_match(-1, 0));
                    else
                        match_.subs_.push_back(sub_match(m.begin - cur + offset_, m.len));
                }
            }
            offset_ = match_.subs_[0].begin() + match_.subs_[0].length();
        }
        else
        {
            offset_ = std::numeric_limits<std::size_t>::max();
        }
    }
    else
    {
        offset_ = std::numeric_limits<std::size_t>::max();
    }
    return *this;
}

std::string replace(const std::string& text, expression& re, const std::string& rep)
{
    const char* cur = text.c_str();
    const char* end = text.data() + text.length();
    const char* found;
    const char* last;
    std::string result;
    while (cur < end && trex_search(re.trex_, cur, &found, &last))
    {
        result.append(cur, found);
        result.append(rep);
        cur = last;
    }
    result.append(cur, end - cur);
    return result;
}

bool search(const std::string& text, expression& re)
{
    const char* found;
    const char* last;
    return trex_search(re.trex_, text.c_str(), &found, &last) == TRex_True;
}

bool search(const std::string& text, expression& re, match& mch)
{
    const char* found;
    const char* last;
    mch.subs_.clear();
    if (trex_search(re.trex_, text.c_str(), &found, &last))
    {
	TRexMatch m;
	for (int i = 0; i < trex_getsubexpcount(re.trex_); i++)
	{
	    if (trex_getsubexp(re.trex_, i, &m))
	    {
		if (m.len == 0)
		    mch.subs_.push_back(sub_match(-1, 0));
		else
		    mch.subs_.push_back(sub_match(m.begin - text.c_str(), m.len));
	    }
	}
	return true;
    }
    return false;
}

}

}
