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

#include "writer_creator_item.hpp"
#include "cout_writer_item.hpp"
#include "cerr_writer_item.hpp"
#include "file_writer_item.hpp"

namespace chucho
{

namespace config
{

writer_creator_item::writer_creator_item(QTreeWidget& tree)
    : creator_from_list_item(tree, "<Add Writer>")
{
    creators_["ActiveMQ Writer"] = nullptr;
    creators_["Asynchronous Writer"] = nullptr;
    creators_["DB2 Writer"] = nullptr;
    creators_["Door Writer"] = nullptr;
    creators_["Email Writer"] = nullptr;
    creators_["File Writer"] = get_func<file_writer_item>();
    creators_["MySQL Writer"] = nullptr;
    creators_["Named Pipe Writer"] = nullptr;
    creators_["Oracle Writer"] = nullptr;
    creators_["Pipe Writer"] = nullptr;
    creators_["PostgresSQL Writer"] = nullptr;
    creators_["RabbitMQ Writer"] = nullptr;
    creators_["Remote Chucho Writer"] = nullptr;
    creators_["Rolling File Writer"] = nullptr;
    creators_["SQLite Writer"] = nullptr;
    creators_["Standard Error Writer"] = get_func<cerr_writer_item>();
    creators_["Standard Output Writer"] = get_func<cout_writer_item>();
    creators_["Syslog Writer"] = nullptr;
    creators_["Windows Event Log Writer"] = nullptr;
    creators_["ZeroMQ Writer"] = nullptr;
}

}

}