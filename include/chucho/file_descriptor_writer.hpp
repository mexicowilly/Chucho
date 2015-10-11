/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_FILE_DESCRIPTOR_WRITER_HPP__)
#define CHUCHO_FILE_DESCRIPTOR_WRITER_HPP__

#include <chucho/writer.hpp>
#include <array>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace chucho
{

class CHUCHO_EXPORT file_descriptor_writer : public writer
{
public:
    file_descriptor_writer(std::shared_ptr<formatter> fmt,
                           int fd,
                           bool flsh = true);
    #if defined(_WIN32)
    file_descriptor_writer(std::shared_ptr<formatter> fmt,
                           HANDLE hnd,
                           bool flsh = true);
    #endif
    virtual ~file_descriptor_writer();

    /**
     * Flush the buffer and close the file descriptor.
     */
    void close();
    /**
     * Whether this writer flushes the flie stream after each event 
     * is written. 
     * 
     * @return true if flush is enabled
     */
    bool get_flush() const;

protected:
    file_descriptor_writer(std::shared_ptr<formatter> fmt,
                           bool flsh = true);

    void set_file_descriptor(int fd);
    #if defined(_WIN32)
    void set_file_handle(HANDLE hnd);
    #endif
    virtual void write_impl(const event& evt) override;

private:
    void flush();

    std::array<char, 8 * 1024> buf_;
    std::size_t num_;
    #if defined(_WIN32)
    HANDLE handle_;
    #else
    int fd_;
    #endif
    bool flush_;
};

inline bool file_descriptor_writer::get_flush() const
{
    return flush_;
}

}

#endif