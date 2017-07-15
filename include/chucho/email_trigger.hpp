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

#if !defined(CHUCHO_EMAIL_TRIGGER_HPP__)
#define CHUCHO_EMAIL_TRIGGER_HPP__

#include <chucho/event.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/configurable.hpp>

namespace chucho
{

/**
 * @class email_trigger email_trigger.hpp chucho/email_trigger.hpp
 * An email trigger is used by @ref email_writer to tell it when
 * email should be sent.
 * 
 * @ingroup email
 */
class CHUCHO_EXPORT email_trigger : public status_reporter,
                                    public configurable
{
public:
    virtual ~email_trigger() { }

    /**
     * Return true if the given event should cause an email
     * to be sent.
     * 
     * @param evt the event
     * @return true if this event inspires an email
     */
    virtual bool is_triggered(const event& evt) = 0;
};

}

#endif
