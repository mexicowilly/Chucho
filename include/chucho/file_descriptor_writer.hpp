/*
 * Copyright 2013-2016 Will Mason
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

/**
 * @class file_descriptor_writer file_descriptor_writer.hpp chucho/file_descriptor_writer.hpp
 * Writing to a file descriptor. This is primarily used as a subclass for
 * other writers, like @ref file_writer. It is not usable in a configuration
 * file because the file descriptor that would be used would have to be
 * known before the application would launch in that context.
 * 
 * This class uses a buffer of 8 KB to cache outgoing messages. In the
 * constructor of this class and in those of all subclasses this
 * buffering can be enabled or disabled. By default, it is disabled.
 * When buffering is enabled, the buffer is flushed only when it fills
 * or when the writer is destroyed. Otherwise, it is flushed after
 * every event is written.
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT file_descriptor_writer : public writer
{
public:
    /**
     * @name Constructors and Destructor
     * @{
     */
    /**
     * Construct a writer.
     * 
     * @param fmt the formatter
     * @param fd the file descriptor
     * @param flsh whether to flush the buffer after every
     * event is written or not
     */
    file_descriptor_writer(std::shared_ptr<formatter> fmt,
                           int fd,
                           bool flsh = true);
    #if defined(_WIN32) || defined(CHUCHO_DOXYGEN_SPECIAL)
    /**
     * Construct a writer.
     * 
     * @note This constructor is only available on Windows.
     * 
     * @param fmt the formatter
     * @param hnd the handle
     * @param flsh whether to flush the buffer after every
     * event is written or not
     */
    file_descriptor_writer(std::shared_ptr<formatter> fmt,
                           HANDLE hnd,
                           bool flsh = true);
    #endif
    /**
     * Destroy a file descriptor writer.
     */
    virtual ~file_descriptor_writer();
    /**
     * @}
     */

    /**
     * Flush the buffer and close the file descriptor.
     */
    void close();
    /**
     * Whether this writer flushes the buffer after each event 
     * is written. 
     * 
     * @return true if flush is enabled
     */
    bool get_flush() const;

protected:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct a file descriptor writer without an associated
     * file descriptor. This constructor is only used by subclasses.
     * 
     * @param fmt the formatter
     * @param flsh whether to flush the buffer after every event has
     * been written
     */
    file_descriptor_writer(std::shared_ptr<formatter> fmt,
                           bool flsh = true);
    /**
     * @}
     */

    /**
     * Set whether the writer can allow the file descriptor to be
     * closed when the writer is destroyed.
     * 
     * @param state true if the writer should allow close, false
     * otherwise
     */
    void set_allow_close(bool state);
    /**
     * Set the file descriptor to which the writer should write.
     * 
     * @param fd the file descriptor
     */
    void set_file_descriptor(int fd);
    #if defined(_WIN32) || defined(CHUCHO_DOXYGEN_SPECIAL)
    /**
     * Set the handle to which the writer should write.
     * 
     * @note This method is only available on Windows.
     * 
     * @param hnd the handle
     */
    void set_file_handle(HANDLE hnd);
    #endif
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT void flush();

    std::array<char, 8 * 1024> buf_;
    std::size_t num_;
    #if defined(_WIN32)
    HANDLE handle_;
    #else
    int fd_;
    #endif
    bool flush_;
    bool allow_close_;
};

inline bool file_descriptor_writer::get_flush() const
{
    return flush_;
}

inline void file_descriptor_writer::set_allow_close(bool state)
{
    allow_close_ = state;
}

}

#endif
