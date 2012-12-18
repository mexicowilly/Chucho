#include <chucho/diagnostic_context.hpp>
#include <pthread.h>

namespace
{

void destructor(void* p)
{
    delete reinterpret_cast<std::map<std::string, std::string>*>(p);
}

class key_manager
{
public:
    key_manager();
    ~key_manager();

    pthread_key_t get_key();

private:
    pthread_key_t key_;
};

key_manager kmgr;

std::map<std::string, std::string>& get_map()
{
    void* p = pthread_getspecific(kmgr.get_key());
    if (p == nullptr)
    {
        p = new std::map<std::string, std::string>();
        pthread_setspecific(kmgr.get_key(), p);
    }
    return *reinterpret_cast<std::map<std::string, std::string>*>(p);
}

key_manager::key_manager()
{
    pthread_key_create(&key_, destructor);
}

key_manager::~key_manager()
{
    pthread_key_delete(key_);
}

pthread_key_t key_manager::get_key()
{
    return key_;
}

}

namespace chucho
{

std::string& diagnostic_context::at(const std::string& key)
{
    return get_map()[key];
}

void diagnostic_context::clear()
{
    get_map().clear();
}

bool diagnostic_context::empty()
{
    return get().empty();
}

void diagnostic_context::erase(const std::string& key)
{
    get_map().erase(key);
}

std::map<std::string, std::string> diagnostic_context::get()
{
    return get_map();
}

void diagnostic_context::set(const std::map<std::string, std::string>& ctx)
{
    get_map() = ctx;
}

}
