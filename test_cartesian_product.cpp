/* cartesian_product is a range view adaptor to represent all possible combinations of several input ranges in the form of tuples.
   For instance, applying it on 3 ranges containing:
   { a,b }
   { 1,2 }
   { X,Y,Z }
   produces a view seeing:
   {
     std::tuple(a,1,X), std::tuple(a,1,Y), std::tuple(a,1,Z),
     std::tuple(a,2,X), std::tuple(a,2,Y), std::tuple(a,2,Z),
     std::tuple(b,1,X), std::tuple(b,1,Y), std::tuple(b,1,Z),
     std::tuple(b,2,X), std::tuple(b,2,Y), std::tuple(b,2,Z),
   }
*/

#include "cartesian_product.hpp"
#include "transform.hpp"

#include "algorithm.hpp"
#include "variadic.hpp"

#include <sstream>
#include <vector>

namespace
{
std::string letterPlusNumber(char line, int column)
{
    std::ostringstream position;
    position << line << column;
    return position.str();
}
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

