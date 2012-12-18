#if !defined(CHUCHO_MARKER_HPP__)
#define CHUCHO_MARKER_HPP__

#include <chucho/export.hpp>
#include <memory>
#include <string>
#include <set>
#include <ostream>

namespace chucho
{

class CHUCHO_EXPORT marker
{
public:
    typedef std::set<std::shared_ptr<marker>, std::owner_less<std::shared_ptr<marker>>>::iterator iterator;
    typedef std::set<std::shared_ptr<marker>, std::owner_less<std::shared_ptr<marker>>>::const_iterator const_iterator;

    marker(const std::string& name);

    bool operator== (const marker& mark) const;
    bool operator< (const marker& mark) const;

    iterator begin();
    const_iterator begin() const;
    bool empty() const;
    iterator end();
    const_iterator end() const;
    void erase(iterator mark);
    const std::string& get_name() const;
    void insert(const std::shared_ptr<marker> mark);

private:
    std::string name_;
    std::set<std::shared_ptr<marker>, std::owner_less<std::shared_ptr<marker>>> children_;
};

CHUCHO_EXPORT std::ostream& operator<< (std::ostream& stream, const marker& mark);

inline bool marker::operator== (const marker& mark) const
{
    return name_ == mark.name_;
}

inline bool marker::operator< (const marker& mark) const
{
    return name_ < mark.name_;
}

inline marker::iterator marker::begin()
{
    return children_.begin();
}

inline marker::const_iterator marker::begin() const
{
    return children_.begin();
}

inline bool marker::empty() const
{
    return children_.empty();
}

inline marker::iterator marker::end()
{
    return children_.end();
}

inline marker::const_iterator marker::end() const
{
    return children_.end();
}

inline void marker::erase(iterator mark)
{
    children_.erase(mark);
}

inline const std::string& marker::get_name() const
{
    return name_;
}

inline void marker::insert(const std::shared_ptr<marker> mark)
{
    children_.insert(mark);
}

}

#endif
