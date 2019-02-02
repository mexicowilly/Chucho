/*
 * Copyright 2013-2019 Will Mason
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

#ifndef CHUCHO_CLOUD_WRITER_HPP__
#define CHUCHO_CLOUD_WRITER_HPP__

#include <chucho/writer.hpp>

namespace chucho
{

/**
 * @class cloud_writer cloud_writer.hpp chucho/cloud_writer.hpp
 * A writer that writes to the cloud.
 *
 * @ingroup writers
 */
class cloud_writer : public writer
{
public:
    /**
     * @name Contructor
     * @{
     */
    /**
     * Construct a cloud writer.
     *
     * @param name the name of the writer
     * @param fmt the formatter
     */
    cloud_writer(const std::string& name, std::unique_ptr<formatter>&& fmt);
    /**
     * @}
     */
};

inline cloud_writer::cloud_writer(const std::string& name, std::unique_ptr<chucho::formatter>&& fmt)
    : writer(name, std::move(fmt))
{
}

}

#endif
