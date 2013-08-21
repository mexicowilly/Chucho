#include <chucho/environment.hpp>
#include <windows.h>

namespace chucho
{

namespace environment
{

optional<std::string> get(const std::string& var)
{
    optional<std::string> result;
    DWORD rc = GetEnvironmentVariableA(var.c_str(), nullptr, 0);
    if (rc > 0)
    {
        result = std::string(rc, 0);
        GetEnvironmentVariableA(var.c_str(),
                                const_cast<char*>(result->data()),
                                rc);
        result->pop_back();
    }
    return result;
}

}

}
