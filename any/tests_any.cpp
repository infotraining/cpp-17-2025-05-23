#include <algorithm>
#include <any>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("any - safe void*")
{
    std::any anything = 42;
    anything = 3.14;
    anything = std::vector{1, 2, 3};
    anything = std::string("text");

    std::string str = std::any_cast<std::string>(anything);
    REQUIRE(str == "text");

    REQUIRE_THROWS_AS(std::any_cast<double>(anything), std::bad_any_cast);

    if (std::string* ptr_str = std::any_cast<std::string>(&anything); ptr_str)
    {
        std::cout << "String: " << *ptr_str << "\n";
    }
    else
    {
        std::cout << "Wrong type\n";
    }
}