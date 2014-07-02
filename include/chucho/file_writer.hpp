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

#if !defined(CHUCHO_FILE_WRITER_HPP__)
#define CHUCHO_FILE_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writer.hpp>
#include <fstream>
#include <string>
#include <chrono>

namespace chucho
{

/**
 * @class file_writer file_writer.hpp chucho/file_writer.hpp
 * A @ref writer that writes to a file. 
 *  
 * @ingroup writers
 */
class CHUCHO_EXPORT file_writer : public writer
{
public:
    /**
     * Whether to append or truncate a file when it is first opened.
     */
    enum class on_start
    {
        /**
         * Append to the file when it is opened.
         */
        APPEND,
        /**
         * Truncate the file when it is opened.
         */
        TRUNCATE
    };

    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a file_writer.
     * 
     * @param fmt the formatter
     * @param file_name the name of the file
     * @param start action to take at the start
     * @param flush whether to flush the file stream after each 
     *              event is written
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     */
    file_writer(std::shared_ptr<formatter> fmt,
                const std::string& file_name,
                on_start start = on_start::APPEND,
                bool flush = true);
    //@}

    /**
     * Return the name of this file.
     * 
     * @return the file name
     */
    const std::string& get_file_name() const;
    /**
     * Whether this writer flushes the flie stream after each event 
     * is written. 
     * 
     * @return true if flush is enabled
     */
    bool get_flush() const;
    /**
     * Return the initial name of this file. 
     *  
     * @note This method is for internal use, as it helps the the 
     *       functionality of the @ref rolling_file_writer. If you
     *       find you ever need it yourself, please drop me a note.
     *       I would find that rather interesting.
     * 
     * @return the initial file name
     */
    const std::string& get_initial_file_name() const;
    /**
     * Return the behavior of this writer when it opens the file for 
     * writing. 
     * 
     * @return the on_start value
     */
    on_start get_on_start() const;

protected:
    /**
     * @name Protected Constructor
     */
    //@{
    /**
     * Construct a file_writer.
     * 
     * @warning This constructor is for internal use. You probably 
     *          won't find it interesting.
     * 
     * @param fmt the formatter
     * @param start action to take at the start
     * @param flush whether to flush the file stream after each
     *              event is written
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     */
    file_writer(std::shared_ptr<formatter> fmt,
                on_start start,
                bool flush);
    //@}

    /**
     * Close the file.
     */
    void close();
    /**
     * Open a file of a new name.
     * 
     * @param file_name the name of the new file
     */
    void open(const std::string& file_name);
    virtual void write_impl(const event& evt) override;

private:
    void ensure_access();

    std::string initial_file_name_;
    std::string file_name_;
    std::ofstream file_;
    on_start start_;
    bool flush_;
    std::chrono::steady_clock::time_point next_access_check_;
    optional<int> writeability_;
};

inline void file_writer::close()
{
    file_.close();
}

inline const std::string& file_writer::get_file_name() const
{
    return file_name_;
}

inline bool file_writer::get_flush() const
{
    return flush_;
}

inline const std::string& file_writer::get_initial_file_name() const
{
    return initial_file_name_;
}

inline file_writer::on_start file_writer::get_on_start() const
{
    return start_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
