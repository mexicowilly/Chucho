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
#if defined(CHUCHO_HAVE_STD_REGEX)
#include <regex>
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
#include <regex.h>
#endif

namespace chucho
{

namespace regex
{

struct expression_impl
{
#if defined(CHUCHO_HAVE_STD_REGEX)
    std::regex re_;
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
    regex_t re_;
#endif
};

expression::expression(const std::string& re)
    : pimpl_(new expression_impl)
{
#if defined(CHUCHO_HAVE_STD_REGEX)
    pimpl_->re_ = std::regex(re, std::regex_constants::extended);
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
    int rc = regcomp(&pimpl_->re_, re.c_str(), REG_EXTENDED);
    if (rc != 0)
    {
	char buf[1024];
	regerror(rc, &pimpl_->re_, buf, sizeof(buf));
	throw chucho::regex_exception(buf);
    }
#endif
}

expression::~expression()
{
#if defined(CHUCHO_HAVE_POSIX_REGEX)
    regfree(&pimpl_->re_);
#endif
    delete pimpl_;
}

struct iterator_impl
{
#if defined(CHUCHO_HAVE_STD_REGEX)
    std::sregex_iterator itor_;
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
    expression* re_;
    std::size_t offset_;
#endif
};

iterator::iterator()
    : pimpl_(new iterator_impl)
{
#if defined(CHUCHO_HAVE_POSIX_REGEX)
    pimpl_->re_ = nullptr;
    pimpl_->offset_ = 0;
#endif
}

iterator::iterator(const std::string& text, expression& re)
    : text_(text),
      pimpl_(new iterator_impl)
{
#if defined(CHUCHO_HAVE_STD_REGEX)
      pimpl_->itor_ = std::sregex_iterator(text_.begin(), text_.end(), re.pimpl_->re_);
      std::smatch mch = *pimpl_->itor_;
      for (int i = 0; i < mch.size(); i++)
      {
	  if (mch[i].matched)
	      match_.subs_.push_back(sub_match(mch.position(i), mch.length(i)));
	  else
	      match_.subs_.push_back(sub_match(-1, 0));
      }
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
      pimpl_->re_ = &re;
      pimpl_->offset_ = 0;
#endif
#if !defined(CHUCHO_HAVE_STD_REGEX)
    operator++();
#endif
}

iterator::iterator(const iterator& it)
    : text_(it.text_),
      match_(it.match_),
      pimpl_(new iterator_impl(*it.pimpl_))
{
}

iterator::~iterator()
{
    delete pimpl_;
}

iterator& iterator::operator= (const iterator& it)
{
    if (&it != this)
    {
	text_ = it.text_;
	match_ = it.match_;
	delete pimpl_;
	pimpl_ = new iterator_impl(*it.pimpl_);
    }
    return *this;
}

bool iterator::operator== (const iterator& it) const
{
#if defined(CHUCHO_HAVE_STD_REGEX)
    return pimpl_->itor_ == it.pimpl_->itor_;
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
    if (!pimpl_->re_ && !it.pimpl_->re_)
        return true;
    if (pimpl_->re_ == nullptr)
        return it.pimpl_->offset_ == std::numeric_limits<std::size_t>::max();
    if (it.pimpl_->re_ == nullptr)
        return pimpl_->offset_ == std::numeric_limits<std::size_t>::max();
    return pimpl_->re_ == it.pimpl_->re_ &&
           text_ == it.text_ &&
           pimpl_->offset_ == it.pimpl_->offset_;
#endif
}

iterator& iterator::operator++ ()
{
    match_.subs_.clear();
#if defined(CHUCHO_HAVE_STD_REGEX)
    std::smatch mch = *++pimpl_->itor_;
    for (int i = 0; i < mch.size(); i++)
    {
	if (mch[i].matched)
	    match_.subs_.push_back(sub_match(mch.position(i), mch.length(i)));
	else
	    match_.subs_.push_back(sub_match(-1, 0));
    }
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
    if (pimpl_->re_ && pimpl_->offset_ < text_.length())
    {
	regex_t& re(pimpl_->re_->pimpl_->re_);
	auto sub_count = re.re_nsub + 1;
	regmatch_t pmatch[sub_count];
        const char* cur = text_.c_str() + pimpl_->offset_;
	int rc = regexec(&re, cur, sub_count, pmatch, 0);
	if (rc == 0)
	{
	    for (int i = 0; i < sub_count; i++)
	    {
		if (pmatch[i].rm_so == -1)
		    match_.subs_.push_back(sub_match(-1, 0));
		else
		    match_.subs_.push_back(sub_match(pmatch[i].rm_so + pimpl_->offset_, pmatch[i].rm_eo - pmatch[i].rm_so));
	    }
            pimpl_->offset_ = match_.subs_[0].begin() + match_.subs_[0].length();
        }
        else
        {
            pimpl_->offset_ = std::numeric_limits<std::size_t>::max();
        }
    }
    else
    {
        pimpl_->offset_ = std::numeric_limits<std::size_t>::max();
    }
#endif
    return *this;
}

std::string replace(const std::string& text, expression& re, const std::string& rep)
{
#if defined(CHUCHO_HAVE_STD_REGEX)
    return std::regex_replace(text, re.pimpl_->re_, rep);
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
    const char* cur = text.c_str();
    const char* end = text.data() + text.length();
    std::string result;
    regmatch_t mch;
    while (cur < end && regexec(&re.pimpl_->re_, cur, 1, &mch, 0) == 0)
    {
        result.append(cur, mch.rm_so);
        result.append(rep);
        cur += mch.rm_eo;
    }
    result.append(cur, end - cur);
    return result;
#endif
}

bool search(const std::string& text, expression& re)
{
#if defined(CHUCHO_HAVE_STD_REGEX)
    return std::regex_search(text, re.pimpl_->re_);
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
    return regexec(&re.pimpl_->re_, text.c_str(), 0, nullptr, 0) == 0;
#endif
}

bool search(const std::string& text, expression& re, match& mch)
{
    mch.subs_.clear();
#if defined(CHUCHO_HAVE_STD_REGEX)
    std::smatch res;
    if (std::regex_search(text, res, re.pimpl_->re_))
    {
	for (int i = 0; i < res.size(); i++)
	{
	    if (res[i].matched)
		mch.subs_.push_back(sub_match(res.position(i), res.length(i)));
	    else
		mch.subs_.push_back(sub_match(-1, 0));
	}
	return true;
    }
#elif defined(CHUCHO_HAVE_POSIX_REGEX)
    auto sub_count = re.pimpl_->re_.re_nsub + 1;
    regmatch_t pmatch[sub_count];
    if (regexec(&re.pimpl_->re_, text.c_str(), sub_count, pmatch, 0) == 0)
    {
	for (int i = 0; i < sub_count; i++)
	{
	    if (pmatch[i].rm_so == -1)
		mch.subs_.push_back(sub_match(-1, 0));
	    else
		mch.subs_.push_back(sub_match(pmatch[i].rm_so, pmatch[i].rm_eo - pmatch[i].rm_so));
	}
	return true;
    }
#endif
    return false;
}

}

}
