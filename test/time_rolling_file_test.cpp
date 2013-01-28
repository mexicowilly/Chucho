#include <gtest/gtest.h>
#include <chucho/rolling_file_writer.hpp>
#include <chucho/time_file_roller.hpp>
#include <chucho/logger.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/file.hpp>
#include <chrono>
#include <queue>
#include <thread>

namespace
{

class test
{
public:
    test(const std::string& pattern, std::size_t max_history);
    virtual ~test();

    bool operator< (const test& tst) const;

    void check();
    void sleep();

protected:
    virtual void check_impl() = 0;
    std::chrono::system_clock::time_point& next();
    void write();

private:
    chucho::rolling_file_writer writer_;
    std::chrono::system_clock::time_point next_;
};

class minute_test : public test
{
public:
    minute_test();

protected:
    virtual void check_impl() override;
};

test::test(const std::string& pattern, std::size_t max_history)
    : writer_(std::shared_ptr<chucho::formatter>(new chucho::pattern_formatter("%m%n")),
              std::shared_ptr<chucho::file_roller>(new chucho::time_file_roller("time_rolling_file_test/" + pattern, max_history))),
      next_(std::chrono::system_clock::now())
{
    write();
}

test::~test()
{
}

bool test::operator< (const test& tst) const
{
    return next_ < tst.next_;
}

void test::check()
{
    write();
    check_impl();
}

std::chrono::system_clock::time_point& test::next()
{
    return next_;
}

void test::sleep()
{
    std::this_thread::sleep_until(next_);
}

void test::write()
{
    chucho::event evt(chucho::logger::get("time_rolling_file_test"),
                      chucho::INFO_LEVEL,
                      "hello",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    writer_.write(evt);
}

minute_test::minute_test()
    : test("minute/test-%d{%H-%M}", 2)
{
    next() += std::chrono::seconds(61);
}

void minute_test::check_impl()
{
    SCOPED_TRACE("minute_test");
    next() += std::chrono::seconds(61);
}

}

TEST(time_rolling_file_test, all)
{
    chucho::file::remove_all("time_rolling_file_test");
    std::priority_queue<std::shared_ptr<test>,
                        std::vector<std::shared_ptr<test>>,
                        std::owner_less<std::shared_ptr<test>>> tests;
    tests.push(std::make_shared<minute_test>());
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now() + std::chrono::minutes(5);
    while (std::chrono::system_clock::now() < end)
    {
        std::shared_ptr<test> cur = tests.top();
        tests.pop();
        cur->sleep();
        cur->check();
        tests.push(cur);
    }
}
