#include <algorithm>
#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace std::literals;

namespace ReturnAuto
{
    auto describe(int n)
    {
        // if (n % 2 == 0)
        // {
        //     return std::string{"even"};
        // }

        // return "odd";

        return (n % 2 == 0) ? "even"s : "odd";
    }

    template <size_t N, typename T = uint8_t>
    constexpr auto create_buffer()
    {
        if constexpr (N <= 1024)
        {
            std::array<T, N> buffer{};
            return buffer;
        }
        else
        {
            std::vector<T> buffer(N);
            return buffer;
        }
    }
}

TEST_CASE("returning auto")
{
    using namespace ReturnAuto;

    auto result1 = describe(4);
    REQUIRE(result1 == "even"s);

    auto result2 = describe(7);
    REQUIRE(result2 == "odd"s);

    SECTION("returning different types")
    {
        auto buffer1 = create_buffer<512>();
        auto buffer2 = create_buffer<1'000'000>();

        for (auto& item : buffer1)
            item = 1;
    }
}

TEST_CASE("tuple")
{
    std::tuple<int, double, std::string> tpl1(42, 3.14, "text");

    REQUIRE(std::get<0>(tpl1) == 42);
    REQUIRE(std::get<1>(tpl1) == 3.14);
    REQUIRE(std::get<2>(tpl1) == "text"s);
    // REQUIRE(std::get<3>(tpl1) == "text"s);

    std::tuple<int, double, std::string> tpl2;

    tpl2 = tpl1;
    REQUIRE(tpl1 == tpl2);

    // std::tuple<int, double, std::string> tpl3(665, 2.71, "another text");
    auto tpl3 = std::make_tuple(665, 2.71, "another text"s);

    // since C++17
    std::tuple tpl4(665, 2.71, "another text"s);
    REQUIRE(tpl3 == tpl4);

    SECTION("ref tuples")
    {
        int x = 42;
        double math_constant = 3.14;
        std::string text = "hello";

        std::tuple<int&, double&, std::string&> ref_tpl(x, math_constant, text);
        auto tied = std::tie(x, math_constant, text);
        static_assert(std::is_same_v<decltype(ref_tpl), decltype(tied)>);

        REQUIRE(std::get<0>(ref_tpl) == 42);
        std::get<0>(ref_tpl) = 665;
        REQUIRE(x == 665);

        ref_tpl = std::tuple{42, 2.71, "text"}; // assignment to ref_tpl
        REQUIRE(x == 42);
        REQUIRE(math_constant == 2.71);
        REQUIRE(text == "text"s);

        std::tie(x, math_constant, text) = std::tuple{1, 1.11, "one"}; // std::tie creates tuple of refs
        REQUIRE(x == 1);
        REQUIRE(math_constant == 1.11);
        REQUIRE(text == "one"s);
    }
}

namespace [[deprecated]] BeforeCpp17
{
    tuple<int, int, double> calc_stats(const vector<int>& data)
    {
        vector<int>::const_iterator min_pos, max_pos;
        tie(min_pos, max_pos) = minmax_element(data.begin(), data.end());

        double avg = accumulate(data.begin(), data.end(), 0.0) / data.size();

        return make_tuple(*min_pos, *max_pos, avg);
    }
}

tuple<int, int, double> calc_stats(const vector<int>& data)
{
    const auto [min_pos, max_pos] = minmax_element(data.begin(), data.end());

    double avg = accumulate(data.begin(), data.end(), 0.0) / data.size();

    return {*min_pos, *max_pos, avg};
}

struct SimpleData
{
    int a;
    std::string b;
};

TEST_CASE("structured bindings")
{
    SECTION("for tuples")
    {
        std::tuple tpl{42, 3.14, "text"};

        auto [x, pi, message] = tpl;
    }

    SECTION("for native arrays")
    {
        int coord[3] = {1, 2, 3};
        
        auto [x, y, z] = coord;
        
        REQUIRE(x == 1);
        REQUIRE(y == 2);
        REQUIRE(z == 3);
    }

    SECTION("std::array")
    {
        std::array<int, 2> coord2d{1, 2};

        auto [x, y] = coord2d;

        REQUIRE(x == 1);
        REQUIRE(y == 2);
    }

    SECTION("std::pair")
    {
        std::map<std::string, int> dict = { {"one", 1}, {"two", 2} };

        if (auto [pos, was_inserted] = dict.insert(std::pair("three", 3)); was_inserted)
        {
            std::cout << pos->first << " was inserted to map...\n";
        }
        else
        {
            std::cout << pos->first << " is already in map...\n";
        }
    }

    SECTION("struct")
    {
        SimpleData data{1, "one"};
        auto [value, description] = data;

        REQUIRE(value == 1);
        REQUIRE(description == "one");
    }
}

TEST_CASE("calc_stats")
{
    SECTION("Before C++17")
    {
        vector<int> data = {4, 42, 665, 1, 123, 13};

        int min, max;
        double avg;

        tie(min, max, avg) = BeforeCpp17::calc_stats(data);

        REQUIRE(min == 1);
        REQUIRE(max == 665);
        REQUIRE(avg == Catch::Approx(141.333));
    }

    SECTION("Since C++17")
    {
        vector<int> data = {4, 42, 665, 1, 123, 13};

        const auto [min, max, avg] = BeforeCpp17::calc_stats(data); // structured bindings
        
        SECTION("is interpreted as")
        {
            const auto entity = BeforeCpp17::calc_stats(data);
            auto&& min = std::get<0>(entity);
            auto&& max = std::get<1>(entity);
            auto&& avg = std::get<2>(entity);
        }

        REQUIRE(min == 1);
        REQUIRE(max == 665);
        REQUIRE(avg == Catch::Approx(141.333));
    }
}

TEST_CASE("standard use case - structured bindings")
{
    std::map<std::string, int> dict = { {"one", 1}, {"two", 2} };

    for (const auto& [key, value] : dict)
    {
        std::cout << key << " - " << value << "\n";
    }
}