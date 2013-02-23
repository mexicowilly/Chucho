#include <gtest/gtest.h>
#include <chucho/status_manager.hpp>
#include <chucho/configuration.hpp>
#include <iostream>
#include <fstream>

class all_status : public chucho::status_observer
{
public:
    virtual void status_reported(const chucho::status& st) override
    {
        std::cout << st << std::endl;
    }
};

class global_fixture : public ::testing::Environment
{
public:
    virtual void SetUp() override
    {
        chucho::configuration::set_allow_default(false);
        observer_.reset(new all_status());
        chucho::status_manager::get()->add(observer_);
    }

private:
    std::shared_ptr<chucho::status_observer> observer_;
};

int main(int argc, char* argv[])
{
    if (argc == 3 && strcmp(argv[1], "--location") == 0)
    {
        std::ofstream stream(argv[2]);
        if (!stream.is_open())
            return EXIT_FAILURE;
        stream << argv[0];
        return EXIT_SUCCESS;
    }
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new global_fixture());
    return RUN_ALL_TESTS();
}
