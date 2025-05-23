#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>
#include <memory>
#include <functional>

using namespace std;
using namespace std::literals;

template <typename T1, typename T2>
struct ValuePair
{
    T1 fst;
    T2 snd;

    ValuePair(T1 f, T2 s) : fst{std::move(f)}, snd{std::move(s)}
    {}
};

template <typename T1, typename T2>
ValuePair<T1, T2> make_value_pair(T1 fst, T2 snd)
{
    return ValuePair<T1, T2>(std::move(fst), std::move(snd));
}

int foo(int, int)
{
    return 42;
}

TEST_CASE("CTAD - Class Template Arg Deduction")
{
    ValuePair<int, double> vp1{42, 3.14};

    auto vp2 = make_value_pair(42, 3.14);

    ValuePair vp3{42, 3.14};  // CTAD

    std::pair my_pair{42, 3.14};

    std::tuple my_tuple{4, 24.444, "text"s};

    std::vector vec = {1, 2, 3};

    std::unique_ptr<int> my_ptr{new int(10)};

    std::shared_ptr my_shared_ptr(std::move(my_ptr));

    std::function f = foo;
}

template <typename T1, typename T2>
struct AggregateValuePair
{
    T1 fst;
    T2 snd;
};

// deductions guide
template <typename T1, typename T2>
AggregateValuePair(T1, T2) -> AggregateValuePair<T1, T2>;


TEST_CASE("Aggregate + CTAD")
{
    AggregateValuePair vp1{42, 3.24};
}