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
#include "logger_creator_item.hpp"
#include "emittable.hpp"
#include <chucho/log.hpp>
#include <QStyledItemDelegate>
#include <QFileDialog>
#include <fstream>

namespace
{

class editable_delegate : public QStyledItemDelegate, chucho::loggable<editable_delegate>
{
public:
    virtual QWidget* createEditor(QWidget* parent,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const override
    {
        return create_editor_function_ ?  create_editor_function_(parent) : nullptr;
    }

    void set_create_editor_function(std::function<QWidget*(QWidget*)> func)
    {
        create_editor_function_ = func;
    }

private:
    std::function<QWidget*(QWidget*)> create_editor_function_;
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
    connect(ui.save_action, SIGNAL(triggered()), SLOT(save()));
    logger_win_.headerItem()->setText(1, "");
    logger_win_.setColumnWidth(0, 200);
    logger_win_.setItemDelegate(new editable_delegate());
    logger_win_.addTopLevelItem(new logger_creator_item(logger_win_));
}

void logger_win_controller::item_double_clicked(QTreeWidgetItem* it, int col)
{
    auto del = reinterpret_cast<editable_delegate*>(logger_win_.itemDelegate());
    del->set_create_editor_function(std::function<QWidget*(QWidget*)>());
    auto ei = dynamic_cast<editable_item*>(it);
    if (ei && col == ei->column())
    {
        del->set_create_editor_function(std::bind(&editable_item::create_editor, reinterpret_cast<editable_item*>(it), std::placeholders::_1));
        auto ci = dynamic_cast<creator_item*>(it);
        if (ci)
            ci->create_item(ci->parent());
    }
}

void logger_win_controller::new_logger()
{
    for (int i = 0; i < logger_win_.topLevelItemCount(); i++)
    {
        auto lci = dynamic_cast<logger_creator_item*>(logger_win_.topLevelItem(i));
        if (lci)
        {
            lci->create_item(nullptr);
            break;
        }
    }
}

void logger_win_controller::save()
{
    if (save_file_name_.isEmpty())
        save_file_name_ = QFileDialog::getSaveFileName(&logger_win_);
    if (!save_file_name_.isEmpty())
    {
        auto bytes = save_file_name_.toUtf8();
        std::string str(bytes.begin(), bytes.end());
        std::ofstream stream(str.c_str());
        for (int i = 0; i < logger_win_.topLevelItemCount(); i++)
        {
            auto em = dynamic_cast<emittable*>(logger_win_.topLevelItem(i));
            if (em)
                em->emit_config(stream, 0);
        }
    }
}

}

}