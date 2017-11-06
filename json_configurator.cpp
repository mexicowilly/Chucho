#include <chucho/json_configurator.hpp>
#include <chucho/configurable_factory.hpp>
#include <cJSON.h>
#include <cstring>

namespace chucho
{

json_configurator::json_configurator(const security_policy& sec_pol)
    : configurator(sec_pol)
{
    set_status_origin("json_configurator");
}

void json_configurator::configure(std::istream& in)
{
    std::unique_ptr<std::string> f(new std::string((std::istreambuf_iterator<char>(in)),
                                                   std::istreambuf_iterator<char>()));
    const char* ret;
    auto json = cJSON_ParseWithOpts(f->c_str(), &ret, false);
    if (json == nullptr)
    {
        // error
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
    {
        // error
    }
    auto lgr_fact = get_factories().find("chucho::logger")->second;
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
                auto arr = lgr_child->child;
                if (!cJSON_IsArray(arr))
                {
                    // error
                }
                for (int i = 0; i < cJSON_GetArraySize(arr); i++)
                {
                    auto jwrt = cJSON_GetArrayItem(arr, i);
                }
            }
            else if (std::strcmp(lgr_child->string, "writes_to_ancestors") == 0)
            {
                lgr_mnto->handle("write_to_ancestors", (cJSON_IsTrue(lgr_child) ? "true" : "false"));
            }
            else
            {
                // This should just be level, but we'll let it error out on other values
                lgr_mnto->handle(lgr_child->string, lgr_child->valuestring);
            }
            lgr_child = lgr_child->next;
        }
        jlgr = jlgr->next;
    }
}

}