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

/*
 * This file just provides the configuration reference page documentation of Doxygen.
 * It's not actually ever included by anyone except Doxygen.
 */

/**
 * @page Configuration Configuration
 * This is the configuration reference.
 *
 * @section Logger
 * The loggers that contain all other elements.
 *
 * @subsection logger chucho::logger
 * The @c %chucho::logger is the top-level for all Chucho information that will appear in a
 * configuration file. You may include as many @c %chucho::logger instances as are
 * necessary.
 * @subsubsection logger_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the logger, which may be @c &lt;root&gt; for the unnamed root logger</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any number of objects from the @ref Writers group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>level</td><td>The level: trace, debug, info, warn, error, fatal or off</td><td>info</td></tr>
 * <tr><td>writes_to_ancestors</td><td>Whether the logger forwards events to its parent: true or false</td><td>true</td></tr>
 * </table>
 * @subsubsection logger_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 * @endcode
 *
 * @section Writers
 *
 * @subsection activemq chucho::activemq_writer
 * @subsubsection activemq_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>broker</td><td>The ActiveMQ broker to which to connect</td><td>n/a</td></tr>
 * <tr><td>consumer_type</td><td>Either topic or queue</td><td>n/a</td></tr>
 * <tr><td>topic_or_queue</td><td>The name of the topic or queue</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Serializers group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::activemq_writer</td></tr>
 * <tr><td colspan="2">Any object from the @ref Compressors group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any number of objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection activemq_example Example
 * @code{.yaml}
 * - chucho::logger:
 *     - name: example
 *     - chucho::activemq_writer:
 *         - chucho::pattern_formatter:
 *             - pattern: '%m'
 *         - chucho::formatted_message_serializer
 *         - broker: 'tcp://127.0.0.1:61616'
 *         - consumer_type: queue
 *         - topic_or_queue: stuff
 * @endcode
 *
 * @subsection async chucho::async_writer
 * @subsubsection async_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td colspan="2">Any object from the @ref Writers group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>chunk_size</td><td>The size of the chunks in the event cache</td><td>1MB</td></tr>
 * <tr><td>flush_on_destruct</td><td>Whether the event cache should be flushed: true or false</td><td>true</td></tr>
 * <tr><td>max_chunks</td><td>The maximum number of chunks in the event cache</td><td>2</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::async_writer</td></tr>
 * </table>
 * @subsubsection async_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::async_writer:
 *         chucho::file_writer:
 *             chucho::pattern_formatter:
 *                 pattern: '%m%n'
 *             file_name: hello.log
 * @endcode
 *
 * @subsection cerr chucho::cerr_writer
 * @subsubsection cerr_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::cerr_writer</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection cerr_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cerr_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 * @endcode
 *
 * @section Formatters
 *
 * @section Filters
 *
 * @section Compressors
 */