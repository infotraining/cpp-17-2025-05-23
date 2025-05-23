#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

using namespace std;
using namespace std::literals;

TEST_CASE("variant")
{
    //std::variant<int /* 0 */, double /* 1 */, std::string /* 2 */> var1 = 42;
    std::variant<int, double, std::string> var1 = 42;
    var1 = 3.14;
    var1 = "text"s;

    REQUIRE(var1.index() == 2);

    REQUIRE(std::holds_alternative<std::string>(var1) == true);
    REQUIRE(std::holds_alternative<double>(var1) == false);

    std::string str = std::get<std::string>(var1);
    REQUIRE_THROWS_AS(std::get<double>(var1), std::bad_variant_access);
}

struct Printer
{
    void operator()(int x) { std::cout << "int: " << x << "\n"; }
    void operator()(double x) { std::cout << "double: " << x << "\n"; }
    void operator()(const std::string& s) { std::cout << "string: " << s << "\n"; }
};

TEST_CASE("visiting variants")
{
    std::variant<int, double, std::string> var1 = 42;
    var1 = 3.14;
    var1 = "text"s;

    std::visit(Printer{}, var1);
}