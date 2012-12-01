#if !defined(CHUCHO_NUMBERED_FILE_ROLLER_HPP__)
#define CHUCHO_NUMBERED_FILE_ROLLER_HPP__

#include <chucho/file_roller.hpp>

namespace chucho
{

class CHUCHO_EXPORT numbered_file_roller : public file_roller
{
public:
    numbered_file_roller(int min_index, int max_index);

    virtual std::string get_active_file_name() override;
    virtual void roll() override;

private:
    CHUCHO_NO_EXPORT std::string get_name(int number) const;

    int min_index_;
    int max_index_;
};

}

#endif
