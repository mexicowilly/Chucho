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

namespace chucho
{

namespace config
{

logger_win_controller::logger_win_controller(Ui::main_ui& ui)
    : logger_win_(*ui.logger_win)
{
    //QObject::connect(ui.add_logger_action, SIGNAL(triggered()), this, SLOT(new_logger()));
    connect(ui.add_logger_action, SIGNAL(triggered()), SLOT(new_logger()));
    connect(&logger_win_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(item_double_clicked(QTreeWidgetItem*, int)));
    logger_win_.headerItem()->setText(1, "");
    logger_win_.setColumnWidth(0, 200);
}

void logger_win_controller::item_double_clicked(QTreeWidgetItem* it, int col)
{
    CHUCHO_INFO_LGBL("topLevelItemCount() " << logger_win_.topLevelItemCount());
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