/*
 * Copyright 2013 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#if !defined(CHUCHO_OPTIONAL_HPP__)
#define CHUCHO_OPTIONAL_HPP__

#include <chucho/export.hpp>
#include <type_traits>

namespace chucho
{

/**
 * @class optional optional.hpp chucho/optional.hpp
 * An object that might or might not be there. This is used as a
 * helper during configuration of chucho. 
 *  
 * @ingroup miscellaneous 
 */
template <typename type>
class CHUCHO_EXPORT optional
{
public:
    /**
     * @name Constructors and destructor
     */
    //@{
    /**
     * Make an optional that doesn't have an object.
     */
    optional();
    /**
     * Copy an object into a new optional.
     * 
     * @param val the object to copy
     */
    optional(const type& val);
    /**
     * Copy an optional object.
     * 
     * @param opt the optional to copy
     */
    optional(const optional<type>& opt);
    /**
     * Destroy an optional object.
     */
    ~optional();
    //@}

    /**
     * @name Operators
     */
    //@{
    /**
     * Copy an object into this optional.
     * 
     * @param val the value to copy
     * @return this optional
     */
    optional& operator= (const type& val);
    /**
     * Copy an optional object.
     * 
     * @param opt the optional to copy
     * @return this optional
     */
    optional& operator= (const optional<type>& opt);
    /**
     * Return whether this optional exists or not. If there is an 
     * object lurking inside this optional, then this method returns 
     * true. 
     * 
     * @return true if there is an object here
     */
    operator bool () const;
    /**
     * Return a reference to the underlying object.
     * 
     * @note The result is undefined if this optional does not
     *       actually have an object in it.
     * @return a reference to the optional object
     */
    type& operator* ();
    /**
     * Return a reference to the underlying object.
     * 
     * @note The result is undefined if this optional does not
     *       actually have an object in it.
     * @return a reference to the optional object
     */
    const type& operator* () const;
    /**
     * Return a pointer to the underlying object. You do not own 
     * this pointer. 
     * 
     * @note The result is undefined if this optional does not
     *       actually have an object in it.
     * @return a reference to the optional object
     */
    type* operator-> ();
    /**
     * Return a pointer to the underlying object. You do not own 
     * this pointer. 
     * 
     * @note The result is undefined if this optional does not
     *       actually have an object in it.
     * @return a reference to the optional object
     */
    const type* operator-> () const;
    //@}

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
