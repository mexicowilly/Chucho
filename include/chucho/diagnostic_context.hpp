#if !defined(CHUCHO_DIAGNOSTIC_CONTEXT_HPP__)
#define CHUCHO_DIAGNOSTIC_CONTEXT_HPP__

#include <chucho/export.hpp>
#include <string>
#include <map>

namespace chucho
{

class CHUCHO_EXPORT diagnostic_context
{
public:
    static std::string& at(const std::string& key);
    static void clear();
    static bool empty();
    static void erase(const std::string& key);
    static std::map<std::string, std::string> get();
    static void set(const std::map<std::string, std::string>& ctx);
};

}

#endif
