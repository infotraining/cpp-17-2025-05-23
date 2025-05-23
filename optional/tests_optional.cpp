#include <algorithm>
#include <array>
#include <atomic>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

using namespace std;

TEST_CASE("optional")
{
    SECTION("pointers")
    {
        int x = 10;
        
        int* ptr = nullptr;
        
        ptr = &x;
        std::cout << *ptr << "\n";
    }

    SECTION("null semantics for value types")
    {
        std::optional<int> opt_int; 
        REQUIRE(opt_int.has_value() == false);

        opt_int = 42;
        REQUIRE(opt_int.has_value() == true);

        REQUIRE(*opt_int == 42);

        if (opt_int)
        {
            std::cout << "optional holds value: " << *opt_int << "\n";
        }

        REQUIRE(opt_int.value() == 42);
        opt_int = std::nullopt;
        REQUIRE_THROWS_AS(opt_int.value(), std::bad_optional_access);
    }
}

using User = std::string;

[[nodiscard]] std::optional<User> find_user(const std::vector<User>& users_db, const User& user)
{
    if (auto pos = std::find(users_db.begin(), users_db.end(), user); pos != users_db.end())
    {
        return user;
    }

    return std::nullopt;
}

TEST_CASE("using optional")
{
    std::vector<User> users = { "Ala", "Janek", "Tomek" };

    std::optional<User> user_tomek = find_user(users, "Tomek");

    if (user_tomek)
    {
        std::cout << "User: " << *user_tomek << "\n";
    }
    else
    {
        std::cout << "User not found...\n";
    }

    std::cout << "User " << user_tomek.value_or("not found...\n"s) << "\n";
}