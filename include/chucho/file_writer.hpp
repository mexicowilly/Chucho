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

#if !defined(CHUCHO_FILE_WRITER_HPP_)
#define CHUCHO_FILE_WRITER_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/file_descriptor_writer.hpp>
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
class CHUCHO_EXPORT file_writer : public file_descriptor_writer
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
     * @param name the name of this writer
     * @param fmt the formatter
     * @param file_name the name of the file
     * @param start action to take at the start
     * @param flsh whether to flush the file stream after each 
     *             event is written
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::unique_ptr
     */
    file_writer(const std::string& name,
                std::unique_ptr<formatter>&& fmt,
                const std::string& file_name,
                on_start start = on_start::APPEND,
                bool flsh = true);
    //@}

    /**
     * Return the name of this file.
     * 
     * @return the file name
     */
    const std::string& get_file_name() const;
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
     * @param name the name of this writer
     * @param fmt the formatter
     * @param start action to take at the start
     * @param flsh whether to flush the file stream after each
     *             event is written
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::unique_ptr
     */
    file_writer(const std::string& name,
                std::unique_ptr<formatter>&& fmt,
                on_start start,
                bool flsh);
    //@}

    /**
     * Whether we have access to the file is checked every three seconds.
     * If there is no current access to the file, an attempt is made to
     * open it.
     */
    void ensure_access();
    /**
     * Return whether the file is open for business.
     * 
     * @return whether we have opened the file
     */
    bool is_open() const;
    /**
     * Open a file of a new name.
     * 
     * @param file_name the name of the new file
     */
    void open(const std::string& file_name);
    /**
     * Set whether this writer should ever be allowed to create
     * files. This is used by @ref named_pipe_writer. The
     * default behavior is to allow creation.
     * 
     * @param allow whether to allow creation or not
     */
    void set_allow_creation(bool allow);
    virtual void write_impl(const event& evt) override;

private:
    void open_impl(const std::string& file_name);

    std::string initial_file_name_;
    std::string file_name_;
    on_start start_;
    std::chrono::steady_clock::time_point next_access_check_;
    optional<int> writeability_;
    bool is_open_;
    bool has_been_opened_;
    bool allow_creation_;
};

inline const std::string& file_writer::get_file_name() const
{
    return file_name_;
}

inline const std::string& file_writer::get_initial_file_name() const
{
    return initial_file_name_;
}

inline file_writer::on_start file_writer::get_on_start() const
{
    return start_;
}

inline bool file_writer::is_open() const
{
    return is_open_;
}

inline void file_writer::set_allow_creation(bool allow)
{
    allow_creation_ = allow;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
