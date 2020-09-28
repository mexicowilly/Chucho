/*
 * Copyright 2013-2020 Will Mason
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

#include <chucho/ruby_evaluator_filter.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/logger.hpp>
#include <sstream>
#include <mutex>
#if defined(CHUCHO_RUBY_FRAMEWORK)
#include <Ruby/ruby.h>
#else
#include <ruby.h>
#endif

namespace
{

struct static_data
{
    static_data();

    std::mutex guard_;
};

static_data::static_data()
{
    ruby_init();
    chucho::garbage_cleaner::get().add([this] () { delete this; ruby_finalize(); });
}

static_data& data()
{
    static std::once_flag once;
    // This is cleaned in finalize
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

VALUE report_exception(VALUE rprt, VALUE except)
{
    VALUE msg = rb_funcall(except, rb_intern("inspect"), 0);
    auto func = *reinterpret_cast<std::function<void(const std::string&)>*>(rprt);
    func(std::string("Ruby error: ") + StringValueCStr(msg));
    return Qfalse;
}

}

namespace chucho
{

ruby_evaluator_filter::ruby_evaluator_filter(const std::string& name,
                                             const std::string& expression)
    : evaluator_filter(name),
      expression_(expression)
{
}

filter::result ruby_evaluator_filter::evaluate(const event& evt)
{
    std::lock_guard<std::mutex> lg(data().guard_);
    VALUE log_name = rb_str_new2(evt.get_logger()->get_name().c_str());
    rb_define_variable("logger", &log_name);
    VALUE level_name = rb_str_new2(evt.get_level()->get_name());
    rb_define_variable("level", &level_name);
    VALUE msg = rb_str_new2(evt.get_message().c_str());
    rb_define_variable("message", &msg);
    VALUE arg = INT2NUM(event::clock_type::to_time_t(evt.get_time()));
    VALUE timestamp = rb_funcall(rb_path2class("Time"), rb_intern("at"), 1, arg);
    rb_define_variable("timestamp", &timestamp);
    VALUE file_name = rb_str_new2(evt.get_file_name());
    rb_define_variable("file_name", &file_name);
    VALUE line_number = INT2NUM(evt.get_line_number());
    rb_define_variable("line_number", &line_number);
    VALUE func_name = rb_str_new2(evt.get_function_name());
    rb_define_variable("function_name", &func_name);
    VALUE mark;
    if (evt.get_marker())
    {
        std::ostringstream stream; 
        stream << *evt.get_marker();
        mark = rb_str_new2(stream.str().c_str());
    }
    else
    {
        mark = Qnil;
    }
    rb_define_variable("marker", &mark);
    std::function<void(std::string&, std::exception_ptr)> rprt = std::bind(&ruby_evaluator_filter::report_error,
                                                                           this,
                                                                           std::placeholders::_1,
                                                                           std::exception_ptr());
    VALUE res = rb_rescue(RUBY_METHOD_FUNC(rb_eval_string),
                          reinterpret_cast<VALUE>(expression_.c_str()),
                          RUBY_METHOD_FUNC(report_exception),
                          reinterpret_cast<VALUE>(&rprt));
    return res == Qtrue ? result::NEUTRAL : result::DENY;
}

}
