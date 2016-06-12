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

#if !defined(CHUCHO_CONFIG_LOGGER_WIN_CONTROLLER_HPP__)
#define CHUCHO_CONFIG_LOGGER_WIN_CONTROLLER_HPP__

#include <chucho/loggable.hpp>
#include <QObject>
#include <QTreeWidget>
#include "ui_chucho.h"

namespace chucho
{

namespace config
{

class logger_win_controller : public QObject, public chucho::loggable<logger_win_controller>
{
    Q_OBJECT

public:
    logger_win_controller(Ui::main_ui& ui);

public slots:
    void item_double_clicked(QTreeWidgetItem* it, int col);
    void new_logger();

private:
    QTreeWidget& logger_win_;
};

}

}

#endif