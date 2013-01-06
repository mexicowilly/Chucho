#include <chucho/writer_factory.hpp>
#include <assert.h>

namespace chucho
{

void writer_factory::set_filters(std::shared_ptr<configurable> cnf, std::shared_ptr<writer_memento> mnto)
{
    auto wrt = std::dynamic_pointer_cast<writer>(cnf);
    assert(wrt);
    std::for_each(mnto->get_filters().begin(),
                  mnto->get_filters().end(),
                  [&] (std::shared_ptr<filter> f) { wrt->add_filter(f); });
}

}
