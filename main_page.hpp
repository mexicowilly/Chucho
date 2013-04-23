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

/*
 * This file just provides the main page documentation of Doxygen. 
 * It's not actually ever included by anyone except Doxygen.
 */

/**
 * @mainpage API
 *  
 * @image html itzcuintli.png "Chucho"
 *  
 * @section Welcome 
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
 * @copyright &copy; 2013 Will Mason 
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
 * @defgroup writers 
 * Writers write log events to destinations. 
 */

