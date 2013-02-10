#if !defined(CHUCHO_CERR_WRITER_HPP__)
#define CHUCHO_CERR_WRITER_HPP__

#include <chucho/console_writer.hpp>

namespace chucho
{

/**
 * @class cerr_writer cerr_writer.hpp chucho/cerr_writer.hpp 
 * A @ref console_writer that writes to std::cerr. 
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT cerr_writer : public console_writer
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a cerr_writer.
     * 
     * @param fmt the formatter
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     */
    cerr_writer(std::shared_ptr<formatter> fmt);
    //@}
};

}

#endif
