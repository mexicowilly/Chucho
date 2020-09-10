/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_PIPE_WRITER_HPP__)
#define CHUCHO_PIPE_WRITER_HPP__

#include <chucho/file_descriptor_writer.hpp>

namespace chucho
{

/**
 * @class pipe_writer pipe_writer.hpp chucho/pipe_writer.hpp
 * The pipe writer creates a pipe and attaches itself to the
 * output end of it. The input end of the pipe can be retrieved
 * with the @ref get_input() method. When the pipe writer is
 * destroyed, both the input and output ends of it are closed.
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT pipe_writer : public file_descriptor_writer
{
public:
    /**
     * A platform-dependent type for holding a pipe descriptor.
     * This is HANDLE on Windows and int on other operating
     * systems.
     */
    #if defined(CHUCHO_DOXYGEN_SPECIAL)
    typedef <platform dependent> pipe_type;
    #elif defined(_WIN32)
    typedef HANDLE pipe_type;
    #else
    typedef int pipe_type;
    #endif

    /**
     * @name Constructor and Destructor
     * @{
     */
    /**
     * Construct a pipe writer. The constructor creates the
     * pipe and attaches itself to the output end.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param flsh whether to flush the buffer after writing
     * each event
     */
    pipe_writer(const std::string& name,
                std::unique_ptr<formatter>&& fmt,
                bool flsh = true);
    /**
     * Destroy the pipe, closing both the input and output
     * ends of it.
     */
    virtual ~pipe_writer();
    /**
     * @}
     */

    /**
     * Return the input end of the pipe.
     * 
     * @return the input
     */
    pipe_type get_input() const;
    /**
     * Return the output end of the pipe.
     * 
     * @return the output
     */
    pipe_type get_output() const;

private:
    pipe_type input_;
    pipe_type output_;
};

inline pipe_writer::pipe_type pipe_writer::get_input() const
{
    return input_;
}

inline pipe_writer::pipe_type pipe_writer::get_output() const
{
    return output_;
}

}

#endif
