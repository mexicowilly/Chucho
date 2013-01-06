#if !defined(CHUCHO_OPTIONAL_HPP__)
#define CHUCHO_OPTIONAL_HPP__

#include <chucho/export.hpp>
#include <type_traits>

namespace chucho
{

template <typename type>
class CHUCHO_EXPORT optional
{
public:
    optional();
    optional(const type& val);
    optional(const optional<type>& opt);
    ~optional();

    optional& operator= (const type& val);
    optional& operator= (const optional<type>& opt);
    operator bool () const;
    type& operator* ();
    const type& operator* () const;
    type* operator-> ();
    const type* operator-> () const;

private:
    CHUCHO_NO_EXPORT void construct(const type& val);
    CHUCHO_NO_EXPORT void destruct();

    typename std::aligned_storage<sizeof(type), std::alignment_of<type>::value>::type data_;
    bool initialized_;
};

template <typename type>
optional<type>::optional()
    : initialized_(false)
{
}

template <typename type>
optional<type>::optional(const type& val)
{
    construct(val);
}

template <typename type>
optional<type>::optional(const optional<type>& opt)
    : initialized_(opt.initialized_)
{
    if (initialized_)
        construct(*opt);
}

template <typename type>
optional<type>::~optional()
{
    destruct();
}

template <typename type>
optional<type>& optional<type>::operator= (const type& val)
{
    destruct();
    construct(val);
    return *this;
}

template <typename type>
optional<type>& optional<type>::operator= (const optional<type>& opt)
{
    destruct();
    initialized_ = opt.initialized_;
    if (initialized_)
        construct(*opt);
    return *this;
}

template <typename type>
inline optional<type>::operator bool () const
{
    return initialized_;
}

template <typename type>
inline type& optional<type>::operator* ()
{
    return reinterpret_cast<type&>(data_);
}

template <typename type>
const type& optional<type>::operator* () const
{
    return reinterpret_cast<const type&>(data_);
}

template <typename type>
inline type* optional<type>::operator-> ()
{
    return reinterpret_cast<type*>(&data_);
}

template <typename type>
inline const type* optional<type>::operator-> () const
{
    return reinterpret_cast<const type*>(&data_);
}

template <typename type>
void optional<type>::construct(const type& val)
{
    new (&data_) type(val);
    initialized_ = true;
}

template <typename type>
void optional<type>::destruct()
{
    if (initialized_)
    {
        operator*().~type();
        initialized_ = false;
    }
}

}

#endif
