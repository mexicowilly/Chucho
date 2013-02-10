#if !defined(CHUCHO_EXCEPTION_HPP__)
#define CHUCHO_EXCEPTION_HPP__

#include <chucho/export.hpp>
#include <exception>
#include <string>

namespace chucho
{

/**
 * @class exception exception.hpp chucho/exception.hpp
 * Chucho exceptions.
 */
class CHUCHO_EXPORT exception : public std::exception
{
public:
    /**
     * Find the messages from all nested exceptions and piece them 
     * together. 
     * 
     * @param e the exception from which to extract nested messages
     * @return A brack-delimited piece of text containing all nested 
     *         messages
     */
    static std::string nested_whats(const std::exception& e);

    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct an exception.
     * 
     * @param msg the exception's message
     */
    exception(const std::string& msg);
    //@}

    /**
     * Return the exception's message.
     * 
     * @return the message
     */
    virtual const char* what() const noexcept override;

protected:
    /**
     * The message
     */
    std::string message_;
};

}

#endif
