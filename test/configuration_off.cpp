#include <chucho/configuration.hpp>
#include <chucho/log.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/pattern_formatter.hpp>

int main()
{
    chucho::configuration::set_style(chucho::configuration::style::OFF);
    auto lgr = chucho::logger::get("will");
    auto fmt = std::make_shared<chucho::pattern_formatter>("%m%n");
    auto wrt = std::make_shared<chucho::cout_writer>(fmt);
    lgr->add_writer(wrt);
    CHUCHO_INFO(lgr, "hello");
    return 0;
}
