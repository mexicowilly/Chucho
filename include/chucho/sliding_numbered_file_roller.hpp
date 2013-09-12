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

class CHUCHO_EXPORT sliding_numbered_file_roller : public file_roller
{
public:
    sliding_numbered_file_roller(std::size_t max_count,
                                 std::shared_ptr<file_compressor> cmp = std::shared_ptr<file_compressor>());
    sliding_numbered_file_roller(int min_index,
                                 std::size_t max_count,
                                 std::shared_ptr<file_compressor> cmp = std::shared_ptr<file_compressor>());

    virtual std::string get_active_file_name() override;
    int get_current_index() const;
    std::size_t get_max_count() const;
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
