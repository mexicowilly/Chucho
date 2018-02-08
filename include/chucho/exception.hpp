/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_EXCEPTION_HPP__)
#define CHUCHO_EXCEPTION_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/export.h>
#include <exception>
#include <string>

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
#define CHUCHO_NOEXCEPT throw()
#else
#define CHUCHO_NOEXCEPT noexcept
#endif

namespace chucho
{

/**
 * @class exception exception.hpp chucho/exception.hpp
 * Chucho exceptions.
 *  
 * @ingroup miscellaneous 
 */
class CHUCHO_EXPORT exception : public std::exception
{
public:
    /**
     * Find the messages from all nested exceptions and piece them 
     * together. 
     * 
     * @param e the exception from which to extract nested messages
     * @return A brack-delimited piece of text containing all nested 
     *         messages
     */
    static std::string nested_whats(const std::exception& e);

    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct an exception.
     * 
     * @param msg the exception's message
     */
    exception(const std::string& msg);
    //@}

    /**
     * Return the exception's message.
     * 
     * @return the message
     */
    virtual const char* what() const CHUCHO_NOEXCEPT override;

protected:
    /**
     * The message
     */
    std::string message_;
};

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
