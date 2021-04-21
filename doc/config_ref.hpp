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
 * @page config_file Configuration File
 * The configuration file is your primary interface to Chucho.
 *
 * Places to start:
 *   - @ref logger "chucho::logger"
 *   - @ref cout "chucho::cout_writer"
 *   - @ref pattern "chucho::pattern_formatter"
 *   - @ref rolling_file "chucho::rolling_file_writer"
 *   - @ref numbered "chucho::numbered_file_roller"
 *   - @ref size "chucho::size_file_roll_trigger"
 *
 * Chucho supports four different configuration file formats: YAML, JSON, config file, and some basic instances of
 * Log4Cplus. It will automatically determine which format you have on hand. It will also ignore any elements in
 * your configuration file that it does not recognize, allowing you to use one configuration file for your application,
 * even if the file contains elements unimportant to Chucho.
 *
 * The examples below all show instances of a YAML configuration.
 *
 * @section Logger
 * The loggers that contain all other elements.
 *
 * @subsection logger chucho::logger
 * The @c %chucho::logger is the top-level for all Chucho information that will appear in a
 * configuration file. You may include as many @c %chucho::logger instances as are
 * necessary.
 *
 * Refer to @ref chucho::logger "logger" for details.
 *
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
 *
 * Refer to @ref chucho::activemq_writer "activemq_writer" for details.
 *
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
 * <tr><td>coalesce_max</td><td>The number of events to send at once</td><td>25</td></tr>
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
 *
 * Refer to @ref chucho::async_writer "async_writer" for details.
 *
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
 *
 * Refer to @ref chucho::cerr_writer "cerr_writer" for details.
 *
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
 *
 * Refer to @ref chucho::cloudwatch_writer "cloudwatch_writer" for details.
 *
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
 *
 * Refer to @ref chucho::cout_writer "cout_writer" for details.
 *
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
 *
 * Refer to @ref chucho::database_writer "database_writer" for details.
 *
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
 *
 * Refer to @ref chucho::email_writer "email_writer" for details.
 *
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
 *
 * Refer to @ref chucho::file_writer "file_writer" for details.
 *
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
 * @subsection kafka chucho::kafka_writer
 *
 * Refer to @ref chucho::kafka_writer "kafka_writer" for details.
 *
 * @subsubsection kafka_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>topic</td><td>The topic</td><td>n/a</td></tr>
 * <tr><td>brokers</td><td>A comma-separated list of Kafka brokers</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Serializers group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>coalesce_max</td><td>The number of events to send at once</td><td>25</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::kafka_writer</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Compressors group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection kafka_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: will
 *     chucho::kafka_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m'
 *         chucho::formatted_message_serializer
 *         brokers: 192.168.56.101
 *         topic: doggies
 * @endcode
 *
 * @subsection loggly chucho::loggly_writer
 *
 * Refer to @ref chucho::loggly_writer "loggly_writer" for details.
 *
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
 * @subsection named_pipe chucho::named_pipe_writer
 *
 * Refer to @ref chucho::named_pipe_writer "named_pipe_writer" for details.
 *
 * @subsubsection named_pipe_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>pipe_name</td><td>The name of the pipe</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>flush</td><td>Whether to flush the pipe after every write: true or false</td><td>true</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::named_pipe_writer</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection named_pipe_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::named_pipe_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         pipe_name: donglehead
 * @endcode
 *
 * @subsection pipe chucho::pipe_writer
 *
 * Refer to @ref chucho::pipe_writer "pipe_writer" for details.
 *
 * @subsubsection pipe_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>flush</td><td>Whether to flush the pipe after every write: true or false</td><td>true</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::pipe_writer</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection pipe_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::pipe_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 * @endcode
 *
 * @subsection rabbitmq chucho::rabbitmq_writer
 *
 * Refer to @ref chucho::rabbitmq_writer "rabbitmq_writer" for details.
 *
 * @subsubsection rabbitmq_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>exchange</td><td>The exchange to which to write</td><td>n/a</td></tr>
 * <tr><td>url</td><td>The URL of the RabbitMQ instance</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Serializers group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>coalesce_max</td><td>The number of events to send at once</td><td>25</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::rabbitmq_writer</td></tr>
 * <tr><td>routing_key</td><td>The routing key</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Compressors group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection rabbitmq_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rabbitmq_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::formatted_message_serializer
 *         url: 'amqp://user:pass@host:10000/vhost'
 *         exchange: fleas
 * @endcode
 *
 * @subsection rolling_file chucho::rolling_file_writer
 *
 * Refer to @ref chucho::rolling_file_writer "rolling_file_writer" for details.
 *
 * @subsubsection rolling_file_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref rollers "File Rollers" group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>file_name</td><td>The name of the file. If the @ref rollers "roller" does not set the active
 *   file name, then this field is required</td><td>n/a</td></tr>
 * <tr><td>flush</td><td>Whether to flush the file after every write: true or false</td><td>true</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::rolling_file_writer</td></tr>
 * <tr><td>on_start</td><td>Where to start writing: truncate or append</td><td>append</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref triggers "File Roll Triggers" group. If the @ref rollers "roller"
 *   is not also a @ref triggers "trigger", then this field is required.</td><td>n/a</td></tr>
 * </table>
 * @subsubsection rolling_file_example Example
 * @code{.yaml}
 * - chucho::logger:
 *       name: example1
 *       chucho::rolling_file_writer:
 *           file_name: my_stuff.log
 *           chucho::pattern_formatter:
 *               pattern: '%m%n'
 *           chucho::numbered_file_roller:
 *               max_index: 10
 *           chucho::size_file_roll_trigger:
 *               max_size: 10MB
 * - chucho::logger:
 *       name: example2
 *       chucho::rolling_file_writer:
 *           chucho::pattern_formatter:
 *               pattern: '%m%n'
 *           chucho::time_file_roller:
 *               file_name_pattern: '%d{%d}'
 *               max_history: 5
 * @endcode
 *
 * @subsection syslog chucho::syslog_writer
 *
 * Refer to @ref chucho::syslog_writer "syslog_writer" for details.
 *
 * @subsubsection syslog_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>facility</td><td>The syslog facility. Can be one of kern, user, mail, daemon, auth, syslog, lpr,
 *   news, uucp, cron, authpriv, ftp, local0, local1, local2, local3, local4, local5, local6, or local7.</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>host</td><td>The host, which can be omitted if writing to local host</td><td>n/a</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::syslog_writer</td></tr>
 * <tr><td>port</td><td>The port</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection syslog_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::syslog_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         facility: local0
 * @endcode
 *
 * @subsection windows_event_log chucho::windows_event_log_writer
 *
 * Refer to @ref chucho::windows_event_log_writer "windows_event_log_writer" for details.
 *
 * @subsubsection windows_event_log_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>source</td><td>The name of the log source</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>host</td><td>The host, which can be omitted if writing to local host</td><td>n/a</td></tr>
 * <tr><td>log</td><td>The name of the log, which can be omitted if the Registry is prepared outside of Chucho</td><td>n/a</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::windows_event_log_writer</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection windows_event_log_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::windows_event_log_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         source: what
 *         log: hello
 * @endcode
 *
 * @subsection zeromq chucho::zeromq_writer
 *
 * Refer to @ref chucho::zeromq_writer "zeromq_writer" for details.
 *
 * @subsubsection zeromq_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>endpoint</td><td>The endopoint</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Formatters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Serializers group</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>coalesce_max</td><td>The number of events to send at once</td><td>25</td></tr>
 * <tr><td>name</td><td>The name of the writer</td><td>%chucho::zeromq_writer</td></tr>
 * <tr><td>prefix</td><td>A message prefix, which can be used as a queue topic</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any number objects from the @ref Filters group</td><td>n/a</td></tr>
 * <tr><td colspan="2">Any object from the @ref Compressors group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection zeromq_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::zeromq_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m'
 *         chucho::formatted_message_serializer
 *         endpoint: 'tcp://127.0.0.1:7777'
 *         prefix: Hi
 * @endcode
 *
 * @section Formatters
 *
 * @subsection json chucho::json_formatter
 *
 * Refer to @ref chucho::json_formatter "json_formmatter" for details.
 *
 * @subsubsection json_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>excluded</td><td>The comma-separated list of fields to exclude: diagnostic_context, file_name,
 *   function_name, host_name, level, line_number, logger, marker, message, process_id, thread,
 *   timestamp</td><td>n/a</td></tr>
 * <tr><td>included</td><td>The comma-separated list of fields to include: diagnostic_context, file_name,
 *   function_name, host_name, level, line_number, logger, marker, message, process_id, thread,
 *   timestamp. This parameter is ignored if @c excluded is also set.</td><td>all of them</td></tr>
 * <tr><td>style</td><td>The output style: compact or pretty</td><td>compact</td></tr>
 * <tr><td>time_zone</td><td>The time zone: local or utc</td><td>local</td></tr>
 * </table>
 * @subsubsection json_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::json_formatter:
 *             included_fields: message,thread
 * @endcode
 *
 * @subsection pattern chucho::pattern_formatter
 *
 * Refer to @ref chucho::pattern_formatter "pattern_formmatter" for details.
 *
 * @subsubsection pattern_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>pattern</td><td>The pattern for formatting the message. Refer to
 *   @ref chucho::pattern_formatter "pattern_formatter" for details.</td><td>n/a</td></tr>
 * </table>
 * @subsubsection pattern_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 * @endcode
 *
 * @subsection yaml chucho::yaml_formatter
 *
 * Refer to @ref chucho::yaml_formatter "yaml_formmatter" for details.
 *
 * @subsubsection yaml_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>excluded</td><td>The comma-separated list of fields to exclude: diagnostic_context, file_name,
 *   function_name, host_name, level, line_number, logger, marker, message, process_id, thread,
 *   timestamp</td><td>n/a</td></tr>
 * <tr><td>included</td><td>The comma-separated list of fields to include: diagnostic_context, file_name,
 *   function_name, host_name, level, line_number, logger, marker, message, process_id, thread,
 *   timestamp. This parameter is ignored if @c excluded is also set.</td><td>all of them</td></tr>
 * <tr><td>style</td><td>The output style: compact or pretty</td><td>compact</td></tr>
 * <tr><td>time_zone</td><td>The time zone: local or utc</td><td>local</td></tr>
 * </table>
 * @subsubsection yaml_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::yaml_formatter:
 *             included_fields: message,thread
 * @endcode
 *
 * @section rollers File Rollers
 *
 * @subsection numbered chucho::numbered_file_roller
 *
 * Refer to @ref chucho::numbered_file_roller "numbered_file_roller" for details.
 *
 * @subsubsection numbered_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>max_index</td><td>The maximum index of the file name suffix, which must be greater than
 *   or equal to @c min_index</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>min_index</td><td>The minimum index of the file name suffix</td><td>1</td></tr>
 * <tr><td colspan="2">Any object from the @ref file_compressors "File Compressors" group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection numbered_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         file_name: my_stuff.log
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::numbered_file_roller:
 *             max_index: 10
 *         chucho::size_file_roll_trigger:
 *             max_size: 10MB
 * @endcode
 *
 * @subsection sliding_numbered chucho::sliding_numbered_file_roller
 *
 * Refer to @ref chucho::sliding_numbered_file_roller "sliding_numbered_file_roller" for details.
 *
 * @subsubsection sliding_numbered_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>max_count</td><td>The maximum number of files that can exist at a time, which must
 *   be greater than zero</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>min_index</td><td>The minimum index of the file name suffix</td><td>1</td></tr>
 * <tr><td colspan="2">Any object from the @ref file_compressors "File Compressors" group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection sliding_numbered_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         file_name: my_stuff.log
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::sliding_numbered_file_roller:
 *             max_count: 10
 *         chucho::size_file_roll_trigger:
 *             max_size: 10MB
 * @endcode
 *
 * @subsection time chucho::time_file_roller
 *
 * Refer to @ref chucho::time_file_roller "time_file_roller" for details.
 *
 * @subsubsection time_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>file_name_pattern</td><td>The file name that carries a date specification. Refer to
 *   @ref chucho::time_file_roller "time_file_roller" for details.</td><td>n/a</td></tr>
 * <tr><td>max_history</td><td>The maximum number of files to keep</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td colspan="2">Any object from the @ref file_compressors "File Compressors" group</td><td>n/a</td></tr>
 * </table>
 * @subsubsection time_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         file_name: my_stuff.log
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::time_file_roller:
 *             file_name_pattern: '/my/logs/%d{%Y.%m.%d}.log'
 *             max_history: 5
 * @endcode
 *
 * @section triggers File Roll Triggers
 *
 * @subsection interval chucho::interval_file_roll_trigger
 *
 * Refer to @ref chucho::interval_file_roll_trigger "interval_file_roll_trigger" for details.
 *
 * @subsubsection interval_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>every</td><td>Indicator of what the time interval is. Format is <tt>&lt;count&gt; &lt;period&gt;</tt>,
 *   where count is a number and period is one of: minutes, hours, days, weeks or months.</td><td>n/a</td></tr>
 * </table>
 * @subsubsection interval_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         file_name: my_stuff.log
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::sliding_numbered_file_roller:
 *             max_count: 10
 *         chucho::interval_file_roll_trigger:
 *             every: 5 hours
 * @endcode
 *
 * @subsection on_start chucho::on_start_file_roll_trigger
 *
 * Takes no parameters. Refer to @ref chucho::on_start_file_roll_trigger "on_start_file_roll_trigger" for details.
 *
 * @subsection size chucho::size_file_roll_trigger
 *
 * Refer to @ref chucho::size_file_roll_trigger "size_file_roll_trigger" for details.
 *
 * @subsubsection size_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>max_size</td><td>The maximum size of a log file. This can be expressed with a case-insensitive suffix
 *   of k, m or g (with an optional b) to indicate kilobytes, megabytes or gigabytes.</td><td>n/a</td></tr>
 * </table>
 * @subsubsection size_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         file_name: my_stuff.log
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::numbered_file_roller:
 *             max_index: 10
 *         chucho::size_file_roll_trigger:
 *             max_size: 10MB
 * @endcode
 *
 * @subsection time_trigger chucho::time_file_roller
 *
 * The @ref time "chucho::time_file_roller" is also a trigger.
 *
 * @section Filters
 *
 * @subsection cache_and_release chucho::cache_and_release_filter
 *
 * Refer to @ref chucho::cache_and_release_filter "cache_and_release_filter" for details.
 *
 * @subsubsection cache_and_release_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>cache_threshold</td><td>The highest level that will go into the cache: trace, debug, info, warn, error or fatal</td><td>n/a</td></tr>
 * <tr><td>release_threshold</td><td>The level at which to release the cache: trace, debug, info, warn, error or fatal</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>chunk_size</td><td>The byte size of each chunk in the cache</td><td>1MB</td></tr>
 * <tr><td>max_chunks</td><td>The number of chunks to keep in the cache</td><td>2</td></tr>
 * <tr><td>name</td><td>The name of the filter</td><td>%chucho::cache_and_release_filter</td></tr>
 * </table>
 * @subsubsection cache_and_release_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::cache_and_release_filter:
 *             cache_threshold: debug
 *             release_threshold: error
 * @endcode
 *
 * @subsection duplicate chucho::duplicate_message_filter
 *
 * Refer to @ref chucho::duplicate_message_filter "duplicate_message_filter" for details.
 *
 * @subsubsection duplicate_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the filter</td><td>%chucho::duplicate_message_filter</td></tr>
 * </table>
 * @subsubsection duplicate_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::duplicate_message_filter
 * @endcode
 *
 * @subsection level chucho::level_filter
 *
 * Refer to @ref chucho::level_filter "level_filter" for details.
 *
 * @subsubsection level_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>level</td><td>The level of interest: trace, debug, info, warn, error or fatal</td><td>n/a</td></tr>
 * <tr><td>on_match</td><td>What to do if the level matches: deny, neutral or accept</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the filter</td><td>%chucho::level_filter</td></tr>
 * <tr><td>on_mismatch</td><td>What to do if the level doesn't match: deny, neutral or accept</td><td>n/a</td></tr>
 * </table>
 * @subsubsection level_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::level_filter:
 *             level: info
 *             on_match: deny
 * @endcode
 *
 * @subsection level_threshold chucho::level_threshold_filter
 *
 * Refer to @ref chucho::level_threshold_filter "level_threshold_filter" for details.
 *
 * @subsubsection level_threshold_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>level</td><td>The level of interest: trace, debug, info, warn, error or fatal</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the filter</td><td>%chucho::level_threshold_filter</td></tr>
 * </table>
 * @subsubsection level_threshold_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::level_threshold_filter:
 *             level: warn
 * @endcode
 *
 * @subsection ruby_evaluator chucho::ruby_evaluator_filter
 *
 * Refer to @ref chucho::ruby_evaluator_filter "ruby_evaluator_filter" for details.
 *
 * @subsubsection ruby_evaluator_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>expression</td><td>A Ruby expression that will evaluate the log event. Please refer to
 *   @ref chucho::ruby_evaluator_filter "ruby_evaluator_filter" to see what global variables are
 *   available to the expression that correspond to fields in the log event.</td><td>n/a</td></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>name</td><td>The name of the filter</td><td>%chucho::ruby_evaluator_filter</td></tr>
 * </table>
 * @subsubsection ruby_evaluator_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::cout_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::ruby_evaluator_filter:
 *             expression: '$logger == "example"'
 * @endcode
 *
 * @section file_compressors File Compressors
 *
 * @subsection bzip2f chucho::bzip2_file_compressor
 *
 * Refer to @ref chucho::bzip2_file_compressor "bzip2_file_compressor" for details.
 *
 * @subsubsection bzip2f_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>min_index</td><td>The minimum index at which to start compressing rolled files</td><td>n/a</td></tr>
 * </table>
 * @subsubsection bzip2f_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::numbered_file_roller:
 *             max_index: 5
 *             chucho::bzip2_file_compressor:
 *                 min_index: 1
 *         chucho::size_file_roll_trigger:
 *             max_size: 5MB
 *         file_name: what.log
 * @endcode
 *
 * @subsection gzipf chucho::gzip_file_compressor
 *
 * Refer to @ref chucho::gzip_file_compressor "gzip_file_compressor" for details.
 *
 * @subsubsection gzipf_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>min_index</td><td>The minimum index at which to start compressing rolled files</td><td>n/a</td></tr>
 * </table>
 * @subsubsection gzipf_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::numbered_file_roller:
 *             max_index: 5
 *             chucho::gzip_file_compressor:
 *                 min_index: 1
 *         chucho::size_file_roll_trigger:
 *             max_size: 5MB
 *         file_name: what.log
 * @endcode
 *
 * @subsection lzmaf chucho::lzma_file_compressor
 *
 * Refer to @ref chucho::lzma_file_compressor "lzma_file_compressor" for details.
 *
 * @subsubsection lzmaf_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>min_index</td><td>The minimum index at which to start compressing rolled files</td><td>n/a</td></tr>
 * </table>
 * @subsubsection lzmaf_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::numbered_file_roller:
 *             max_index: 5
 *             chucho::lzma_file_compressor:
 *                 min_index: 1
 *         chucho::size_file_roll_trigger:
 *             max_size: 5MB
 *         file_name: what.log
 * @endcode
 *
 * @subsection zipf chucho::zip_file_compressor
 *
 * Refer to @ref chucho::zip_file_compressor "zip_file_compressor" for details.
 *
 * @subsubsection zipf_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>min_index</td><td>The minimum index at which to start compressing rolled files</td><td>n/a</td></tr>
 * </table>
 * @subsubsection zipf_example Example
 * @code{.yaml}
 * chucho::logger:
 *     name: example
 *     chucho::rolling_file_writer:
 *         chucho::pattern_formatter:
 *             pattern: '%m%n'
 *         chucho::numbered_file_roller:
 *             max_index: 5
 *             chucho::zip_file_compressor:
 *                 min_index: 1
 *         chucho::size_file_roll_trigger:
 *             max_size: 5MB
 *         file_name: what.log
 * @endcode
 *
 * @section Compressors
 *
 * @subsection bzip2 chucho::bzip2_compressor
 *
 * Refer to @ref chucho::bzip2_compressor "bzip2_compressor" for details.
 *
 * @subsubsection bzip2_params Parameters
 *
 * Takes no parameters.
 *
 * @subsubsection bzip2_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::formatted_message_serializer
           chucho::bzip2_compressor
 * @endcode
 *
 * @subsection lz4 chucho::lz4_compressor
 *
 * Refer to @ref chucho::lz4_compressor "lz4_compressor" for details.
 *
 * @subsubsection lz4_params Parameters
 *
 * Takes no parameters.
 *
 * @subsubsection lz4_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::formatted_message_serializer
           chucho::lz4_compressor
 * @endcode
 *
 * @subsection lzma chucho::lzma_compressor
 *
 * Refer to @ref chucho::lzma_compressor "lzma_compressor" for details.
 *
 * @subsubsection lzma_params Parameters
 *
 * Takes no parameters.
 *
 * @subsubsection lzma_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::formatted_message_serializer
           chucho::lzma_compressor
 * @endcode
 *
 * @subsection noop chucho::noop_compressor
 *
 * Refer to @ref chucho::noop_compressor "noop_compressor" for details.
 *
 * @subsubsection noop_params Parameters
 *
 * Takes no parameters.
 *
 * @subsubsection noop_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::formatted_message_serializer
           chucho::noop_compressor
 * @endcode
 *
 * @subsection zlib chucho::zlib_compressor
 *
 * Refer to @ref chucho::zlib_compressor "zlib_compressor" for details.
 *
 * @subsubsection zlib_params Parameters
 *
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Optional Parameters</b></td></tr>
 * <tr><td>compression_level</td><td>The level of compression, which must be in the range [0, 9].</td><td>6</td></tr>
 * </table>
 *
 * @subsubsection zlib_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::formatted_message_serializer
           chucho::zlib_compressor
 * @endcode
 *
 * @section Serializers
 *
 * @subsection capn chucho::capn_proto_serializer
 *
 * Refer to @ref chucho::capn_proto_serializer "capn_proto_serializer" for details.
 *
 * @subsubsection capn_params Parameters
 *
 * Takes no parameters.
 *
 * @subsubsection capn_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::capn_proto_serializer
 * @endcode
 *
 * @subsection flatbuffers chucho::flatbuffers_serializer
 *
 * Refer to @ref chucho::flatbuffers_serializer "flatbuffers_serializer" for details.
 *
 * @subsubsection flatbuffers_params Parameters
 *
 * Takes no parameters.
 *
 * @subsubsection flatbuffers_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::flatbuffers_serializer
 * @endcode
 *
 * @subsection formatted_message chucho::formatted_message_serializer
 *
 * Refer to @ref chucho::formatted_message_serializer "formatted_message_serializer" for details.
 *
 * @subsubsection formatted_message_params Parameters
 *
 * Takes no parameters.
 *
 * @subsubsection formatted_message_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::formatted_message_serializer
 * @endcode
 *
 * @subsection protobuf chucho::protobuf_serializer
 *
 * Refer to @ref chucho::protobuf_serializer "protobuf_serializer" for details.
 *
 * @subsubsection protobuf_params Parameters
 *
 * Takes no parameters.
 *
 * @subsubsection protobuf_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::zeromq_writer:
           endpoint: 'tcp://127.0.0.1:7776'
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::protobuf_serializer
 * @endcode
 *
 * @section email_triggers Email Triggers
 *
 * @subsection level_threshold_email chucho::level_threshold_email_trigger
 *
 * Refer to @ref chucho::level_threshold_email_trigger "level_threshold_email_trigger" for details.
 *
 * @subsubsection level_threshold_email_params Parameters
 * <table>
 * <tr><th>Name</th><th>Description</th><th>Default</th></tr>
 * <tr><td colspan="3"><b>Required Parameters</b></td></tr>
 * <tr><td>level</td><td>The level at which email will be sent: trace, debug, info, warn, error or fatal</td><td>n/a</td></tr>
 * </table>
 * @subsubsection level_threshold_email_example Example
 * @code{.yaml}
 * chucho::logger:
       name: example
       chucho::email_writer:
           chucho::pattern_formatter:
               pattern: '%m'
           chucho::level_threshold_email_trigger:
               level: error
           host: mail.dummy.com
           port: 123
           from: whistler@mctweaky.com
           to: one@blubbery.com,two@humid.org
           subject: '%c'
           connection_type: clear
           user: scrumpy
           password: lumpy
           buffer_size: 7000
 * @endcode
 */