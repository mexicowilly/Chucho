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

#if !defined(CHUCHO_CERR_WRITER_HPP__)
#define CHUCHO_CERR_WRITER_HPP__

#include <chucho/prefix.hpp>
#include <chucho/console_writer.hpp>

namespace chucho
{

/**
 * @class cerr_writer cerr_writer.hpp chucho/cerr_writer.hpp 
 * A @ref console_writer that writes to std::cerr. 
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT cerr_writer : public console_writer
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a cerr_writer.
     * 
     * @param fmt the formatter
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     */
    cerr_writer(std::shared_ptr<formatter> fmt);
    //@}
};

}

#include <chucho/suffix.hpp>

#endif
