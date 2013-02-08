#include <chucho/file_roll_trigger.hpp>

namespace chucho
{

/**
 * A @ref file_roll_trigger that monitors file size. When the 
 * active log file reaches this trigger's maximum size, then it 
 * will signal that now would be a good time to roll the file. 
 */
class CHUCHO_EXPORT size_file_roll_trigger : public file_roll_trigger
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a size_file_roll_trigger.
     * 
     * @param max_size the maximum size that a file can reach before 
     *                 rolling
     */
    size_file_roll_trigger(unsigned long long max_size);
    //@}

    /**
     * Return this trigger's maximum size.
     * 
     * @return this trigger's maximum size
     */
    unsigned long long get_max_size() const;
    /**
     * If the size of active_file is greater than this trigger's 
     * maximum size, then this trigger fires. 
     * 
     * @param active_file the currently active file name
     * @param e the log event
     * @return bool true if the file size has exceeded the limit
     */
    virtual bool is_triggered(const std::string& active_file, const event& e) override;

private:
    unsigned long long max_size_;
};

inline unsigned long long size_file_roll_trigger::get_max_size() const
{
    return max_size_;
}

}
