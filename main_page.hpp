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

/*
 * This file just provides the main page documentation of Doxygen. 
 * It's not actually ever included by anyone except Doxygen.
 */

/**
 * @mainpage API
 *  
 * @image html itzcuintli.png "Chucho"
 *  
 * @section Chucho 
 *  
 * @subsection Welcome 
 * Chucho is a logging framework based on the popular Java 
 * library, <a href="http://logback.qos.ch/" 
 * target="_blank">Logback</a>. Chucho makes it easy to add 
 * logging to an application with, aside from the log statements 
 * themselves, very little extra code having to be written. In 
 * most cases you would just need to set the name of the Chucho 
 * configuration file, and you would be ready to go. 
 *  
 * This is the documentation of the Chucho API. For access to 
 * releases, documentation, tickets and source code, please 
 * visit the <a href="https://www.assembla.com/spaces/chucho" 
 * target="_blank">project 
 * site</a>.
 *  
 * Chucho uses the same architecture as <a 
 * href="http://logback.qos.ch/" 
 * target="_blank">Logback</a>, which involes loggers, writers, 
 * formatters and other elements. For more information on the 
 * design and use of Chucho, please visit the <a 
 * href="https://www.assembla.com/spaces/chucho/wiki" 
 * target="_blank">user manual</a>, which is under construction.
 *  
 * @subsection c C API
 * Though the core of Chucho is written in C++11, a wrapping C 
 * API is also exposed. This allows Chucho to be used in C 
 * programs and in C++ programs that may be built with a 
 * compiler that cannot build Chucho, such as a non-C++11 
 * compiler. 
 *  
 * The C API is expressed in header files with the extension @c 
 * .h, while the core Chucho C++11 API is in files with the 
 * extension @c .hpp. 
 *  
 * Some documentation for the C API is copied directly from the 
 * C++ documentation, so you may find reference to C++ classes 
 * in the C documentation. All the referenced functionality can 
 * also be found in the C API, usually with a same or similar 
 * name. 
 *
 * @subsection Contact
 * If you need answers to questions or help in any way, please 
 * join our <a 
 * href="https://groups.google.com/forum/?fromgroups#!forum/chucho-logging" 
 * target="_blank">Google group</a>. 
 *  
 * @copyright &copy; 2013-2014 Will Mason 
 */

/**
 * @defgroup compressors 
 * Classes that can compress rolling files. 
 */

/**
 * @defgroup configuration 
 * Classes that involve the configuration of chucho. 
 */

/**
 * @defgroup filters 
 * Filters are attached to @ref writers, and they can be used to 
 * refine log filtering beyond the basic level check. 
 */

/**
 * @defgroup formatters 
 * Formatters turn a log event into a piece of text so that a
 * writer can write the event. 
 */

/**
 * @defgroup levels 
 * Levels facilitate basic log event filtration. 
 */

/**
 * @defgroup loggers
 * The logger class and macros for writing filtered log events. 
 */

/**
 * @defgroup miscellaneous
 * Stuff that has no relatives.
 */

/**
 * @defgroup rolling Rolling Files
 * Classes that roll files, write to rolled files, and determine 
 * when files should roll. 
 */

/**
 * @defgroup status 
 * Classes that facilitate the status reporting system of 
 * chucho. 
 */

/**
 * @defgroup syslog 
 * Classes and enumerations related to writing to syslog. 
 */

/**
 * @defgroup writers 
 * Writers write log events to destinations. 
 */

/**
 * @defgroup c 
 * Files that comprise the Chucho C API.
 */

/**
 * @defgroup c_compressors Compressors 
 * Functions for compressing rolling files.
 *  
 * @ingroup c 
 */

/**
 * @defgroup c_configuration Configuration 
 * Functions that help manage the configuration of Chucho.
 *  
 * @ingroup c 
 */

/**
 * @defgroup c_filters Filters
 * Filters are attached to @ref chucho_writer, and they can be 
 * used to refine log filtering beyond the basic level check. 
 *
 * @ingroup c 
 */

/**
 * @defgroup c_formatters Formatters 
 * @copydoc formatters 
 *  
 * @ingroup c 
 */

/**
 * @defgroup c_levels Levels 
 * @copydoc levels 
 *  
 * @ingroup c 
 */

/**
 * @defgroup c_loggers Loggers 
 * Types, functions and macros for writing filtered log events. 
 *  
 * @ingroup c
 */

/**
 * @defgroup c_miscellaneous Miscellaneous 
 * @copydoc miscellaneous 
 *  
 * @ingroup c 
 */

/**
 * @defgroup c_rolling Rolling Files 
 * Types and functions that help with rolling files, writing 
 * rolling files and determining when files should roll. 
 *  
 * @ingroup c 
 */

/**
 * @defgroup c_status Status 
 * @copydoc status 
 *  
 * @ingroup c 
 */

/**
 * @defgroup c_syslog Syslog 
 * Functions and enumerations for writing to syslog. 
 *  
 * @ingroup c 
 */

/**
 * @defgroup c_writers Writers 
 * @copydoc writers 
 *  
 * @ingroup c 
 */
