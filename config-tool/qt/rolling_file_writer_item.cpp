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

#include "rolling_file_writer_item.hpp"
#include "creator_from_list_item.hpp"
#include "number_editable_item.hpp"
#include "text_editable_item.hpp"
#include "from_list_editable_item.hpp"

namespace
{

using chucho::config::operator<<;

class file_compressor_item : public QTreeWidgetItem, public chucho::config::emittable
{
public:
    virtual void emit_config(std::ostream& stream, std::size_t tabstop) override
    {
        indent(stream, tabstop++) << name_prefix_ << "_file_compressor:" << std::endl;
        indent(stream, tabstop) << "- min_index: " << min_idx_->text(1) << std::endl;
    }

protected:
    file_compressor_item(const std::string& name_prefix)
        : name_prefix_(name_prefix)
    {
        min_idx_ = new chucho::config::number_editable_item("Minimum Index", 1, 1, 1000);
        addChild(min_idx_);
    }

private:
    std::string name_prefix_;
    QTreeWidgetItem* min_idx_;
};

class bzip2_file_compressor_item : public file_compressor_item
{
public:
    bzip2_file_compressor_item(QTreeWidget&) : file_compressor_item("bzip2") { }
};

class gzip_file_compressor_item : public file_compressor_item
{
public:
    gzip_file_compressor_item(QTreeWidget&) : file_compressor_item("gzip") { }
};

class zip_file_compressor_item : public file_compressor_item
{
public:
    zip_file_compressor_item(QTreeWidget&) : file_compressor_item("zip") { }
};

class file_compressor_creator_item : public chucho::config::creator_from_list_item
{
public:
    file_compressor_creator_item(QTreeWidget& tree)
        : creator_from_list_item(tree, "<Add Compressor>", true)
    {
        creators_["Gzip Compressor"] = get_func<gzip_file_compressor_item>();
        creators_["Bzip2 Compressor"] = get_func<bzip2_file_compressor_item>();
        creators_["Zip Compressor"] = get_func<zip_file_compressor_item>();
    }
};

class file_roller_item : public QTreeWidgetItem, public chucho::config::emittable
{
public:
    file_roller_item(QTreeWidget& tree)
    {
        addChild(new file_compressor_creator_item(tree));
    }

    virtual void emit_config(std::ostream& stream, std::size_t tabstop) override
    {
        for (int i = 0; i < childCount(); i++)
        {
            auto fci = dynamic_cast<file_compressor_item*>(child(i));
            if (fci)
                fci->emit_config(stream, tabstop);
        }
    }
};

class numbered_file_roller_item : public file_roller_item
{
public:
    numbered_file_roller_item(QTreeWidget& tree)
        : file_roller_item(tree)
    {
        maximum_ = new chucho::config::number_editable_item("Maximum Index", 1, -1000, 1000);
        insertChild(0, maximum_);
        minimum_ = new chucho::config::number_editable_item("Minimum Index", 1, -1000, 1000);
        insertChild(0, minimum_);
    }

    virtual void emit_config(std::ostream& stream, std::size_t tabstop) override
    {
        indent(stream, tabstop++) << "- chucho::numbered_file_roller:" << std::endl;
        indent(stream, tabstop) << "- min_index: " << minimum_->text(1) << std::endl;
        indent(stream, tabstop) << "- max_index: " << maximum_->text(1) << std::endl;
        file_roller_item::emit_config(stream, tabstop);
    }

private:
    QTreeWidgetItem* minimum_;
    QTreeWidgetItem* maximum_;
};

class sliding_numbered_file_roller_item : public file_roller_item
{
public:
    sliding_numbered_file_roller_item(QTreeWidget& tree)
        : file_roller_item(tree)
    {
        max_count_ = new chucho::config::number_editable_item("Maximum Index", 1, 1, 1000);
        insertChild(0, max_count_);
        min_ = new chucho::config::number_editable_item("Minimum Index", 1, -1000, 1000);
        insertChild(0, min_);
    }

    virtual void emit_config(std::ostream& stream, std::size_t tabstop) override
    {
        indent(stream, tabstop++) << "- chucho::sliding_numbered_file_roller:" << std::endl;
        indent(stream, tabstop) << "- min_index: " << min_->text(1) << std::endl;
        indent(stream, tabstop) << "- max_count: " << max_count_->text(1) << std::endl;
        file_roller_item::emit_config(stream, tabstop);
    }

private:
    QTreeWidgetItem* min_;
    QTreeWidgetItem* max_count_;
};

class time_file_roller_item : public file_roller_item
{
public:
    time_file_roller_item(QTreeWidget& tree)
        : file_roller_item(tree)
    {
        max_history_ = new chucho::config::number_editable_item("Maximum History", 1, 1, 1000);
        insertChild(0, max_history_);
        pattern_ = new chucho::config::text_editable_item("File Name Pattern", "");
        insertChild(0, pattern_);
    }

    virtual void emit_config(std::ostream& stream, std::size_t tabstop) override
    {
        indent(stream, tabstop++) << "- chucho::time_file_roller:" << std::endl;
        indent(stream, tabstop) << "- file_name_pattern: '" << pattern_->text(1) << '\'' << std::endl;
        indent(stream, tabstop) << "- max_history: " << max_history_->text(1) << std::endl;
        file_roller_item::emit_config(stream, tabstop);
    }

private:
    QTreeWidgetItem* pattern_;
    QTreeWidgetItem* max_history_;
};

class roller_creator_item : public chucho::config::creator_from_list_item
{
public:
    roller_creator_item(QTreeWidget& tree)
        : creator_from_list_item(tree, "<Add Roller>", true)
    {
        creators_["Numbered Roller"] = get_func<numbered_file_roller_item>();
        creators_["Sliding Numbered Roller"] = get_func<sliding_numbered_file_roller_item>();
        creators_["Time Roller"] = get_func<time_file_roller_item>();
    }
};

class interval_file_roll_trigger_item : public QTreeWidgetItem, public chucho::config::emittable
{
public:
    interval_file_roll_trigger_item(QTreeWidget& tree)
    {
        interval_ = new chucho::config::number_editable_item("Count", 1, 1, 5000);
        addChild(interval_);
        period_ = new chucho::config::from_list_editable_item("Time Period",
                                                              "hours",
                                                              5,
                                                              "minutes",
                                                              "hours",
                                                              "days",
                                                              "weeks",
                                                              "months");
        addChild(period_);
    }

    virtual void emit_config(std::ostream& stream, std::size_t tabstop) override
    {
        indent(stream, tabstop++) << "- chucho::interval_file_roll_trigger:" << std::endl;
        indent(stream, tabstop) << "- every: " << interval_->text(1) << ' ' << period_->text(1) << std::endl;
    }

private:
    QTreeWidgetItem* interval_;
    QTreeWidgetItem* period_;
};

class size_file_roll_trigger_item : public QTreeWidgetItem, public chucho::config::emittable
{
public:
    size_file_roll_trigger_item(QTreeWidget&)
    {
        max_size_ = new chucho::config::number_editable_item("Maximum Size", 1, 1, 1024 * 1024 * 1024);
        addChild(max_size_);
    }

    virtual void emit_config(std::ostream& stream, std::size_t tabstop) override
    {
        indent(stream, tabstop++) << "- chucho::size_file_roll_trigger:" << std::endl;
        indent(stream, tabstop) << "- max_size: " << max_size_->text(1) << std::endl;
    }

private:
    QTreeWidgetItem* max_size_;
};

class trigger_creator_item : public chucho::config::creator_from_list_item
{
public:
    trigger_creator_item(QTreeWidget& tree)
        : creator_from_list_item(tree, "<Add Trigger>", true)
    {
        creators_["Interval Trigger"] = get_func<interval_file_roll_trigger_item>();
        creators_["Size Trigger"] = get_func<size_file_roll_trigger_item>();
    }
};

}

namespace chucho
{

namespace config
{

rolling_file_writer_item::rolling_file_writer_item(QTreeWidget& tree)
    : file_writer_item(tree, "rolling_file")
{
    addChild(new roller_creator_item(tree));
    addChild(new trigger_creator_item(tree));
}

void rolling_file_writer_item::emit_config(std::ostream& stream, std::size_t tabstop)
{
    file_writer_item::emit_config(stream, tabstop++);
    for (int i = 0; i < childCount(); i++)
    {
        auto em = dynamic_cast<emittable*>(child(i));
        if (em)
            em->emit_config(stream, tabstop);
    }
}

}

}