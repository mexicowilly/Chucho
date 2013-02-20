#include <chucho/regex.hpp>
#include <chucho/regex_exception.hpp>
#include <regex.h>

namespace chucho
{

namespace regex
{

struct expression_handle
{
    regex_t re_;
};

struct iterator_handle
{
    iterator_handle(const std::string& text, expression& re);

    std::string text_;
    const char* cur_;
    expression re_;
};

expression::expression(const std::string& re, int flags)
    : handle_(std::make_shared<expression_handle>())
{
    int pflags = REG_EXTENDED;
    if (flags & ignore_case)
        pflags |= REG_ICASE;
    int rc = regcomp(&handle_->re_, re.c_str(), pflags);
    if (rc != 0)
    {
        std::size_t req = regerror(rc, &handle_->re_, nullptr, 0);
        std::vector<char> buf(req);
        regerror(rc, &handle_->re_, &buf[0], req);
        throw chucho::regex_exception(std::string(&buf[0]));
    }
}

expression::expression(const expression& ex)
{
    if (handle_ && handle_.unique())
        regfree(&handle_->re_);
    handle_ = ex.handle_;
}

expression::~expression()
{
    if (handle_ && handle_.unique())
        regfree(&handle_->re_);
}

expression& expression::operator= (const expression& ex)
{
    if (handle_ && handle_.unique())
        regfree(&handle_->re_);
    handle_ = ex.handle_;
    return *this;
}

iterator::iterator()
{
}

iterator::iterator(const std::string& text, expression& re, std::size_t groups)
    : handle_(std::make_shared<iterator_handle>(text, re)),
      groups_(groups)
{
    operator++();
}

bool iterator::operator== (const iterator& it) const
{
    if (!handle_ && !it.handle_)
        return true;
    if (!handle_)
        return it.handle_->cur_ == it.handle_->text_.c_str() + it.handle_->text_.length();
    if (!it.handle_)
        return handle_->cur_ == handle_->text_.c_str() + handle_->text_.length();
    return handle_->text_ == it.handle_->text_ &&
           handle_->cur_ == it.handle_->cur_ &&
           handle_->re_.handle_ == it.handle_->re_.handle_;
}

iterator& iterator::operator++ ()
{
    if (handle_ && handle_->cur_ < (handle_->text_.c_str() + handle_->text_.length()))
    {
        match_.subs_.clear();
        std::vector<regmatch_t> rmtchs(groups_ + 1);
        std::ptrdiff_t offset = handle_->cur_ - handle_->text_.c_str();
        if (regexec(&handle_->re_.handle_->re_,
                    handle_->cur_,
                    rmtchs.size(),
                    &rmtchs[0],
                    0) == 0)
        {
            for (unsigned idx = 0; idx < rmtchs.size(); idx++)
            {
                if (rmtchs[idx].rm_so == -1)
                {
                    match_.subs_.push_back(sub_match(-1, 0));
                }
                else
                {
                    match_.subs_.push_back(sub_match(rmtchs[idx].rm_so + offset,
                                                     rmtchs[idx].rm_eo - rmtchs[idx].rm_so));
                }
            }
            handle_->cur_ = handle_->text_.c_str() + rmtchs[0].rm_eo + offset;
        }
        else
        {
            handle_->cur_ = handle_->text_.c_str() + handle_->text_.length();
        }
    }
    return *this;
}

iterator iterator::operator++ (int)
{
    iterator pre = *this;
    operator++();
    return pre;
}

iterator_handle::iterator_handle(const std::string& text, expression& re)
    : text_(text),
      cur_(text.c_str()),
      re_(re)
{
}

sub_match::sub_match(int begin, std::size_t length)
    : begin_(begin),
      length_(length)
{
}

std::string replace(const std::string& text, expression& re, const std::string& rep)
{
    regmatch_t match;
    const char* cur = text.c_str();
    const char* end = text.data() + text.length();
    std::string result;
    while (cur < end &&
           regexec(&re.handle_->re_, cur, 1, &match, 0) == 0)
    {
        result.append(cur, match.rm_so);
        result.append(rep);
        cur += match.rm_eo;
    }
    result.append(cur, end - cur);
    return result;
}

bool search(const std::string& text, expression& re)
{
    return regexec(&re.handle_->re_, text.c_str(), 0, nullptr, 0) == 0;
}

}

}
