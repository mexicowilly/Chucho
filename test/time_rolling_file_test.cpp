#include <gtest/gtest.h>
#include <chucho/rolling_file_writer.hpp>
#include <chucho/time_file_roller.hpp>
#include <chucho/logger.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/file.hpp>
#include <chucho/calendar.hpp>
#include <chrono>
#include <vector>
#include <thread>
#include <iostream>
#include <queue>

namespace
{

class test
{
public:
    template <typename duration_type>
    test(const std::string& pattern,
         std::size_t max_history,
         const duration_type& dur);
    template <typename duration_type>
    test(const std::string& active,
         const std::string& pattern,
         std::size_t max_history,
         const duration_type& dur);
    virtual ~test();

    bool operator< (const test& tst) const;

    void check();
    const std::chrono::system_clock::time_point& end() const;
    void sleep();

protected:
    static std::string TOP_LEVEL_DIR;

    virtual void check_impl() = 0;
    std::deque<std::string>& expected_file_names();
    std::string format_file_name(const std::string& pattern);
    std::chrono::system_clock::time_point& next();
    std::vector<std::string>& unexpected_file_names();
    void write();

private:
    chucho::rolling_file_writer writer_;
    std::chrono::system_clock::time_point next_;
    std::deque<std::string> expected_file_names_;
    std::vector<std::string> unexpected_file_names_;
    std::chrono::system_clock::time_point end_;
};

class date_only_test : public test
{
public:
    template <typename duration_type, typename next_type>
    date_only_test(const std::string& pattern,
                   std::size_t max_history,
                   const duration_type& dur,
                   const next_type& nxt);

protected:
    virtual void check_impl() override;

private:
    std::chrono::seconds until_next_;
    std::string pattern_;
    std::size_t max_history_;
};

class fixed_active_test : public test
{
public:
    template <typename duration_type, typename next_type>
    fixed_active_test(const std::string& active,
                      const std::string& pattern,
                      std::size_t max_history,
                      const duration_type& dur,
                      const next_type& nxt);

protected:
    virtual void check_impl() override;

private:
    std::chrono::seconds until_next_;
    std::string active_;
    std::string pattern_;
    std::size_t max_history_;
};

class minute_test : public date_only_test
{
public:
    minute_test();
};

class active_minute_test : public fixed_active_test
{
public:
    active_minute_test();
};

class hour_test : public date_only_test
{
public:
    hour_test();
};

class active_hour_test : public fixed_active_test
{
public:
    active_hour_test();
};

std::string test::TOP_LEVEL_DIR("time_rolling_file_test/");

template <typename duration_type>
test::test(const std::string& pattern,
           std::size_t max_history,
           const duration_type& dur)
    : writer_(std::shared_ptr<chucho::formatter>(new chucho::pattern_formatter("%m%n")),
              std::shared_ptr<chucho::file_roller>(new chucho::time_file_roller(TOP_LEVEL_DIR + pattern, max_history))),
      next_(std::chrono::system_clock::now()),
      end_(std::chrono::system_clock::now() + dur)
{
    write();
}

template <typename duration_type>
test::test(const std::string& active,
           const std::string& pattern,
           std::size_t max_history,
           const duration_type& dur)
    : writer_(std::shared_ptr<chucho::formatter>(new chucho::pattern_formatter("%m%n")),
              TOP_LEVEL_DIR + active,
              std::shared_ptr<chucho::file_roller>(new chucho::time_file_roller(TOP_LEVEL_DIR + pattern, max_history))),
      next_(std::chrono::system_clock::now()),
      end_(std::chrono::system_clock::now() + dur)
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

const std::chrono::system_clock::time_point& test::end() const
{
    return end_;
}

std::deque<std::string>& test::expected_file_names()
{
    return expected_file_names_;
}

std::string test::format_file_name(const std::string& pattern)
{
    struct std::tm cal = chucho::calendar::get_utc(std::time(nullptr));
    std::ostringstream stream;
    stream << TOP_LEVEL_DIR << std::put_time(&cal, pattern.c_str());
    return stream.str();
}

std::chrono::system_clock::time_point& test::next()
{
    return next_;
}

void test::sleep()
{
    std::this_thread::sleep_until(next_);
}

std::vector<std::string>& test::unexpected_file_names()
{
    return unexpected_file_names_;
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

bool file_doesnt_exist(const std::string& file_name)
{
    return !chucho::file::exists(file_name);
}

template <typename duration_type, typename next_type>
date_only_test::date_only_test(const std::string& pattern,
                               std::size_t max_history,
                               const duration_type& dur,
                               const next_type& nxt)
    : test(pattern, max_history, dur),
      until_next_(std::chrono::duration_cast<std::chrono::seconds>(nxt)),
      pattern_(pattern),
      max_history_(max_history)
{
    std::size_t pos = pattern_.find("%d{");
    if (pos != std::string::npos)
        pattern_.erase(pos, 3);
    pos = pattern_.find("}");
    if (pos != std::string::npos)
        pattern_.erase(pos, 1);
    expected_file_names().push_back(format_file_name(pattern_));
    next() += until_next_;
}

void date_only_test::check_impl()
{
    expected_file_names().push_back(format_file_name(pattern_));
    if (expected_file_names().size() > max_history_ + 1)
    {
        unexpected_file_names().push_back(expected_file_names().front());
        expected_file_names().pop_front();
    }
    for (auto f : expected_file_names())
        EXPECT_PRED1(chucho::file::exists, f);
    for (auto f : unexpected_file_names())
        EXPECT_PRED1(file_doesnt_exist, f);
    next() += until_next_;
}

template <typename duration_type, typename next_type>
fixed_active_test::fixed_active_test(const std::string& active,
                                     const std::string& pattern,
                                     std::size_t max_history,
                                     const duration_type& dur,
                                     const next_type& nxt)
    : test(active, pattern, max_history, dur),
      until_next_(std::chrono::duration_cast<std::chrono::seconds>(nxt)),
      active_(active),
      pattern_(pattern),
      max_history_(max_history)
{
    std::size_t pos = pattern_.find("%d{");
    if (pos != std::string::npos)
        pattern_.erase(pos, 3);
    pos = pattern_.find("}");
    if (pos != std::string::npos)
        pattern_.erase(pos, 1);
    next() += until_next_;
}

void fixed_active_test::check_impl()
{
    expected_file_names().push_back(format_file_name(pattern_));
    if (expected_file_names().size() > max_history_)
    {
        unexpected_file_names().push_back(expected_file_names().front());
        expected_file_names().pop_front();
    }
    EXPECT_PRED1(chucho::file::exists, TOP_LEVEL_DIR + active_);
    for (auto f : expected_file_names())
        EXPECT_PRED1(chucho::file::exists, f);
    for (auto f : unexpected_file_names())
        EXPECT_PRED1(file_doesnt_exist, f);
    next() += until_next_;
}

minute_test::minute_test()
    : date_only_test("minute/test-%d{%H-%M}",
                     2,
                     std::chrono::minutes(5),
                     std::chrono::seconds(61))
{
}

active_minute_test::active_minute_test()
    : fixed_active_test("active-minute/active",
                        "active-minute/test-%d{%H-%M}",
                        2,
                        std::chrono::minutes(5),
                        std::chrono::seconds(62))
{
}

hour_test::hour_test()
    : date_only_test("hour/test-%d{%d-%H}",
                     2,
                     std::chrono::hours(5),
                     std::chrono::minutes(61))
{
}

active_hour_test::active_hour_test()
    : fixed_active_test("active-hour/active",
                        "active-hour/test-%d{%d-%H}",
                        2,
                        std::chrono::hours(5),
                        std::chrono::minutes(62))
{
}

void run(std::shared_ptr<test> tst)
{
    while (std::chrono::system_clock::now() < tst->end())
    {
        tst->sleep();
        tst->check();
    }
}

}

TEST(time_rolling_file_test, all)
{
    chucho::file::remove_all("time_rolling_file_test");
    std::vector<std::shared_ptr<std::thread>> threads =
    {
        std::make_shared<std::thread>(run, std::make_shared<minute_test>()),
        std::make_shared<std::thread>(run, std::make_shared<active_minute_test>()),
        std::make_shared<std::thread>(run, std::make_shared<hour_test>()),
        std::make_shared<std::thread>(run, std::make_shared<active_hour_test>())
    };
    for (auto thrd : threads)
        thrd->join();
}
