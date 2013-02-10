#if !defined(CHUCHO_NUMBERED_FILE_ROLLER_HPP__)
#define CHUCHO_NUMBERED_FILE_ROLLER_HPP__

#include <chucho/file_roller.hpp>

namespace chucho
{

/**
 * @class numbered_file_roller numbered_file_roller.hpp chucho/numbered_file_roller.hpp 
 * A @ref file_roller that numbers the rolled files. This roller 
 * can be used to created sequences of numbered files, like 
 * my.log, my.log.1, my.log.2, etc.... 
 *  
 * @ingroup rolling 
 */
class CHUCHO_EXPORT numbered_file_roller : public file_roller
{
public:
    /**
     * @name Constructors
     */
    //@{
    /**
     * Construct a numbered_file_roller with a maximum index. The 
     * minimum index will be 1. 
     * 
     * @param max_index the maximum index
     */
    numbered_file_roller(int max_index);
    /**
     * Construct a numbered_file_roller with both a minimum and a 
     * maximum index. 
     * 
     * @pre max_index must be greater than or equal to min_index
     * @throw std::invalid_argument if min_index is greater than 
     *        max_index
     * @param min_index the minimum index
     * @param max_index the maximum index
     */
    numbered_file_roller(int min_index, int max_index);
    //@}

    virtual std::string get_active_file_name() override;
    /**
     * Return the maximum index.
     * 
     * @return the maximum index
     */
    int get_max_index() const;
    /**
     * Return the minimum index.
     * 
     * @return the minimum index
     */
    int get_min_index() const;
    virtual void roll() override;

private:
    CHUCHO_NO_EXPORT std::string get_name(int number) const;

    int min_index_;
    int max_index_;
};

inline int numbered_file_roller::get_max_index() const
{
    return max_index_;
}

inline int numbered_file_roller::get_min_index() const
{
    return min_index_;
}

}

#endif
