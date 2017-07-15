/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_MARKER_HPP__)
#define CHUCHO_MARKER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/export.hpp>
#include <memory>
#include <string>
#include <set>
#include <ostream>

namespace chucho
{

/**
 * @class marker marker.hpp chucho/marker.hpp
 * A marker for customizing log messages. A marker is a 
 * hierarchical tag that one can place in a log message. Its 
 * sole attribute is its name. These can be used for 
 * differentiating log messages that originate from the same 
 * location, or for any other situation where distinguishing log 
 * messages is important. 
 *  
 * @ingroup miscellaneous 
 */
class CHUCHO_EXPORT marker
{
public:
    /**
     * An iterator for traversing the marker hierarchy.
     */
    typedef std::set<marker>::iterator iterator;
    /**
     * An iterator for traversing the marker hierarchy.
     */
    typedef std::set<marker>::const_iterator const_iterator;

    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a marker.
     * 
     * @param name the name of the marker
     */
    marker(const std::string& name);
    //@}

    /**
     * @name Operators 
     */
    //@{
    /**
     * Is this marker equal to another one? The comparison is by 
     * name only, not by any children of this marker. 
     *
     * @param mark the other marker
     * @return true if the two markers are equal
     */
    bool operator== (const marker& mark) const;
    /**
     * Is this marker less than another one? The comparison is by 
     * name only, not by any children of this marker. 
     *
     * @param mark the other marker
     * @return true if this marker is less than the other one
     */
    bool operator< (const marker& mark) const;
    //@}

    /**
     * Return the beginning. The first in the collection of children
     * is returned.
     *
     * @return the first child marker
     */
    iterator begin();
    /**
     * Return the beginning. The first in the collection of children
     * is returned.
     *
     * @return the first child marker
     */
    const_iterator begin() const;
    /**
     * Does this marker have any children?
     *
     * @return true if there are no children
     */
    bool empty() const;
    /**
     * Return the end. The returned iterator does not point to a 
     * marker.
     *
     * @return the first child marker
     */
    iterator end();
    /**
     * Return the end. The returned iterator does not point to a 
     * marker.
     *
     * @return the first child marker
     */
    const_iterator end() const;
    /**
     * Remove a child.
     * 
     * @param mark the marker to remove
     */
    void erase(iterator mark);
    /**
     * Return the name of this marker.
     * 
     * @return the name
     */
    const std::string& get_name() const;
    /**
     * Add a child.
     * 
     * @param mark the child to add
     */
    void insert(const marker& mark);

private:
    std::string name_;
    std::set<marker> children_;
};

/**
 * Write a marker to a stream. The entire contained hierarchy of 
 * markers is written, with children enclosed in square braces. 
 * 
 * @relates marker
 * @param stream the stream
 * @param mark the marker to write
 * @return the stream
 */
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

inline void marker::insert(const marker& mark)
{
    children_.insert(mark);
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
