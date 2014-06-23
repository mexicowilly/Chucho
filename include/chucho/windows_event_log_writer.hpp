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

#if !defined(CHUCHO_WINDOWS_EVENT_LOG_WRITER__)
#define CHUCHO_WINDOWS_EVENT_LOG_WRITER__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writer.hpp>
#include <windows.h>

namespace chucho
{

/**
 * @class windows_event_log_writer windows_event_log_writer.hpp chucho/windows_event_log_writer.hpp 
 * A writer that sends message to Windows Event Log. This writer 
 * requires some special handling. Please refer to the <a 
 * href="https://www.assembla.com/spaces/chucho/wiki/Windows_Event_Log" 
 * target="_blank">instructions</a> in order to set it up 
 * properly. This writer is only available on Windows platforms.
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT windows_event_log_writer : public writer
{
public:
    /**
     * @name Constructors
     */
    //@{
    /**
     * Construct a writer.
     * 
     * @param fmt the formatter
     * @param source the name of the source, which becomes a 
     *               registry key
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw std::invalid_argument if the source is empty
     * @throw exception if there is a problem registering the event 
     *        source
     */
    windows_event_log_writer(std::shared_ptr<formatter> fmt,
                             const std::string& source);
    /**
     * Construct a writer.
     * 
     * @param fmt the formatter
     * @param log the name of the log, which becomes a registry key
     * @param source the name of the source, which becomes a 
     *               registry key
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw std::invalid_argument if the source is empty
     * @throw exception if there is a problem registering the event 
     *        source
     */
    windows_event_log_writer(std::shared_ptr<formatter> fmt,
                             const std::string& log,
                             const std::string& source);
    /**
     * Construct a writer.
     * 
     * @param fmt the formatter
     * @param log the name of the log, which becomes a registry key
     * @param source the name of the source, which becomes a 
     *               registry key
     * @param host the name of the host to which you are writing 
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw std::invalid_argument if the source is empty
     * @throw exception if there is a problem registering the event 
     *        source
     */
    windows_event_log_writer(std::shared_ptr<formatter> fmt,
                             const std::string& log,
                             const std::string& source,
                             const std::string& host);
    //@}
    /**
     * @name Destructor
     */
    //@{
    /**
     * Destroy the writer.
     */
    ~windows_event_log_writer();
    //@}

    /**
     * Return the host name. If the host name has not been set then 
     * this value will be empty. 
     * 
     * @return the host name
     */
    const std::string& get_host() const;
    /**
     * Return the name of the log.
     * 
     * @return the log name
     */
    const std::string& get_log() const;
    /**
     * Return the name of the source.
     * 
     * @return the source name
     */
    const std::string& get_source() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    void init();
    void look_up_user();
    void prepare_registry();

    HANDLE handle_;
    SID* user_;
    std::string log_;
    std::string source_;
    std::string host_;
};

inline const std::string& windows_event_log_writer::get_host() const
{
    return host_;
}

inline const std::string& windows_event_log_writer::get_log() const
{
    return log_;
}

inline const std::string& windows_event_log_writer::get_source() const
{
    return source_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
