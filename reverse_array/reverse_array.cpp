#include <iostream>
#include <vector>
#include <sstream>

void reverse(std::vector<int> & v)
{
    for (int i = 0; i < v.size()/2; i++)
    {
        int swap = v[i];
        v[i] = v[v.size() - i - 1];
        v[v.size() - i - 1] = swap;
    }
}

void print(const std::vector<int> & v, const std::string msg)
{
    std::stringstream ss;
    ss << msg << ": [";
    for (int i : v)
    {
        ss << i << ",";
    }
    ss << "]" << std::endl;
    std::cout << ss.str();
}

int main()
{
    std::vector<int> even = {0,1,2,3};
    std::vector<int> odd = {0,1,2,3,4};
    std::cout << "even:" << std::endl;
    print(even,"before");
    reverse(even);
    print(even,"after");

    std::cout << std::endl << std::endl;

    std::cout << "odd:" << std::endl;
    print(odd,"before");
    reverse(odd);
    print(odd,"after");
    
    return 0;
}
