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

#if !defined(CHUCHO_ORACLE_WRITER_HPP__)
#define CHUCHO_ORACLE_WRITER_HPP__

#include <chucho/database_writer.hpp>
#include "oci.h"

namespace chucho
{

class oracle_writer : public database_writer
{
public:
    oracle_writer(std::shared_ptr<formatter> fmt,
                  const std::string& user,
                  const std::string& password,
                  const std::string& database);
    ~oracle_writer();

protected:
    virtual void write_impl(const event& evt) override;

private:
    void finalize();
    void react(sword code, const std::string& msg, bool final);

    OCIEnv* env_;
    OCIError* err_;
    OCISvcCtx* ctx_;
    OCIStmt* insert_evt_;
    OCIBind* formatted_msg_;
    OCIBind* timestamp_;
    OCIBind* file_name_;
    OCIBind* line_number_;
    OCIBind* func_;
    OCIBind* logger_;
    OCIBind* level_;
    OCIBind* marker_;
    OCIBind* thread_name_;
};

}

#endif
