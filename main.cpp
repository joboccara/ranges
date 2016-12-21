#include "adjacent.hpp"
#include "cartesian_product.hpp"
#include "transform.hpp"
#include "zip.hpp"

#include "algorithm.hpp"
#include "variadic.hpp"

#include <iostream>
#include <sstream>
#include <vector>

namespace
{

int multiplyBy2(int i)
{
    return i * 2;
}

bool testTransform()
{
	std::vector<int> input = {1, 2, 3, 4, 5};
    std::vector<int> expected = { 2, 4, 6, 8, 10 };
    std::vector<int> result;

	auto doubledNumbers = input | ranges::view::transform(multiplyBy2);
	push_back(result, doubledNumbers);

    return result == expected;
}

std::string letterPlusNumber(char line, int column)
{
    std::ostringstream position;
    position << line << column;
    return position.str();
}

bool testZip()
{
	std::vector<char> input1 = {'A', 'B', 'C', 'D', 'E'};
	std::vector<int> input2 = {1, 2, 3, 4, 5};
    std::vector<std::string> expected = { "A1", "B2", "C3", "D4", "E5" };
    std::vector<std::string> result;

	push_back(result, ranges::view::zip(input1, input2) | ranges::view::transform(tupled_args(letterPlusNumber)));

    return result == expected;
}

bool testCartesianProduct()
{
    std::vector<char> input1 = {'A', 'B', 'C', 'D', 'E'};
    std::vector<int> input2 = {1, 2, 3, 4, 5};
    std::vector<std::string> expected = { "A1", "A2", "A3", "A4", "A5",
                                          "B1", "B2", "B3", "B4", "B5",
                                          "C1", "C2", "C3", "C4", "C5",
                                          "D1", "D2", "D3", "D4", "D5",
                                          "E1", "E2", "E3", "E4", "E5" };
    std::vector<std::string> result;

	push_back(result, ranges::view::cartesian_product(input1, input2) | ranges::view::transform(tupled_args(letterPlusNumber)));

    return result == expected;
}

std::string letterToNext(char letter, char next)
{
    std::ostringstream result;
    result << letter << '-' << next;
    return result.str();
}

bool testAdjacent()
{
    std::vector<char> input = { 'A', 'B', 'C', 'D', 'E', 'F' };
    std::vector<std::string> expected = { "A-B", "B-C", "C-D", "D-E", "E-F" };
    std::vector<std::string> result;

	push_back(result, input | ranges::view::adjacent | ranges::view::transform(paired_args(letterToNext)));

    return result == expected;
}

template <typename Function>
std::string testResult(Function test)
{
    return test() ? "PASSED" : "FAILED";
}

void launchTests()
{
    std::cout << "TEST TRANSFORM\t\t" << testResult(testTransform) << std::endl;
    std::cout << "TEST ZIP\t\t" << testResult(testZip) << std::endl;
    std::cout << "TEST CARTESIAN_PRODUCT\t\t" << testResult(testCartesianProduct) << std::endl;
    std::cout << "TEST ADJACENT\t\t" << testResult(testAdjacent) << std::endl;
}

}

int main()
{
    launchTests();
}
    
