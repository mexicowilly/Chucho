/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_COMPRESSING_WRITER_HPP__)
#define CHUCHO_COMPRESSING_WRITER_HPP__

#include <chucho/writer.hpp>
#include <chucho/serializer.hpp>
#include <chucho/compressor.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class CHUCHO_EXPORT compressing_writer : public writer
{
public:
    compressing_writer(std::shared_ptr<formatter> fmt,
                       std::ostream& stream,
                       std::shared_ptr<compressor> cmp,
                       std::shared_ptr<serializer> ser,
                       const optional<std::size_t>& max_cache_in_kb,
                       const optional<std::size_t>& max_cached_events);
    ~compressing_writer();

    std::shared_ptr<compressor> get_compressor() const;
    std::size_t get_events_in_cache() const;
    const optional<std::size_t>& get_max_cached_bytes() const;
    const optional<std::size_t>& get_max_cached_events() const;
    std::shared_ptr<serializer> get_serializer() const;
    std::ostream& get_stream() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT void cache(const event& evt);
    CHUCHO_NO_EXPORT void flush();
    CHUCHO_NO_EXPORT std::uint32_t to_network(std::uint32_t num) const;

    std::ostream& stream_;
    std::vector<std::uint8_t> compressed_cache_;
    std::size_t events_in_cache_;
    optional<std::size_t> max_cached_;
    optional<std::size_t> max_bytes_;
    std::shared_ptr<serializer> serializer_;
    std::shared_ptr<compressor> compressor_;
};

inline std::shared_ptr<compressor> compressing_writer::get_compressor() const
{
    return compressor_;
}

inline std::size_t compressing_writer::get_events_in_cache() const
{
    return events_in_cache_;
}

inline const optional<std::size_t>& compressing_writer::get_max_cached_bytes() const
{
    return max_bytes_;
}

inline const optional<std::size_t>& compressing_writer::get_max_cached_events() const
{
    return max_cached_;
}

inline std::shared_ptr<serializer> compressing_writer::get_serializer() const
{
    return serializer_;
}

inline std::ostream& compressing_writer::get_stream() const
{
    return stream_;
}

}

#endif