#include <chucho/json_configurator.hpp>
#include <cstring>
#include <algorithm>

namespace chucho
{

json_configurator::json_configurator(const security_policy& sec_pol)
    : configurator(sec_pol)
{
    set_status_origin("json_configurator");
}

void json_configurator::configure(std::istream& in)
{
    report_info("Using JSON configuration");
    std::unique_ptr<std::string> f(new std::string((std::istreambuf_iterator<char>(in)),
                                                   std::istreambuf_iterator<char>()));
    auto json = cJSON_Parse(f->c_str());
    if (json == nullptr)
    {
        auto pos = cJSON_GetErrorPtr() - f->c_str();
        pos = (pos >= 5) ? pos - 5 : 0;
        auto err = f->substr(pos, 10);
        throw std::runtime_error("Error in JSON configuration near: " + err);
    }
    struct sentry
    {
        sentry(cJSON* j) : cj(j) {};
        ~sentry() { cJSON_Delete(cj); }
        cJSON* cj;
    } sent(json);
    f.reset();
    auto cl = cJSON_GetObjectItemCaseSensitive(json, "chucho_loggers");
    if (json == nullptr)
        throw std::runtime_error("Could not find \"chucho_loggers\" element in the JSON configuration");
    auto lgr_fact = get_factory("chucho::logger");
    auto jlgr = cl->child;
    while (jlgr != nullptr)
    {
        auto lgr_mnto = lgr_fact->create_memento(*this);
        lgr_mnto->handle("name", jlgr->string);
        auto lgr_child = jlgr->child;
        while (lgr_child != nullptr)
        {
            if (std::strcmp(lgr_child->string, "writers") == 0)
            {
                if (!cJSON_IsArray(lgr_child))
                    throw std::runtime_error("writers must be an array");
                for (int i = 0; i < cJSON_GetArraySize(lgr_child); i++)
                {
                    auto jwrt = cJSON_GetArrayItem(lgr_child, i)->child;
                    auto fact = get_factory(jwrt->string);
                    lgr_mnto->handle(create_subobject(jwrt->child, fact));
                }
            }
            else
            {
                lgr_mnto->handle(lgr_child->string, value_to_text(lgr_child));
            }
            lgr_child = lgr_child->next;
        }
        lgr_fact->create_configurable(lgr_mnto);
        jlgr = jlgr->next;
    }
}

std::unique_ptr<configurable> json_configurator::create_subobject(const cJSON* json,
                                                                  std::shared_ptr<configurable_factory> fact)
{
    auto facts = get_factories();
    auto mnto = std::move(fact->create_memento(*this));
    while (json != nullptr)
    {
        auto found = facts.find(json->string);
        if (found == facts.end())
            mnto->handle(json->string, value_to_text(json));
        else
            mnto->handle(std::move(create_subobject(json->child, found->second)));
        json = json->next;
    }
    return std::move(fact->create_configurable(mnto));
}

std::shared_ptr<configurable_factory> json_configurator::get_factory(const char* const str)
{
    auto fact = get_factories().find(str);
    if (fact == get_factories().end())
        throw std::runtime_error(std::string("No type named ") + str + " exists");
    return fact->second;
}

std::string json_configurator::value_to_text(const cJSON* json)
{
    std::string result;
    if (cJSON_IsString(json))
    {
        result = json->valuestring;
    }
    else if (cJSON_IsNumber(json))
    {
        // Using a stream properly strips trailing zeros, which std::to_string does not
        std::ostringstream stream;
        stream << json->valuedouble;
        result = stream.str();
    }
    else if (cJSON_IsBool(json))
    {
        result = cJSON_IsTrue(json) ? "true" : "false";
    }
    else
    {
        throw std::runtime_error(std::string("Expected a scalar type for ") + json->string);
    }
    return result;
}

}