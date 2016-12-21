#include "cartesian_product.hpp"
#include "transform.hpp"

#include "algorithm.hpp"
#include "variadic.hpp"

#include <sstream>
#include <vector>

std::string letterPlusNumber(char line, int column)
{
    std::ostringstream position;
    position << line << column;
    return position.str();
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

