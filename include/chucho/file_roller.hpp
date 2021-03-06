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

#if !defined(CHUCHO_FILE_ROLLER_HPP_)
#define CHUCHO_FILE_ROLLER_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/status_reporter.hpp>
#include <chucho/configurable.hpp>
#include <chucho/non_copyable.hpp>
#include <chucho/file_compressor.hpp>
#include <string>

namespace chucho
{

class file_writer;

/**
 * @class file_roller file_roller.hpp chucho/file_roller.hpp
 * Roll files. A file_roller is owned by a @ref 
 * rolling_file_writer and serves as the writer's strategy for 
 * rolling files. The @ref file_roll_trigger determines when a 
 * roll happens, while a file_roller determines how.
 *  
 * @ingroup rolling 
 */
class CHUCHO_EXPORT file_roller : non_copyable,
                                  public virtual status_reporter,
                                  public virtual configurable
{
public:
    /**
     * @name Constructor and Destructor
     * @{
     */
    /**
     * Construct a file roller.
     */
    file_roller(std::unique_ptr<file_compressor>&& cmp = std::move(std::unique_ptr<file_compressor>()));
    /**
     * Destroy the file roller.
     */
    virtual ~file_roller();
    /** @} */

    /**
     * Return this roller's currently active file name. This is the 
     * name of the file to which the writer is currently writing. 
     * 
     * @return the file name
     */
    virtual std::string get_active_file_name() = 0;
    /**
     * Return the file compressor.
     *
     * @post You do not own this pointer.
     *
     * @return the file compressor or nullptr
     */
    file_compressor* get_file_compressor() const;
    /**
     * Roll the files.
     */
    virtual void roll() = 0;
    /**
     * Set the file writer. Each file_roller is owned by a 
     * @ref rolling_file_writer, which calls this method in its 
     * constructor. The file_roller must have access to the writer 
     * during the normal course of its operation. 
     *  
     * @note This method is for Chucho's internal use only. 
     * 
     * @param file_writer the rolling_file_writer that owns this 
     *                    roller
     */
    void set_file_writer(file_writer& file_writer);

protected:
    /**
     * The file writer that owns this roller.
     *  
     * @note You don't own this pointer. It is just a settable 
     * reference. And using shared_from_this() with writer won't 
     * work, since this pointer is set in the constructor. Also, the 
     * writer owns the file_roller, so this pointer can never become 
     * invalid. 
     */
    file_writer* file_writer_;
    /**
     * The @ref file_compressor used by this roller, if there is 
     * one. 
     */
    std::unique_ptr<file_compressor> compressor_;
};

inline file_compressor* file_roller::get_file_compressor() const
{
    return compressor_.get();
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
