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
 * @subsection cloudwatch chucho::cloudwatch_writer
 * @subsubsection cloudwatch_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>log_group</td><td>The AWS Cloudwatch log group</td><td>n/a</td></tr>
 * <tr><td>log_stream</td><td>The AWS Cloudwatch log stream</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::cloudwatch_writer</td></tr>
 * <tr><td>region</td><td>The region</td><td>From env or AWS config file</td></tr>
 * <tr><td>batch_size</td><td>The number of events to send at a time</td><td>50</td></tr>
 * <tr><td colspan="2">Any number of objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection cloudwatch_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cloudwatch_writer:
 *         chucho::pattern_formatter
 *             pattern: '%m'
 *         log_group: mygroup
 *         log_stream: mystream
 *         region: us-west-1
 * @endcode
 *
 * @subsection cout chucho::cout_writer
 * @subsubsection cout_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::cout_writer</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection cout_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 * @endcode
 *
 * @subsection database chucho::database_writer
 * @subsubsection database_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>connection</td><td>The database connection</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::database_writer</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection database_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::database_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m'
 *         connection: 'postgresql://host=lotsofdoggies.com port=5432 dbname=chucho user=scrumpy password=dumpy'
 * @endcode
 *
 * @subsection email chucho::email_writer
 * @subsubsection email_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>connection_type</td><td>The type of connection: clear, ssl, or starttls</td><td>n/a</td></tr>
 * <tr><td>from</td><td>The email address of the sender</td><td>n/a</td></tr>
 * <tr><td>host</td><td>The server from which to send</td><td>n/a</td></tr>
 * <tr><td>subject</td><td>The email subject with follows the same rules as @ref chucho::pattern_formatter "pattern_formatter"</td><td>n/a</td></tr>
 * <tr><td>to</td><td>The email addresses of the recipients, comma-separated</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref email_triggers "Email Triggers" group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>buffer_size</td><td>The maximum number of events to send in a single email</td><td>256</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::email_writer</td></tr>
 * <tr><td>password</td><td>The password</td><td>n/a</td></tr>
 * <tr><td>port</td><td>The port to which to connect</td><td>25</td></tr>
 * <tr><td>user</td><td>The user name</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection email_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::email_writer
 *         chucho::pattern_formatter:
 *             pattern: '%m'
 *         chucho::level_threshold_email_trigger:
 *             level: error
 *         host: mail.dummy.com
 *         from: whistler@mctweaky.com
 *         to: one@blubbery.com,two@humid.org
 *         subject: '%c'
 *         connection_type: clear
 *         user: scrumpy
 *         password: lumpy
 * @endcode
 *
 * @subsection file chucho::file_writer
 * @subsubsection file_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>file_name</td><td>The name of the file</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>flush</td><td>Whether to flush the file after every write: true or false</td><td>true</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::file_writer</td></tr>
 * <tr><td>on_start</td><td>Where to start writing: truncate or append</td><td>append</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection file_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::file_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         file_name: hello.log
 * @endcode
 *
 * @subsection loggly chucho::loggly_writer
 * @subsubsection loggly_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>token</td><td>The Loggly authorization token</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::loggly_writer</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection loggly_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::loggly_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m'
 *         token: blah.blah.blah
 * @endcode
 *
 * @section Formatters
 *
 * @section Filters
 *
 * @section Compressors
 *
 * @section email_triggers Email Triggers
 */