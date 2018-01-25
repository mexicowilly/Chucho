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

#if !defined(CHUCHO_PATTERN_FORMATTER_HPP__)
#define CHUCHO_PATTERN_FORMATTER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/formatter.hpp>
#include <vector>
#include <ctime>

namespace chucho
{

namespace calendar
{

struct pieces;

}

/**
 * @class pattern_formatter pattern_formatter.hpp chucho/pattern_formatter.hpp 
 * A pattern_formatter accepts a pattern in its constructor that 
 * is then used to format events. The patterns used by 
 * pattern_formatter are based on the format patterns of the 
 * printf() family of functions in the standard C library. Just 
 * as with printf(), a pattern may be composed of oridnary 
 * characters and conversion specifications. Unlike the printf() 
 * functions, pattern_formatter does not take immediate input in 
 * order to convert a conversion specification to text, but 
 * rather takes information from the @ref event or from the host 
 * operating system. 
 *  
 * Just as with printf(), the conversion specifications of 
 * pattern_formatter accept modifiers that determine things like 
 * justification and field width. 
 *  
 * A conversion specification always starts with the percent 
 * sign ('%%'). This character may be followed by: 
 * - An optional minus sign ('-'). If this field is provided, 
 *   then justification of the text will be to the left.
 *   Otherwise, it will be to the right.
 * - An optional decimal digit string specifying a minimum field 
 *   width. If the converted value has fewer characters than the
 *   minimum field width, then it will be padded on the left or
 *   the right, depending on whether the aforementioned minus
 *   sign ('-') field has been used.
 * - An optional dot ('.') followed by a decimal digit string 
 *   specifying the maximum field width. If the converted value
 *   is longer than the maximum width, then characters are
 *   trimmed from the beginning of the value, leaving maximum
 *   width trailing characters.
 * - A character that specifies the type of conversion that will 
 *   be performed. This must be one of the following.
 * <table>
 *     <tr><th>Character</th><th>Conversion Value</th></tr>
 *     <tr><td>b</td>
 *         <td>The base file name from which the log
 *      @ref event originated. The base file name does not
 *      include any path prefix from the name of the
 *      file.</td></tr>
 *     <tr><td>c</td>
 *         <td>The name of the @ref logger that is writing this
 *         event.</td></tr>
 *     <tr><td>C{&lt;key&gt;}</td>
 *         <td>The key from this thread's @ref
 *         diagnostic_context is looked up, and its value is
 *         placed into the log message. The key
 *         argument is required.</td></tr>
 *     <tr><td>d{&lt;spec&gt;}</td>
 *         <td>The time format specification is used to format
 *         the UTC time of the @ref event. The specification
 *         must be one that is compatible with the function
 *         <a
 *         href="http://en.cppreference.com/w/cpp/io/manip/put_time">std::put_time()</a>,
 *         which is very similar to the specifications used by
 *         the C function strftime(). A key difference is that
 *         the tokens "%q" and "%Q" may be used to insert the number of
 *         milliseconds or microseconds, respectively. This is added due to the fact that <a
 *         href="http://en.cppreference.com/w/cpp/io/manip/put_time">std::put_time()</a>
 *         does not support milliseconds or microseconds.
 *  
 *         If the spec argument is omitted, then a
 *         default of "%Y-%m-%d %H:%M:%S" will be
 *         used.</td></tr>
 *     <tr><td>D{&lt;spec&gt;}</td>
 *         <td>The time format specification is used to format
 *         the local time of the @ref event. The specification
 *         must be one that is compatible with the function
 *         <a
 *         href="http://en.cppreference.com/w/cpp/io/manip/put_time">std::put_time()</a>,
 *         which is very similar to the specifications used by
 *         the C function strftime(). A key difference is that
 *         the tokens "%q" and "%Q" may be used to insert the number of
 *         milliseconds or microseconds, respectively. This is added due to the fact that <a
 *         href="http://en.cppreference.com/w/cpp/io/manip/put_time">std::put_time()</a>
 *         does not support milliseconds or microseconds.
 *
 *         If the spec argument is omitted, then a
 *         default of "%Y-%m-%d %H:%M:%S" will be
 *         used.</td></tr>
 *     <tr><td>F</td>
 *         <td>The fully qualified name of the file from which
 *         the log @ref event originated.</td></tr>
 *     <tr><td>h</td>
 *         <td>The base name of the host.</td></tr>
 *     <tr><td>H</td>
 *         <td>The fully qualified domain name of the
 *         host.</td></tr>
 *     <tr><td>i</td>
 *         <td>The process identifier (pid) of the current
 *         process.</td></tr>
 *     <tr><td>k</td>
 *         <td>The @ref marker, if any, that the log @ref event
 *         carries.</td></tr>
 *     <tr><td>L</td>
 *         <td>The line number from which the @ref event
 *         originated.</td></tr>
 *     <tr><td>m</td>
 *         <td>The message associated with the log @ref
 *         event.</td></tr>
 *     <tr><td>M</td>
 *         <td>The name of the function from which the log
 *         @ref event originated.</td></tr>
 *     <tr><td>n</td>
 *         <td>A platform-appropriate end-of-line.</td></tr>
 *     <tr><td>p</td>
 *         <td>The log @ref level of the @ref event.</td></tr>
 *     <tr><td>r</td>
 *         <td>The number of milliseconds that have elapsed
 *         since the program started running.</td></tr>
 *     <tr><td>t</td>
 *         <td>The thread from which the @ref event
 *         originated. This is the same value returned
 *         by std::this_thread::get_id().</td></tr>
 *     <tr><td>%</td>
 *         <td>A literal percent sign ('%%') is placed in the
 *         message. Justification and width modifiers
 *         may not be used with this token.</td></tr>
 * </table>
 *  
 * @ingroup formatters 
 */
class CHUCHO_EXPORT pattern_formatter : public formatter
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a formatter from the pattern specification.
     * 
     * @param pattern the pattern to use for formatting
     * @throw exception if the pattern is invalid
     */
    pattern_formatter(const std::string& pattern);
    //@}

    virtual std::string format(const event& evt) override;

private:
    enum class justification
    {
        LEFT,
        RIGHT,
        UNSET
    };

    struct CHUCHO_NO_EXPORT format_params
    {
        std::size_t min_width_;
        std::size_t max_width_;
        justification just_;

        format_params();
        void reset();
    };
    
    class CHUCHO_NO_EXPORT piece
    {
    public:
        piece(const format_params& params);

        std::string get_text(const event& evt) const;

    protected:
        virtual std::string get_text_impl(const event& evt) const = 0;

    private:
        format_params params_;
    };

    class CHUCHO_NO_EXPORT base_file_piece : public piece
    {
    public:
        base_file_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT full_file_piece : public piece
    {
    public:
        full_file_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT logger_piece : public piece
    {
    public:
        logger_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT date_time_piece : public piece
    {
    public:
        date_time_piece(const std::string& date_pattern,
                        const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
        virtual void to_calendar(time_t t, calendar::pieces& cal) const = 0;

    private:
        enum class frac_type
        {
            MILLI,
            MICRO
        };

        std::string date_pattern_;
        std::vector<std::tuple<frac_type, std::size_t>> frac_positions_;
    };

    class CHUCHO_NO_EXPORT utc_date_time_piece : public date_time_piece
    {
    public:
        utc_date_time_piece(const std::string& date_pattern,
                            const format_params& params);

    protected:
        virtual void to_calendar(time_t t, calendar::pieces& cal) const override;
    };

    class CHUCHO_NO_EXPORT local_date_time_piece : public date_time_piece
    {
    public:
        local_date_time_piece(const std::string& date_pattern,
                              const format_params& params);

    protected:
        virtual void to_calendar(time_t t, calendar::pieces& cal) const override;
    };

    class CHUCHO_NO_EXPORT base_host_piece : public piece
    {
    public:
        base_host_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT full_host_piece : public piece
    {
    public:
        full_host_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT line_number_piece : public piece
    {
    public:
        line_number_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT message_piece : public piece
    {
    public:
        message_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT function_piece : public piece
    {
    public:
        function_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT end_of_line_piece : public piece
    {
    public:
        end_of_line_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT level_piece : public piece
    {
    public:
        level_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT milliseconds_since_start_piece : public piece
    {
    public:
        milliseconds_since_start_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT pid_piece : public piece
    {
    public:
        pid_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };
    
    class CHUCHO_NO_EXPORT literal_piece : public piece
    {
    public:
        literal_piece(const std::string& text);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;

    private:
        std::string text_;
    };

    class CHUCHO_NO_EXPORT thread_piece : public piece
    {
    public:
        thread_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT marker_piece : public piece
    {
    public:
        marker_piece(const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT diagnostic_context_piece : public piece
    {
    public:
        diagnostic_context_piece(const std::string& key,
                                 const format_params& params);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;

    private:
        std::string key_;
    };

    CHUCHO_NO_EXPORT std::unique_ptr<piece> create_piece(std::string::const_iterator& pos,
                                                         std::string::const_iterator end,
                                                         const format_params& params);
    CHUCHO_NO_EXPORT void parse(const std::string& pattern);
    CHUCHO_NO_EXPORT std::string get_argument(std::string::const_iterator& pos,
                                              std::string::const_iterator end);

    std::vector<std::unique_ptr<piece>> pieces_;
};

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
