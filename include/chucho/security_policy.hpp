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

#if !defined(CHUCHO_SECURITY_POLICY_HPP__)
#define CHUCHO_SECURITY_POLICY_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/exception.hpp>
#include <chucho/optional.hpp>
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <cstdint>

namespace chucho
{

/**
 * @class security_policy security_policy.hpp chucho/security_policy.hpp 
 * The security policy is a means of controlling the external 
 * input to Chucho. External input consists of data that enter 
 * the system from the configuration source. Integers can be 
 * bounded by certain ranges and text can be bounded by its 
 * length. This helps to prevent attacks on the application that 
 * could come from crazy values in the configuration source. 
 *  
 * There is a default security policy, which can easily be 
 * modified or eliminated. The method @ref 
 * configuration::get_security_policy can be used for this 
 * purpose before configuration actually occurs. 
 *  
 * For text bounds, there is a default maximum length of 2,048 
 * bytes. If a text value does not have an explicit length 
 * limit, then the default is used. However, if an integer value 
 * does not have an explicit range limit, then the validation,
 * and thus the configuration, fails. You may set the default to 
 * any value you wish. 
 *  
 * Data members are identified by a textual key. For example, if 
 * you with to restrict a @ref file_writer file name to 10 
 * characters, you would use the following: 
 * @code 
 * chucho::configuration::get_security_policy().set_text("file_writer::file_name", 10); 
 * @endcode 
 * The following is the default security policy. 
 * <table> 
 *     <tr><th>Key</th><th>Value</th></tr>
 *     <tr><td>async_writer::queue_capacity</td>
 *         <td>[10, 32768]</td></tr>
 *     <tr><td>async_writer::queue_capacity(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>async_writer::discard_threshold</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>async_writer::flush_on_destruct</td>
 *         <td>5</td></tr>
 *     <tr><td>email_writer::buffer_size</td>
 *         <td>[1, 65536]</td></tr>
 *     <tr><td>email_writer::buffer_size(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>%email_writer::connection_type</td>
 *         <td>8</td></tr>
 *     <tr><td>email_writer::from</td>
 *         <td>320</td></tr>
 *     <tr><td>email_writer::host</td>
 *         <td>253</td></tr>
 *     <tr><td>email_writer::port</td>
 *         <td>[1, 65535]</td></tr>
 *     <tr><td>email_writer::port(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>email_writer::subject</td>
 *         <td>989</td></tr>
 *     <tr><td>email_writer::to(address)</td>
 *         <td>[1, 320]</td></tr>
 *     <tr><td>email_writer::to(count)</td>
 *         <td>[1, 100]</td></tr>
 *     <tr><td>email_writer::to(text)</td>
 *         <td>32100</td></tr>
 *     <tr><td>email_writer::user</td>
 *         <td>320</td></tr>
 *     <tr><td>email_writer::verbose</td>
 *         <td>5</td></tr>
 *     <tr><td>file_compressor::min_index</td>
 *         <td>[1, 1000]</td></tr>
 *     <tr><td>file_compressor::min_index(text)</td>
 *         <td>4</td></tr>
 *     <tr><td>file_writer::file_name</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>file_writer::flush</td>
 *         <td>5</td></tr>
 *     <tr><td>%file_writer::on_start</td>
 *         <td>8</td></tr>
 *     <tr><td>interval_file_roll_trigger::count</td>
 *         <td>[1, 5000]</td></tr>
 *     <tr><td>interval_file_roll_trigger::period(text)</td>
 *         <td>12</td></tr>
 *     <tr><td>level_filter::level</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>level_filter::on_match</td>
 *         <td>7</td></tr>
 *     <tr><td>level_filter::on_mismatch</td>
 *         <td>7</td></tr>
 *     <tr><td>level_threshold_email_trigger::level</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>level_threshold_filter::level</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>logger::level</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>logger::name</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>%logger::writes_to_ancestors</td>
 *         <td>5</td></tr>
 *     <tr><td>mysql_writer::host</td>
 *         <td>253</td></tr>
 *     <tr><td>mysql_writer::user</td>
 *         <td>16</td></tr>
 *     <tr><td>mysql_writer::password</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>mysql_writer::database</td>
 *         <td>64</td></tr>
 *     <tr><td>mysql_writer::port</td>
 *         <td>[1, 65535]</td></tr>
 *     <tr><td>mysql_writer::port(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>mysql_writer::queue_capacity</td>
 *         <td>[10, 32768]</td></tr>
 *     <tr><td>mysql_writer::queue_capacity(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>mysql_writer::discard_threshold</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>mysql_writer::flush_on_destruct</td>
 *         <td>5</td></tr>
 *     <tr><td>named_pipe_writer::flush</td>
 *         <td>5</td></tr>
 *     <tr><td>named_pipe_writer::name</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>numbered_file_roller::min_index</td>
 *         <td>[-1000, 1000]</td></tr>
 *     <tr><td>numbered_file_roller::min_index(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>numbered_file_roller::max_index</td>
 *         <td>[-1000, 1000]</td></tr>
 *     <tr><td>numbered_file_roller::max_index(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>oracle_writer::user</td>
 *         <td>30</td></tr>
 *     <tr><td>oracle_writer::password</td>
 *         <td>30</td></tr>
 *     <tr><td>oracle_writer::database</td>
 *         <td>512</td></tr>
 *     <tr><td>pattern_formatter::pattern</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>pipe_writer::flush</td>
 *         <td>5</td></tr>
 *     <tr><td>postgres_writer::uri</td>
 *         <td>8000</td></tr>
 *     <tr><td>remote_writer::port</td>
 *         <td>[1, 65535]</td></tr>
 *     <tr><td>remote_writer::port(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>ruby_evaluator_filter::expression</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>size_file_roll_trigger::max_size</td>
 *         <td>[1, 1G]</td></tr>
 *     <tr><td>size_file_roll_trigger::max_size(text)</td>
 *         <td>50</td></tr>
 *     <tr><td>sliding_numbered_file_roller::min_index</td>
 *         <td>[-1000, 1000]</td></tr>
 *     <tr><td>sliding_numbered_file_roller::min_index(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>sliding_numbered_file_roller::max_count</td>
 *         <td>[1, 1000]</td></tr>
 *     <tr><td>sliding_numbered_file_roller::max_count(text)</td>
 *         <td>4</td></tr>
 *     <tr><td>sqlite_writer::file_name</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>syslog_writer::facility</td>
 *         <td>8</td></tr>
 *     <tr><td>syslog_writer::host_name</td>
 *         <td>253</td></tr>
 *     <tr><td>syslog_writer::port</td>
 *         <td>[1, 65535]</td></tr>
 *     <tr><td>syslog_writer::port(text)</td>
 *         <td>5</td></tr>
 *     <tr><td>time_file_roller::file_name_pattern</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>time_file_roller::max_history</td>
 *         <td>[1, 1000]</td></tr>
 *     <tr><td>time_file_roller::max_history(text)</td>
 *         <td>4</td></tr>
 *     <tr><td>zeromq_writer::endpoint</td>
 *         <td><i>default</i></td></tr>
 *     <tr><td>zeromq_writer::prefix</td>
 *         <td><i>default</i></td></tr>
 * </table>
 * 
 * @ingroup miscellaneous
 */
class CHUCHO_EXPORT security_policy
{
public:
    /**
     * @name Constructor 
     * @{ 
     */
    /**
     * Construct a security policy. Using this constructor will
     * accomplish nothing. If you need to query or modify the
     * security policy, you may retrieve the one in use by calling
     * @ref configuration::get_security_policy.
     */
    security_policy();
    /** @}  */

    /**
     * Return the default maximum length of text.
     * 
     * @return the default maximum length of text
     */
    std::size_t get_default_text_max() const;
    /**
     * Return the range of a given integer key.
     * 
     * @param key they key
     * @return the range that has been set, or an empty @ref 
     *         optional if no range has been set
     */
    optional<std::pair<std::intmax_t, std::intmax_t>> get_integer_range(const std::string& key) const;
     /**
      * Return the maximum length of text set for a given key.
      * 
      * @param key the key
      * @return the set length or the result of @ref 
      *         get_default_text_max() if no value has been set for
      *         the key
      */
    std::size_t get_text_max(const std::string& key) const;
    /**
     * Set an integer range. This method clobbers any existing 
     * integer value for the key. The method @ref set_integer will 
     * not set the range if there is already an existing range for 
     * the key. 
     * 
     * @param key the key
     * @param low the minimum value of the range
     * @param high the maximum value of the range
     */
    template <typename val_type>
    void override_integer(const std::string& key, val_type low, val_type high);
    /**
     * Set a text maximum length. This method clobbers any existing 
     * text value for the key. The method @ref set_text will not set
     * the maximum length if there is already an existing maximum 
     * for the key. 
     * 
     * @param key the key
     * @param max_len the maximum value of the length
     */
    void override_text(const std::string& key, std::size_t max_len);
    /**
     * Set the default maximum length of text. If no specific 
     * maximum text length is set for the key, then the default is 
     * used. 
     * 
     * @param dflt the default maximum length
     */
    void set_default_text_max(std::size_t dflt);
    /**
     * Set an integer range. This method will not replace an 
     * existing range for the key. If you wish to unconditionally 
     * set the range, then please use @ref override_integer. 
     * 
     * @param key the key
     * @param low the minimum value of the range
     * @param high the maximum value of the range
     */
    template <typename val_type>
    void set_integer(const std::string& key, val_type low, val_type high);
    /**
     * Set a text maximum length. This method will not replace an 
     * existing maximum length for the key. If you wish to 
     * unconditionally set the maximum length, then please use 
     * @ref override_text. 
     * 
     * @param key the key
     * @param max_len the maximum value of the length
     */
    void set_text(const std::string& key, std::size_t max_len);
    /**
     * Validate a text length value. This method is called from the 
     * @ref memento of the @ref configurable that should validate 
     * the value.
     * 
     * @param key the key
     * @param val the text to validate
     * @throw exception if the value does not conform to the 
     *        security policy
     */
    std::string validate(const std::string& key, const std::string& val) const;
    /**
     * Validate an integer range value. This method is called from 
     * the @ref memento of the @ref configurable that should 
     * validate the value. 
     * 
     * @param key the key
     * @param val the integer to validate
     * @throw exception if the value does not conform to the 
     *        security policy
     */
    template <typename val_type>
    val_type validate(const std::string& key, const val_type& val) const;

private:
    class CHUCHO_NO_EXPORT basic_range
    {
    public:
    	virtual ~basic_range() { };

        virtual std::pair<std::intmax_t, std::intmax_t> get_range() const = 0;
        virtual bool in_range(std::uintmax_t val) const = 0;
        virtual std::string to_text() const = 0;
    };

    template <typename int_type>
    class CHUCHO_NO_EXPORT range : public basic_range
    {
    public:
        range(int_type low, int_type high);

        virtual std::pair<std::intmax_t, std::intmax_t> get_range() const override;
        virtual bool in_range(std::uintmax_t val) const override;
        virtual std::string to_text() const override;

    private:
        int_type low_;
        int_type high_;
    };

    std::map<std::string, std::size_t> text_maxes_;
    std::map<std::string, std::shared_ptr<basic_range>> int_ranges_;
    std::size_t default_text_max_;
};

inline std::size_t security_policy::get_default_text_max() const
{
    return default_text_max_;
}

template <typename val_type>
void security_policy::override_integer(const std::string& key, val_type low, val_type high)
{
    int_ranges_[key] = std::make_shared<range<val_type>>(low, high);
}

inline void security_policy::override_text(const std::string& key, std::size_t max_len)
{
    text_maxes_[key] = max_len;
}

inline void security_policy::set_default_text_max(std::size_t dflt)
{
    default_text_max_ = dflt;
}

template <typename val_type>
void security_policy::set_integer(const std::string& key, val_type low, val_type high)
{
    if (int_ranges_.count(key) == 0)
        override_integer(key, low, high);
}

template <typename val_type>
val_type security_policy::validate(const std::string& key, const val_type& val) const
{
    auto found = int_ranges_.find(key);
    if (found == int_ranges_.end())
        throw exception("The key " + key + " could not be found in the security policy, so its value cannot be tested");
    if (!found->second->in_range(val))
    {
        std::ostringstream stream;
        stream << "The value," << val << ", of key, " << key << ", lies outside the range," <<
            found->second->to_text() << ", permitted by the security policy";
        throw exception(stream.str());
    }
    return val;
}

template <typename int_type>
security_policy::range<int_type>::range(int_type low, int_type high)
    : low_(low),
      high_(high)
{
}

template <typename int_type>
std::pair<std::intmax_t, std::intmax_t> security_policy::range<int_type>::get_range() const
{
    return std::make_pair(static_cast<std::intmax_t>(low_), static_cast<std::intmax_t>(high_));
}

template <typename int_type>
bool security_policy::range<int_type>::in_range(std::uintmax_t val) const
{
    int_type cval = static_cast<int_type>(val);
    return cval >= low_ && cval <= high_;
}

template <typename int_type>
std::string security_policy::range<int_type>::to_text() const
{
    std::ostringstream stream;
    stream << '(' << low_ << ", " << high_ << ')';
    return stream.str();
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
