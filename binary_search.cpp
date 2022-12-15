#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

using Binary_Search_Func = std::function<int(const std::vector<int>&, int)>;
using Test_Data = std::vector<std::tuple<int, int, std::vector<int>>>;

bool BS_MASTER_SAYS_YOU_PASSED = true;

const std::vector<std::tuple<int, int, std::vector<int>>> test_data__find_any = {
    {5,     -1,     {}},
    {5,     -1,     {1}},
    {5,      0,     {5}},
    {5,      0,     {5, 6}},
    {5,      1,     {0, 5}},
    {5,      0,     {5, 7, 9}},
    {5,      2,     {0, 0, 5}},
    {9,      9,     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
    {5,      5,     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
    {0,      0,     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
    {35,    -1,     {1, 1, 2, 3, 5, 8, 13, 21, 34}},
    {2,      2,     {1, 1, 2, 3, 5, 8, 13, 21, 34}},
    {5,      4,     {1, 1, 2, 3, 5, 8, 13, 21, 34}},
    {99,    -1,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540}},
    {100,   12,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540}},
    {7,      3,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540}},
    {1240,  15,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540}},
    {1540,  16,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540}},
    {2000,  -1,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540}},
};

const std::vector<std::tuple<int, int, std::vector<int>>> test_data__find_left = {
    {5,      0,     {5, 5}},
    {5,      0,     {5, 5, 6}},
    {6,      2,     {5, 5, 6, 6}},
    {5,      0,     {5, 5, 5, 6}},
    {5,      1,     {0, 5, 5}},
    {5,      1,     {0, 5, 5, 5}},
    {5,      1,     {0, 5, 5, 5, 5}},
    {7,      1,     {5, 7, 7, 9}},
    {5,      2,     {0, 0, 5, 5, 5}},
    {9,      9,     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9}},
    {5,      5,     {0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9}},
    {0,      0,     {0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
    {2,      2,     {1, 1, 2, 2, 3, 5, 8, 13, 21, 34}},
    {5,      4,     {1, 1, 2, 3, 5, 5, 5, 5, 5, 5, 8, 13, 21, 34}},
    {100,   12,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 100, 100, 200, 1230, 1240, 1540}},
    {7,      3,     {1, 3, 5, 7, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540}},
    {1240,  15,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1240, 1240, 1240, 1540}},
    {1540,  16,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540, 1540}},
    {1540,  16,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540, 1540, 1540}},
};

const std::vector<std::tuple<int, int, std::vector<int>>> test_data__find_right = {
    {5,      1,     {5, 5}},
    {5,      1,     {5, 5, 6}},
    {6,      3,     {5, 5, 6, 6}},
    {5,      2,     {5, 5, 5, 6}},
    {5,      2,     {0, 5, 5}},
    {5,      3,     {0, 5, 5, 5}},
    {5,      4,     {0, 5, 5, 5, 5}},
    {7,      2,     {5, 7, 7, 9}},
    {5,      4,     {0, 0, 5, 5, 5}},
    {9,     11,     {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9}},
    {5,      6,     {0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 9}},
    {0,      4,     {0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
    {2,      3,     {1, 1, 2, 2, 3, 5, 8, 13, 21, 34}},
    {5,      9,     {1, 1, 2, 3, 5, 5, 5, 5, 5, 5, 8, 13, 21, 34}},
    {100,   14,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 100, 100, 200, 1230, 1240, 1540}},
    {7,      4,     {1, 3, 5, 7, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540}},
    {1240,  18,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1240, 1240, 1240, 1540}},
    {1540,  17,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540, 1540}},
    {1540,  18,     {1, 3, 5, 7, 10, 11, 12, 13, 14, 50, 70, 80, 100, 200, 1230, 1240, 1540, 1540, 1540}},
};

bool test(Binary_Search_Func bs_func, const std::vector<int>& v, int value_to_find, int expected_result)
{
    int index = bs_func(v, value_to_find);
    if (index == expected_result)
        return true;

    std::cout << "FAILURE:\n";
    std::cout << "  data: [" << v.size() << "] (";
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i];
        if (i != v.size() - 1)
            std::cout << ", ";
    }
    std::cout << ")\n";
    std::cout
        << "  value_to_find = " << value_to_find
        << ", expected_result = " << expected_result
        << ", actual_result = " << index << "\n\n";
    return false;
}

void test_binary_search_func(Binary_Search_Func bs_func, const char* func_name, const Test_Data& test_data)
{
    std::cout << "Testing " << func_name << "\n";
    for (const auto& [value_to_find, expected_result, data] : test_data) {
        if (!test(bs_func, data, value_to_find, expected_result)) {
            BS_MASTER_SAYS_YOU_PASSED = false;
            return;
        }
    }
    std::cout << "SUCCESS\n\n";
}

#define TEST_FIND_ANY(f) test_binary_search_func(f, #f, test_data__find_any)
#define TEST_FIND_LEFT(f) test_binary_search_func(f, #f, test_data__find_left)
#define TEST_FIND_RIGHT(f) test_binary_search_func(f, #f, test_data__find_right)

int bs_find_any_1(const std::vector<int>& data, int value)
{
    if (data.empty())
        return -1;

    size_t left = 0;
    size_t right = data.size() - 1;

    while (left <= right) {
        size_t middle = (left + right) / 2;
        if (data[middle] > value)
            right = middle - 1;
        else if (data[middle] < value)
            left = middle + 1;
        else
            return int(middle);
    }
    return -1;
}

int bs_find_any_2(const std::vector<int>& data, int value)
{
    size_t left = 0;
    size_t right = data.size();

    while (left < right) {
        size_t middle = (left + right) / 2;
        if (data[middle] > value)
            right = middle;
        else if (data[middle] < value)
            left = middle + 1;
        else
            return int(middle);
    }
    return -1;
}

int bs_find_any_3(const std::vector<int>& data, int value)
{
    auto left = data.cbegin();
    auto right = data.cend();

    while (left < right) {
        auto middle = left + (right - left) / 2;
        if (*middle > value)
            right = middle;
        else if (*middle < value)
            left = middle + 1;
        else
            return int(middle - data.cbegin());
    }
    return -1;
}

int bs_find_left_1(const std::vector<int>& data, int value)
{
    if (data.empty())
        return -1;

    size_t left = 0;
    size_t right = data.size() - 1;

    while (left < right) {
        size_t middle = (left + right) / 2;
        if (data[middle] >= value)
            right = middle;
        else
            left = middle + 1;
    }
    return  data[left] == value ? int(left) : -1;
}

int bs_find_left_2(const std::vector<int>& data, int value)
{
    size_t left = 0;
    size_t right = data.size();

    while (left < right) {
        size_t middle = (left + right) / 2;
        if (data[middle] >= value)
            right = middle;
        else
            left = middle + 1;
    }
    return (left != data.size() && data[left] == value) ? int(left) : -1;
}

int bs_find_right_1(const std::vector<int>& data, int value)
{
    size_t left = 0;
    size_t right = data.size();

    while (left < right) {
        size_t middle = (left + right) / 2;
        if (data[middle] > value)
            right = middle;
        else
            left = middle + 1;
    }
    return (right != 0 && data[right - 1] == value) ? int(right) - 1 : -1;
}

int main()
{
    std::cout << "============ Welcome to BS Master ==============\n";

    for (const auto& [_, __, data] : test_data__find_any)
        assert(std::is_sorted(data.begin(), data.end()));
    for (const auto& [_, __, data] : test_data__find_left)
        assert(std::is_sorted(data.begin(), data.end()));
    for (const auto& [_, __, data] : test_data__find_right)
        assert(std::is_sorted(data.begin(), data.end()));

    bool PASSED = true;

    TEST_FIND_ANY(bs_find_any_1);
    TEST_FIND_ANY(bs_find_any_2);
    TEST_FIND_ANY(bs_find_any_3);
    TEST_FIND_ANY(bs_find_left_1);
    TEST_FIND_ANY(bs_find_left_2);
    TEST_FIND_ANY(bs_find_right_1);

    TEST_FIND_LEFT(bs_find_left_1);
    TEST_FIND_LEFT(bs_find_left_2);

    TEST_FIND_RIGHT(bs_find_right_1);

    std::cout << "BS Master says you " << (BS_MASTER_SAYS_YOU_PASSED ? "PASSED\n" : "FAILED MISERABLY\n");
}
