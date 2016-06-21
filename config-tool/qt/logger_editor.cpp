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

#include "logger_editor.hpp"
#include <vector>

namespace
{

class logger_validator : public QValidator
{
public:
    logger_validator(const std::vector<QString>& existing)
        : existing_(existing)
    {
    }

    virtual State validate(QString& input, int& pos) const override
    {
        State st = std::find(existing_.begin(), existing_.end(), input) == existing_.end() ?
            Acceptable : Intermediate;
        return st;
    }

private:
    std::vector<QString> existing_;
};

}

namespace chucho
{

namespace config
{

logger_editor::logger_editor(QWidget* parent, QTreeWidget& tree)
    : QLineEdit(parent)
{
    std::vector<QString> existing;
    for (int i = 0; i < tree.topLevelItemCount() - 1; i++)
        existing.push_back(tree.topLevelItem(i)->text(0));
    setValidator(new logger_validator(existing));
}

}

}