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

#if !defined(CHUCHO_CONFIG_BRAIN_HPP__)
#define CHUCHO_CONFIG_BRAIN_HPP__

#include "ui_chucho.h"
#include "logger_win_controller.hpp"
#include <QMainWindow>
#include <QApplication>
#include <memory>

namespace chucho
{

namespace config
{

class brain : public QObject
{
    Q_OBJECT

public:
    brain(int argc, char* argv[]);
    int think();

private:
    QApplication app_;
    QMainWindow main_win_;
    Ui::main_ui ui_;
    std::unique_ptr<logger_win_controller> lgr_ctrl_;
};

} 

}

#endif