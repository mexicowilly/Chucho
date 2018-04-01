/*
 * Copyright 2013-2018 Will Mason
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
     * @param cmp the file compressor
     * @throw std::invalid_argument if the default minimum index of 
     *        1 is greater than max_index
     */
    numbered_file_roller(int max_index,
                         std::unique_ptr<file_compressor>&& cmp = std::move(std::unique_ptr<file_compressor>()));
    /**
     * Construct a numbered_file_roller with both a minimum and a 
     * maximum index. 
     * 
     * @pre max_index must be greater than or equal to min_index
     * @throw std::invalid_argument if min_index is greater than 
     *        max_index
     * @param min_index the minimum index
     * @param max_index the maximum index
     * @param cmp the file compressor
     */
    numbered_file_roller(int min_index,
                         int max_index,
                         std::unique_ptr<file_compressor>&& cmp = std::move(std::unique_ptr<file_compressor>()));
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
    CHUCHO_NO_EXPORT std::string get_name(int number, bool with_compression_ext = true) const;
    CHUCHO_NO_EXPORT bool is_compressed(int number) const;

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

inline bool numbered_file_roller::is_compressed(int number) const
{
    return compressor_ &&
           static_cast<unsigned>(number - min_index_ + 1) >= compressor_->get_min_index();
}

}

#endif
