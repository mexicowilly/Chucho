#include <chucho/diagnostic_context.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/exception.hpp>
#include <mutex>
#include <thread>
#include <map>
#include <vector>
#include <memory>
#include <condition_variable>
#include <windows.h>

namespace
{

class thread_exit_manager
{
public:
    thread_exit_manager();
    ~thread_exit_manager();

    void add(HANDLE thr, std::map<std::string, std::string>* diag);

private:
    void main();

    std::unique_ptr<std::thread> thread_;
    std::map<HANDLE, std::map<std::string, std::string>*> diags_;
    std::mutex guard_;
    std::condition_variable condition_;
    bool stop_;
};

thread_exit_manager::thread_exit_manager()
    : stop_(false)
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
    thread_.reset(new std::thread(std::bind(&thread_exit_manager::main, this)));
}

thread_exit_manager::~thread_exit_manager()
{
    guard_.lock();
    stop_ = true;
    condition_.notify_one();
    guard_.unlock();
    thread_->join();
    // This is called in finalize, so Chucho can no longer be used, which
    // means it is time to clean up all existing diagnostic contexts.
    std::lock_guard<std::mutex> lg(guard_);
    for (auto d : diags_)
        delete d.second;
}

void thread_exit_manager::add(HANDLE thr, std::map<std::string, std::string>* diag)
{
    std::lock_guard<std::mutex> lg(guard_);
    diags_[thr] = diag;
}

void thread_exit_manager::main()
{
    std::vector<HANDLE> thrs;
    while (true)
    {
        std::unique_lock<std::mutex> ul(guard_);
        while (!stop_ && diags_.empty())
            condition_.wait(ul);
        if (stop_)
            break;
        thrs.clear();
        for (auto d : diags_)
            thrs.push_back(d.first);
        ul.unlock();
        DWORD rc = WaitForMultipleObjects(thrs.size(), &thrs[0], FALSE, 500);
        if (rc != WAIT_FAILED && rc != WAIT_TIMEOUT)
        {
            HANDLE hnd = thrs[rc - WAIT_OBJECT_0];
            ul.lock();
            delete diags_[hnd];
            diags_.erase(hnd);
            ul.unlock();
        }
    }
}

class key_manager
{
public:
    key_manager();
    ~key_manager();

    DWORD get_key();

private:
    DWORD key_;
};

key_manager::key_manager()
    : key_(TlsAlloc())
{
    if (key_ == TLS_OUT_OF_INDEXES)
        throw chucho::exception("No more thread local storage indexes");
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

key_manager::~key_manager()
{
    TlsFree(key_);
}

inline DWORD key_manager::get_key()
{
    return key_;
}

std::once_flag konce;
std::once_flag tonce;

key_manager& kmgr()
{
    // This is cleaned at finalize time
    static key_manager* km;

    std::call_once(konce, [&] () { km = new key_manager(); });
    return *km;
}

thread_exit_manager& temgr()
{
    static thread_exit_manager* tem;

    std::call_once(tonce, [&] () { tem = new thread_exit_manager(); });
    return *tem;
}

}

namespace chucho
{

std::map<std::string, std::string>& diagnostic_context::get_map()
{
    void* p = TlsGetValue(kmgr().get_key());
    if (p == nullptr)
    {
        p = new std::map<std::string, std::string>();
        temgr().add(GetCurrentThread(),
                    reinterpret_cast<std::map<std::string, std::string>*>(p));
        TlsSetValue(kmgr().get_key(), p);
    }
    return *reinterpret_cast<std::map<std::string, std::string>*>(p);
}

}
