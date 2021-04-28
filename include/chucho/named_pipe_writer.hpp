/*
 * Copyright 2013-2021 Will Mason
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

#if !defined(CHUCHO_NAMED_PIPE_WRITER_HPP_)
#define CHUCHO_NAMED_PIPE_WRITER_HPP_

#include <chucho/file_writer.hpp>

namespace chucho
{

/**
 * @class named_pipe_writer named_pipe_writer.hpp chucho/named_pipe_writer.hpp
 * A writer for writing to named pipes. This class works just as
 * @ref file_writer but with two important differences. This class will
 * never try to create the named pipe. The named pipe must exist for this
 * writer to be able to write. Just as a @ref file_writer will, this
 * class will check for access every three seconds and open the file as
 * needed. But this class will never try to create the file in question
 * or any of the parent directories that may be part of its name.
 * 
 * On Windows, you may pass an unadorned name as the name of the pipe.
 * For example, if the name passed in the constructor here is
 * my_dog_has_fleas, then the name will be fleshed out completely as
 * @c \\\\\.\\pipe\\my_dog_has_fleas. On POSIX systems, you must pass the
 * file system name associated with the pipe, which may be a relative
 * path if you know your current working directory.
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT named_pipe_writer : public file_writer
{
public:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct a named pipe writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param pipe_name the name of the writer, which can be an unadorned
     * name on Windows
     * @param flsh whether to flush the buffer after every event
     * is written
     */
    named_pipe_writer(const std::string& name,
                      std::unique_ptr<formatter>&& fmt,
                      const std::string& pipe_name,
                      bool flsh = true);
    /**
     * @}
     */

private:
    std::string normalize_name(const std::string& name);
};

}

#endif
