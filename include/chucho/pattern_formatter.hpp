#if !defined(CHUCHO_PATTERN_FORMATTER_HPP__)
#define CHUCHO_PATTERN_FORMATTER_HPP__

#include <chucho/formatter.hpp>
#include <chucho/exception.hpp>
#include <vector>
#include <ctime>

namespace chucho
{

class CHUCHO_EXPORT pattern_exception : public exception
{
public:
    pattern_exception(const std::string& msg);
};

class CHUCHO_EXPORT pattern_formatter : public formatter
{
public:
    pattern_formatter(const std::string& pattern);

    virtual std::string format(const event& evt) override;

private:
    enum class justification
    {
        LEFT,
        RIGHT,
        UNSET
    };

    class CHUCHO_NO_EXPORT piece
    {
    public:
        piece(justification just,
              std::size_t min_width,
              std::size_t max_width);

        std::string get_text(const event& evt) const;

    protected:
        virtual std::string get_text_impl(const event& evt) const = 0;

    private:
        justification just_;
        std::size_t min_width_;
        std::size_t max_width_;
    };

    class CHUCHO_NO_EXPORT base_file_piece : public piece
    {
    public:
        base_file_piece(justification just,
                        std::size_t min_width,
                        std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT full_file_piece : public piece
    {
    public:
        full_file_piece(justification just,
                        std::size_t min_width,
                        std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT logger_piece : public piece
    {
    public:
        logger_piece(justification just,
                     std::size_t min_width,
                     std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT date_time_piece : public piece
    {
    public:
        date_time_piece(const std::string& date_pattern,
                        justification just,
                        std::size_t min_width,
                        std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
        virtual void to_calendar(time_t t, struct std::tm& cal) const = 0;

    private:
        std::string date_pattern_;
        std::vector<std::size_t> milli_positions_;
    };

    class CHUCHO_NO_EXPORT utc_date_time_piece : public date_time_piece
    {
    public:
        utc_date_time_piece(const std::string& date_pattern,
                            justification just,
                            std::size_t min_width,
                            std::size_t max_width);

    protected:
        virtual void to_calendar(time_t t, struct std::tm& cal) const override;
    };

    class CHUCHO_NO_EXPORT local_date_time_piece : public date_time_piece
    {
    public:
        local_date_time_piece(const std::string& date_pattern,
                              justification just,
                              std::size_t min_width,
                              std::size_t max_width);

    protected:
        virtual void to_calendar(time_t t, struct std::tm& cal) const override;
    };

    class CHUCHO_NO_EXPORT base_host_piece : public piece
    {
    public:
        base_host_piece(justification just,
                        std::size_t min_width,
                        std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;

    private:
        std::string name_;
    };

    class CHUCHO_NO_EXPORT line_number_piece : public piece
    {
    public:
        line_number_piece(justification just,
                          std::size_t min_width,
                          std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT message_piece : public piece
    {
    public:
        message_piece(justification just,
                      std::size_t min_width,
                      std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT function_piece : public piece
    {
    public:
        function_piece(justification just,
                       std::size_t min_width,
                       std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT end_of_line_piece : public piece
    {
    public:
        end_of_line_piece(justification just,
                          std::size_t min_width,
                          std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;

    private:
        std::string eol_;
    };

    class CHUCHO_NO_EXPORT level_piece : public piece
    {
    public:
        level_piece(justification just,
                    std::size_t min_width,
                    std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT milliseconds_since_start_piece : public piece
    {
    public:
        milliseconds_since_start_piece(justification just,
                                       std::size_t min_width,
                                       std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };

    class CHUCHO_NO_EXPORT pid_piece : public piece
    {
    public:
        pid_piece(justification just,
                  std::size_t min_width,
                  std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };
    
    class CHUCHO_NO_EXPORT literal_piece : public piece
    {
    public:
        literal_piece(const std::string& text);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;

    private:
        std::string text_;
    };

    class CHUCHO_NO_EXPORT thread_piece : public piece
    {
    public:
        thread_piece(justification just,
                     std::size_t min_width,
                     std::size_t max_width);

    protected:
        virtual std::string get_text_impl(const event& evt) const override;
    };
    
    CHUCHO_NO_EXPORT std::shared_ptr<piece> create_piece(std::string::const_iterator& pos,
                                                         std::string::const_iterator end,
                                                         justification just,
                                                         std::size_t min_width,
                                                         std::size_t max_width);
    CHUCHO_NO_EXPORT void parse(const std::string& pattern);
    CHUCHO_NO_EXPORT std::string get_argument(std::string::const_iterator& pos,
                                              std::string::const_iterator end);

    std::vector<std::shared_ptr<piece>> pieces_;
};

}

#endif
