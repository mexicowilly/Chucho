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

#include "logger_win_controller.hpp"
#include "logger_emittable.hpp"
#include <chucho/log.hpp>
#include <assert.h>
#include <vector>
#include <QStyledItemDelegate>
#include <QLineEdit>

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

class logger_delegate : public QStyledItemDelegate
{
public:
    logger_delegate(QTreeWidget& logger_win)
        : logger_win_(logger_win)
    {
    }

    virtual QWidget* createEditor(QWidget* parent,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const override
    {
        QLineEdit* le = reinterpret_cast<QLineEdit*>(QStyledItemDelegate::createEditor(parent, option, index));
        std::vector<QString> existing;
        for (int i = 0; i < logger_win_.topLevelItemCount() - 1; i++)
            existing.push_back(logger_win_.topLevelItem(i)->text(0));
        le->setValidator(new logger_validator(existing));
        return le;
    }

private:
    QTreeWidget& logger_win_;
};

}

namespace chucho
{

namespace config
{

logger_win_controller::logger_win_controller(Ui::main_ui& ui)
    : logger_win_(*ui.logger_win)
{
    connect(ui.add_logger_action, SIGNAL(triggered()), SLOT(new_logger()));
    connect(&logger_win_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(item_double_clicked(QTreeWidgetItem*, int)));
    logger_win_.headerItem()->setText(1, "");
    logger_win_.setColumnWidth(0, 200);
    logger_win_.setItemDelegateForColumn(0, new logger_delegate(logger_win_));
}

void logger_win_controller::item_double_clicked(QTreeWidgetItem* it, int col)
{
    if (logger_win_.indexOfTopLevelItem(it) == logger_win_.topLevelItemCount() - 1)
        new_logger();
}

void logger_win_controller::new_logger()
{
    logger_emittable* item = new logger_emittable("<Type logger name>");
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    logger_win_.insertTopLevelItem(logger_win_.topLevelItemCount() - 1, item);
    logger_win_.editItem(item);
    logger_win_.setCurrentItem(item);
}

}

}