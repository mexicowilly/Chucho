#include <chucho/marker.hpp>

namespace chucho
{

std::ostream& operator<< (std::ostream& stream, const marker& mark)
{
    stream << mark.get_name();
    if (!mark.empty())
    {
        bool first = true;
        stream << " [ ";
        for (auto m : mark)
        {
            if (first)
                first = false;
            else
                stream << ", ";
            stream << m;
        }
        stream << " ]";
    }
    return stream;
}

marker::marker(const std::string& name)
    : name_(name)
{
}

}
