/*
 * Copyright 2013 Will Mason
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

#if !defined(CHUCHO_FILE_ROLLER_HPP__)
#define CHUCHO_FILE_ROLLER_HPP__

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
     * @name Constructor
     */
    //@{
    /**
     * Construct a file roller.
     */
    file_roller(std::shared_ptr<file_compressor> cmp = std::shared_ptr<file_compressor>());
    //@}

    /**
     * Return this roller's currently active file name. This is the 
     * name of the file to which the writer is currently writing. 
     * 
     * @return the file name
     */
    virtual std::string get_active_file_name() = 0;
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
    std::shared_ptr<file_compressor> compressor_;
};

}

#endif
