#if !defined(CHUCHO_ROLLING_FILE_WRITER_HPP__)
#define CHUCHO_ROLLING_FILE_WRITER_HPP__

#include <chucho/file_writer.hpp>
#include <chucho/file_roller.hpp>
#include <chucho/file_roll_trigger.hpp>

namespace chucho
{

/**
 * @defgroup rolling Rolling Files
 * Classes that roll files, write to rolled files, and determine 
 * when files should roll. 
 */

/**
 * @class rolling_file_writer rolling_file_writer.hpp chucho/rolling_file_writer.hpp 
 * A @ref writer that writes to files and rolls them. A 
 * rolling_file_writer does its work with the help of a couple 
 * of friends: the @ref file_roll_trigger and the @ref 
 * file_roller. A @ref file_roll_trigger is queried every time 
 * an @ref event is written to find out whether it is time to 
 * roll the file. The trigger answers the question, when? 
 *  
 * If the trigger indicates that the time for rolling has come, 
 * then the @ref file_roller is told to perform a roll. The 
 * roller answers the question, how? 
 *  
 * Additionally, the @ref file_roller may also be a @ref 
 * file_roll_trigger, as is the case of @ref time_file_roller. 
 * Therefore, if the @ref file_roller is also a @ref 
 * file_roll_trigger, then it is not necessary also to construct 
 * the rolling_file_writer with a separate @ref 
 * file_roll_trigger. 
 *  
 * The rolling_file_writer may be constructed either with or 
 * without an active file name. Without an active file name, the 
 * active name becomes the current name becomes the result of 
 * calling the @ref file_roller::get_active_file_name() method. 
 * If the rolling_file_writer is constructed with its own active 
 * file name, then that file is always the one that is currently 
 * being written, and the rolled files get their names from the 
 * @ref file_roller. 
 *  
 * @ingroup writers 
 * @ingroup rolling
 */
class CHUCHO_EXPORT rolling_file_writer : public file_writer
{
public:
    /**
     * @name Constructors
     */
    //@{
    /**
     * Construct a rolling_file_writer.
     * 
     * @param fmt the formatter
     * @param roller the roller
     * @param trigger the optional trigger
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw std::invalid_argument if the roller is an 
     *        uninitialized std::shared_ptr
     * @throw std::invalid_argument if the trigger cannot be 
     *        resolved
     */
    rolling_file_writer(std::shared_ptr<formatter> fmt,
                        std::shared_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());
    /**
     * Construct a rolling_file_writer.
     * 
     * @param fmt the formatter
     * @param file_name the active file name
     * @param roller the roller
     * @param trigger the optional trigger
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw std::invalid_argument if the roller is an 
     *        uninitialized std::shared_ptr
     * @throw std::invalid_argument if the trigger cannot be 
     *        resolved
     */
    rolling_file_writer(std::shared_ptr<formatter> fmt,
                        const std::string& file_name,
                        std::shared_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());
    /**
     * Construct a rolling_file_writer.
     * 
     * @param fmt the formatter
     * @param file_name the active file name
     * @param start action to take on start
     * @param flush whether to flush the stream after every write
     * @param roller the roller
     * @param trigger the optional trigger
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw std::invalid_argument if the roller is an 
     *        uninitialized std::shared_ptr
     * @throw std::invalid_argument if the trigger cannot be 
     *        resolved
     */
    rolling_file_writer(std::shared_ptr<formatter> fmt,
                        const std::string& file_name,
                        on_start start,
                        bool flush,
                        std::shared_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());
    /**
     * Construct a rolling_file_writer.
     * 
     * @param fmt the formatter
     * @param start action to take on start
     * @param flush whether to flush the 
     * @param roller the roller
     * @param trigger the optional trigger
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     * @throw std::invalid_argument if the roller is an 
     *        uninitialized std::shared_ptr
     * @throw std::invalid_argument if the trigger cannot be 
     *        resolved
     */
    rolling_file_writer(std::shared_ptr<formatter> fmt,
                        on_start start,
                        bool flush,
                        std::shared_ptr<file_roller> roller,
                        std::shared_ptr<file_roll_trigger> trigger = std::shared_ptr<file_roll_trigger>());
    //@}

    /**
     * Return the roller.
     * 
     * @return the roller
     */
    std::shared_ptr<file_roller> get_file_roller() const;
    /**
     * Return the trigger.
     * 
     * @return the trigger
     */
    std::shared_ptr<file_roll_trigger> get_file_roll_trigger() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT void init();

    std::shared_ptr<file_roller> roller_;
    std::shared_ptr<file_roll_trigger> trigger_;
};

inline std::shared_ptr<file_roller> rolling_file_writer::get_file_roller() const
{
    return roller_;
}

inline std::shared_ptr<file_roll_trigger> rolling_file_writer::get_file_roll_trigger() const
{
    return trigger_;
}

}

#endif
