#include <gtest/gtest.h>
#include <chucho/utf8.hpp>
#include <chucho/exception.hpp>
#include <iostream>
#include <sstream>

//
// If Visual Studio 2012 supported initializer lists, this code
// would not be hideous.
//

TEST(utf8, escape)
{
    struct { const char* first; const char* second; } v[] =
    {
        { "wi\x80ll", "wi\\x80ll" },
        { "wi\x80\x81\x82ll", "wi\\x80\\x81\\x82ll" },
        { "wi\xf5ll", "wi\\xf5ll" },
        { "wi\xc5ll", "wi\\xc5ll" },
        { "wi\xe0ll", "wi\\xe0ll" },
        { "wi\xe0\x80ll", "wi\\xe0\\x80ll" },
        { "wi\xf0ll", "wi\\xf0ll" },
        { "wi\xf1\x81ll", "wi\\xf1\\x81ll" },
        { "wi\xf2\x82\x83ll", "wi\\xf2\\x82\\x83ll" },
        { "\xf3will", "\\xf3will" },
        { "will\x8f", "will\\x8f" },
        { nullptr, nullptr }
    };
    int i = 0;
    while (v[i].first != nullptr)
        EXPECT_EQ(v[i].second, chucho::utf8::escape_invalid(v[i++].first));
}

TEST(utf8, invalid)
{
    const char* v[] =
    {
        "\x80",
        "\xf5",
        "\xc1\x7f",
        "\xe9\x80\xff",
        "\xf1\xa9\xc0\x80",
        "\xc2",
        "\xea\x80",
        "\xf2\x80",
        "\xf3\xa0\xb0",
        nullptr
    };
    int i = 0;
    while (v[i] != nullptr)
    {
        std::cout << "Check " << v[i] << std::endl;
        std::istringstream stream(v[i++]);
        EXPECT_THROW(chucho::utf8::validate(stream), chucho::exception);
    }
}

TEST(utf8, valid)
{
    const char* v[] =
    {
        "I know you are, but what am I",
        "\xc0\xb0, but what am I",
        "I know you are, but \xdf\xa1",
        "\xe0\x80\x81, but what am I",
        "I know you are, but \xef\xbe\xbf",
        "\xf0\x80\x81\x82, but what am I",
        "I know you are, but \xf4\xbd\xbe\xbf",
        nullptr 
    };
    int i = 0;
    while (v[i] != nullptr)
    {
        std::cout << "Check " << v[i] << std::endl;
        std::istringstream stream(v[i++]);
        EXPECT_NO_THROW(chucho::utf8::validate(stream));
    }
}
