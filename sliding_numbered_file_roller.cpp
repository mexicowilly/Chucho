/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/sliding_numbered_file_roller.hpp>
#include <chucho/file.hpp>
#include <chucho/file_writer.hpp>
#include <sstream>

namespace chucho
{

sliding_numbered_file_roller::sliding_numbered_file_roller(std::size_t max_count,
                                                           std::shared_ptr<file_compressor> cmp)
    : file_roller(cmp),
      max_count_(max_count),
      cur_index_(0),
      min_index_(1)
{
    set_status_origin("sliding_numbered_file_roller");
}

sliding_numbered_file_roller::sliding_numbered_file_roller(int min_index,
                                                           std::size_t max_count,
                                                           std::shared_ptr<file_compressor> cmp)
    : file_roller(cmp),
      max_count_(max_count),
      cur_index_(min_index - 1),
      min_index_(min_index)
{
    if (min_index == std::numeric_limits<int>::min())
        throw std::invalid_argument("The min_index can be any number except " + std::to_string(min_index));
    set_status_origin("sliding_numbered_file_roller");
}

std::string sliding_numbered_file_roller::get_active_file_name()
{
    return get_file_name(cur_index_, false);
}

std::string sliding_numbered_file_roller::get_file_name(int idx, bool with_compression_ext) const
{
    std::ostringstream stream;
    stream << file_writer_->get_initial_file_name();
    if (idx >= min_index_)
        stream << '.' << idx;
    if (with_compression_ext && is_compressed(idx))
        stream << compressor_->get_extension();
    return stream.str();
}

void sliding_numbered_file_roller::roll()
{
    std::string fn;
    try
    {
        int cut = ++cur_index_ - max_count_;
        if (cut >= min_index_ - 1)
        {
            fn = get_file_name(cut, true);
            file::remove(fn);
        }
    }
    catch (std::exception& e)
    {
        report_warning("Error removing " + fn + ": " + e.what());
    }
    if (compressor_)
    {
        int cmp = cur_index_ - compressor_->get_min_index();
        if (cmp >= min_index_ - 1)
        {
            std::string nm = get_file_name(cmp, false);
            if (file::exists(nm))
                compressor_->compress(nm);
        }
    }
}

}
