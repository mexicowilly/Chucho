/*
 * Copyright 2013-2016 Will Mason
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

#include "filter_creator_item.hpp"
#include "level_threshold_filter_item.hpp"

namespace chucho
{

namespace config
{

filter_creator_item::filter_creator_item(QTreeWidget& tree)
    : creator_from_list_item(tree, "<Add Filter>")
{
    creators_["Duplicate Message Filter"] = nullptr;
    creators_["Level Filter"] = nullptr;
    creators_["Level Threshold Filter"] = get_func<level_threshold_filter_item>();
    creators_["Ruby Evaluator Filter"] = nullptr;
}

}

}