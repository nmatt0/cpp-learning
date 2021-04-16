#include <iostream>
#include <vector>
#include <unordered_set>

bool sum_to_number(const std::vector<int> & v, const int k)
{
    std::unordered_set<int> s;
    for (int i : v)
    {
        auto item = s.find(k - i);
        if (item != s.end())
        {
            return true;
        }
        s.insert(i);
    }
    return false;
}

int main()
{
    std::vector<int> v1 = {1,10,20,50};
    
    std::cout << "sum_to_number(v1,51): " << sum_to_number(v1,51) << std::endl;
    std::cout << "sum_to_number(v1,100): " << sum_to_number(v1,100) << std::endl;
    return 0;
}
