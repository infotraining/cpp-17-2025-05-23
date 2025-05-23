#include <algorithm>
#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using namespace std::literals;

template <typename Container>
void print_all(const Container& container, std::string_view prefix)
{
    cout << prefix << ": [ ";
    for (const auto& item : container)
        cout << item << " ";
    cout << "]\n";
}

TEST_CASE("string_view")
{
    SECTION("const char* -> std::string_view")
    {
        const char* ctext = "Ala ma kota...";

        std::string_view text_sv = ctext;
        std::cout << text_sv << "\n";

        std::string_view token_sv{ctext + 7, 4};
        std::cout << token_sv << "\n";
    }

    SECTION("std::string -> std::string_view")
    {
        std::string str_text = "Ala ma kota...";

        std::string_view text_sv = str_text;
        std::cout << text_sv << "\n";

        std::string_view token_sv{str_text.data() + 7, 4};
        std::cout << token_sv << "\n";
    }

    SECTION("sv")
    {
        auto text = "text"sv;
    }

    SECTION("using string_view")
    {
        std::vector vec = {1, 2, 3};
        print_all(vec, "vec");
    }
}

TEST_CASE("string_view & lifetimes")
{
    std::string str_text = "Ala ma kota...";

    std::string_view text_sv = str_text;
    std::cout << text_sv << "\n";

    str_text += ".............."; 
    std::cout << text_sv << "\n"; // UB
}

TEST_CASE("string_view & null-terminated strings")
{
    const char tab[] = {'a', 'b', 'c'};
    std::string_view sv_text{tab, 3};
    REQUIRE(sv_text == "abc");

    std::cout << sv_text << "\n";

    SECTION("beware")
    {
        // fopen(sv_text.data(), "w+");  // ERROR - sv_text.data() is not null terminated
    }
}