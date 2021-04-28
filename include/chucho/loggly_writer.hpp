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

#ifndef CHUCHO_LOGGLY_WRITER_HPP_
#define CHUCHO_LOGGLY_WRITER_HPP_

#include <chucho/cloud_writer.hpp>

namespace chucho
{

class curl;

/**
 * @class loggly_writer loggly_writer.hpp chucho/loggly_writer.hpp
 * Write to Loggly. This writer must make an HTTP connection
 * and transfer for every event, so it would probably be best to
 * use it with an @ref async_writer.
 *
 * @ingroup writers
 */
class CHUCHO_EXPORT loggly_writer : public cloud_writer
{
public:
    /**
     * @name Constructor and Destructor
     * @{
     */
    /**
     * Construct a Loggly writer.
     * @param name the writer's name
     * @param fmt the formatter
     * @param token the Loggly customer token
     */
    loggly_writer(const std::string& name,
                  std::unique_ptr<formatter>&& fmt,
                  const std::string& token);
    /**
     * Destruct a Loggly writer.
     */
    virtual ~loggly_writer();
    /**
     * @}
     */

    /**
     * Return the Loggly customer token.
     *
     * @return the customer token
     */
    const std::string& get_token() const;
    /**
     * Return whether the curl output is verbose.
     *
     * @return verbose or not
     */
    bool get_verbose() const;
    /**
     * Set whether the curl output should be verbose or not.
     *
     * @param state verbose or not
     */
    void set_verbose(bool state);

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::unique_ptr<curl> curl_;
    std::string token_;
};

inline const std::string& loggly_writer::get_token() const
{
    return token_;
}

}

#endif
