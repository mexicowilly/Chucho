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

#if !defined(CHUCHO_DATABASE_WRITER_HPP__)
#define CHUCHO_DATABASE_WRITER_HPP__

#include <chucho/writer.hpp>

namespace chucho
{

/**
 * @class database_writer database_writer.hpp chucho/database_writer.hpp
 * A writer that writes to a database.
 * 
 * @ingroup writers database
 */
class database_writer : public writer
{
protected:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a database writer.
     *
     * @param fmt the formatter
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     */
    database_writer(std::shared_ptr<formatter> fmt);
    //@}
};

}

#endif
