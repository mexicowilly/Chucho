#if !defined(CHUCHO_CONSOLE_WRITER__)
#define CHUCHO_CONSOLE_WRITER__

#include <chucho/writer.hpp>
#include <iosfwd>

namespace chucho
{

/**
 * @class console_writer console_writer.hpp chucho/console_writer.hpp 
 * A writer that writes to the console.
 *  
 * @warning This class is meant for chucho use only. Its 
 *          subclasses @ref cout_writer and @ref cerr_writer are
 *          the only subclasses that should exist. Please don't
 *          use this class. Thank you.
 *  
 * @ingroup writers 
 */
class CHUCHO_EXPORT console_writer : public writer
{
protected:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a console_writer.
     * 
     * @param fmt the formatter
     * @param stream the stream, which should be either std::cout or 
     *               std::cerr
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     */
    console_writer(std::shared_ptr<formatter> fmt,
                   std::ostream& stream);
    //@}

    virtual void write_impl(const event& evt) override;

    /**
     * The console stream to which this writer writes.
     */
    std::ostream& stream_;
};

}

#endif
