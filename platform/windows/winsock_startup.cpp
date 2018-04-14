#include "winsock_startup.hpp"
#include <winsock2.h>
#include "error_util.hpp"
#include <chucho/garbage_cleaner.hpp>
#include <chucho/exception.hpp>

namespace
{

class worker
{
public:
    worker()
    {
        WSADATA data;
        int rc = WSAStartup(MAKEWORD(2, 0), &data);
        if (rc != 0)
            throw chucho::exception("Could not start socket subsystem: " + chucho::error_util::message(WSAGetLastError()));
    }

    ~worker()
    {
        WSACleanup();
    }
};

}

namespace chucho
{

namespace winsock
{

std::once_flag once;

void startup()
{
    static worker* wrk = new worker();
    chucho::garbage_cleaner::get().add([&] () { delete wrk; });
}

}

}
