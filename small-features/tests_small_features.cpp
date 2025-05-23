#include "gadget.hpp"

#include <algorithm>
#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("static inline")
{
    Gadget g1;
    REQUIRE(g1.id() == 1);

    Gadget g2;
    REQUIRE(g2.id() == 2);
}

struct ErrorCode
{
    int code;
    std::string msg;
};

[[nodiscard]] ErrorCode open_file(const std::string& file_name)
{
    if (file_name == "unknown")
        return ErrorCode {13, "File not found"};
    
    //...
    return {0, ""};
}

int capi_fun()
{
    //...
    return 0;
}

TEST_CASE("attributes")
{
    if (auto [err_code, err_message] = open_file("unknown"); err_code != 0)
    {
        std::cout << "Error!!! " << err_message << "!!!\n"; 
    }

    [[maybe_unused]] int status = capi_fun();
    assert(status == 0);
}