#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <vector>
#include <array>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fts.h>

namespace chucho
{

namespace file
{

const char dir_sep = '/';

std::string base_name(const std::string& name)
{
    if (name.empty())
        return ".";
    // clang has a bug where it returns past the end instead of npos
    if (name.find_first_not_of('/') >= name.length())
        return "/";
    std::size_t end = name.find_last_not_of('/');
    std::size_t slash = name.rfind('/', end);
    return name.substr(slash + 1, end - slash);
}

void create_directories(const std::string& name)
{
    if (access(name.c_str(), F_OK) != 0)
    {
        std::vector<std::string> dirs = { name };
        std::string dir = directory_name(name);
        while (dir != "/" && dir != ".")
        {
            dirs.push_back(dir);
            dir = directory_name(dir);
        }
        for (auto itor = dirs.rbegin(); itor != dirs.rend(); itor++)
        {
            if (access((*itor).c_str(), F_OK) != 0)
                create_directory(*itor);
        }
    }
}

void create_directory(const std::string& name)
{
    if (::mkdir(name.c_str(), 0755) != 0)
        throw exception("Could not create directory " + name + ": " + std::string(strerror(errno)));
}

std::string directory_name(const std::string& name)
{
    // clang has a bug where it returns past the end instead of npos
    if (name.empty())
        return ".";
    if (name.find_first_not_of('/') >= name.length())
        return "/";
    std::size_t end = name.find_last_not_of('/');
    std::size_t slash = name.rfind('/', end);
    if (slash == std::string::npos)
        return ".";
    end = name.find_last_not_of('/', slash);
    // clang has a bug where it returns past the end instead of npos
    if (end >= name.length())
        return "/";
    return name.substr(0, end + 1);
}

bool exists(const std::string& name)
{
    return access(name.c_str(), F_OK) == 0;
}

bool is_fully_qualified(const std::string& name)
{
    return !name.empty() && name[0] == '/';
}

void remove_all(const std::string& name)
{
    if (access(name.c_str(), F_OK) != 0)
        return;
    const char* names[] = { name.c_str(), nullptr };
    FTS* fts = fts_open(const_cast<char* const*>(names), FTS_NOSTAT, nullptr);
    if (fts == nullptr)
        throw exception("Could not open directory hierarchy for reading");
    struct sentry
    {
        sentry(FTS* f) { fts_ = f; }
        ~sentry() { fts_close(fts_); }
        FTS* fts_;
    } s(fts);
    FTSENT* ent = fts_read(fts);
    std::array<char, PATH_MAX + 1> path_buf;
    while (ent != nullptr)
    {
        if (ent->fts_info & (FTS_DP | FTS_F | FTS_SL))
        {
            if (remove(ent->fts_accpath) != 0)
            {
                int this_err = errno;
                realpath(ent->fts_accpath, path_buf.data());
                throw exception(std::string("Could not remove directory ") + path_buf.data() + ": " + strerror(this_err));
            }
        }
        ent = fts_read(fts);
    }
}

void remove_directory(const std::string& name)
{
    if (access(name.c_str(), F_OK) == 0 && rmdir(name.c_str()) != 0)
        throw exception("Unable to remove directory " + name + ": " + strerror(errno));
}

}

}
