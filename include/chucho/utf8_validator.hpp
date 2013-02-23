#if !defined(CHUCHO_UTF8_VALIDATOR_HPP__)
#define CHUCHO_UTF8_VALIDATOR_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <istream>

namespace chucho
{

namespace utf8
{

void validate(std::istream& stream);

}

}

#endif
