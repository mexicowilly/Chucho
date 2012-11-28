#include <chucho/writer.hpp>
#include <chucho/level_threshold_filter.hpp>

namespace chucho
{

writer::writer()
{
    add_filter(std::make_shared<level_threshold_filter>());
}

writer::~writer()
{
}

}
