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

#if !defined(CHUCHO_SLIDING_NUMBERED_FILE_ROLLER_HPP__)
#define CHUCHO_SLIDING_NUMBERED_FILE_ROLLER_HPP__

#include <chucho/file_roller.hpp>

namespace chucho
{

/**
 * @class sliding_numbered_file_roller sliding_numbered_file_roller.hpp chucho/sliding_numbered_file_roller.hpp 
 * A file_roller that numbers files from a minimum index in 
 * increasing order. New files have higher numbers than old 
 * files, and files are never renamed. This allows expensive 
 * rename operations on large sets of files to be avoided. You 
 * provide a maximum count, which is the total set of files, not 
 * the set of "rolled" ones. For example, if you have a 
 * rolling_file_writer with a file name of my.log, and your 
 * sliding_numbered_file_roller has a maximum count of 3, then 
 * the files will roll like this: 
 * @code 
 * my.log 
 * my.log.1 
 * my.log.2 
 * @endcode 
 * Now, on the next roll operation you will have the following 
 * set of files: 
 * @code 
 * my.log.1 
 * my.log.2 
 * my.log.3 
 * @endcode 
 * And the current active file is the one with the highest 
 * index. 
 *  
 * @ingroup rolling 
 */
class CHUCHO_EXPORT sliding_numbered_file_roller : public file_roller
{
public:
    /**
     * @name Constructors
     */
    //@{
    /**
     * Construct a sliding roller with a maximum count and optional 
     * compressor. 
     * 
     * @param max_count the maximum of number of files that can 
     *                  exist for this roller at a time
     * @param cmp the compressor
     */
    sliding_numbered_file_roller(std::size_t max_count,
                                 std::shared_ptr<file_compressor> cmp = std::shared_ptr<file_compressor>());
    /**
     * Construct a sliding roller with a minimum index, a maximum 
     * count and optional compressor. 
     * 
     * @param min_index the starting index for the first rolled file
     * @param max_count the maximum of number of files that can 
     *                  exist for this roller at a time
     * @param cmp the compressor
     */
    sliding_numbered_file_roller(int min_index,
                                 std::size_t max_count,
                                 std::shared_ptr<file_compressor> cmp = std::shared_ptr<file_compressor>());
    //@}

    virtual std::string get_active_file_name() override;
    /**
     * Return the index of the file that is current being written.
     * @return the current index
     */
    int get_current_index() const;
    /**
     * Return the maximum number of files that can exist at a time.
     * 
     * @return the maximum count
     */
    std::size_t get_max_count() const;
    /**
     * Return the lowest index in the set. This is not the lowest 
     * that is currently in existence, but rather the starting 
     * minimum. 
     * 
     * @return the minimum index
     */
    int get_min_index() const;
    virtual void roll() override;

private:
    CHUCHO_NO_EXPORT std::string get_file_name(int idx, bool with_compression_ext) const;
    CHUCHO_NO_EXPORT bool is_compressed(int idx) const;

    std::size_t max_count_;
    int cur_index_;
    int min_index_;
};

inline int sliding_numbered_file_roller::get_current_index() const
{
    return cur_index_;
}

inline std::size_t sliding_numbered_file_roller::get_max_count() const
{
    return max_count_;
}

inline int sliding_numbered_file_roller::get_min_index() const
{
    return min_index_;
}

inline bool sliding_numbered_file_roller::is_compressed(int idx) const
{
    return compressor_ && idx <= cur_index_ - compressor_->get_min_index();
}

}

#endif
