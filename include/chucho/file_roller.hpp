#if !defined(CHUCHO_FILE_ROLLER_HPP__)
#define CHUCHO_FILE_ROLLER_HPP__

#include <chucho/export.hpp>
#include <string>

namespace chucho
{

class file_writer;

class CHUCHO_EXPORT file_roller
{
public:
    virtual std::string get_active_file_name() = 0;
    virtual void roll() = 0;
    /**
     * You don't own this pointer. It is just a settable reference. 
     * And using shared_from_this() with log_writer won't work, 
     * since this pointer is set in the constructor.
     */
    void set_file_writer(file_writer* file_writer);

protected:
    file_writer* file_writer_;
};

}

#endif
