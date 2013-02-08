#if !defined(CHUCHO_COUT_WRITER_HPP__)
#define CHUCHO_COUT_WRITER_HPP__

#include <chucho/console_writer.hpp>

namespace chucho
{

/**
 * @class cout_writer cout_writer.hpp chucho/cout_writer.hpp 
 * A @ref console_writer that writes to std::cout. 
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT cout_writer : public console_writer
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a cout_writer.
     * 
     * @param fmt the formatter
     */
    cout_writer(std::shared_ptr<formatter> fmt);
    //@}
};

}

#endif
