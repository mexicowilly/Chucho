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

#if !defined(CHUCHO_WRITER_EMITTER_HPP__)
#define CHUCHO_WRITER_EMITTER_HPP__

#include "formatter_emitter.hpp"
#include "filter_emitter.hpp"

namespace chucho
{

namespace config_tool
{

class writer_emitter : public emitter
{
public:
    void add_filter_emitter(std::shared_ptr<filter_emitter> flt);
    virtual void emit(std::ostream& stream, std::size_t shifts) override;
    const std::vector<std::shared_ptr<filter_emitter>>& get_filter_emitters() const;
    std::shared_ptr<formatter_emitter> get_formatter_emitter() const;
    virtual const char* get_name() const = 0;
    void set_formatter_emitter(std::shared_ptr<formatter_emitter> fmt);

protected:
    writer_emitter(const properties& props,
                   std::shared_ptr<formatter_emitter> fmt,
                   const std::vector<std::shared_ptr<filter_emitter>>& flts);

private:
    std::shared_ptr<formatter_emitter> fmt_;
    std::vector<std::shared_ptr<filter_emitter>> flts_;
};

inline void writer_emitter::add_filter_emitter(std::shared_ptr<filter_emitter> flt)
{
    flts_.push_back(flt);
}

inline const std::vector<std::shared_ptr<filter_emitter>>& writer_emitter::get_filter_emitters() const
{
    return flts_;
}

inline std::shared_ptr<formatter_emitter> writer_emitter::get_formatter_emitter() const
{
    return fmt_;
}

inline void writer_emitter::set_formatter_emitter(std::shared_ptr<formatter_emitter> fmt)
{
    fmt_ = fmt;
}

}

}

#endif
