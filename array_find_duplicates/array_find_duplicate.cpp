#include <iostream>
#include <vector>
#include <unordered_set>

template <typename T>

bool duplicate(const std::vector<T> & v)
{
    std::unordered_set<T> s;
    for (auto & i : v)
    {
        auto item = s.find(i);
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
    std::vector<int> v1 = {1,2,3,4,5,6,7,8,9};
    std::vector<int> v2 = {1,2,3,4,5,6,1,8,9};

    std::cout << "duplicate(v1): " << duplicate(v1) << std::endl;
    std::cout << "duplicate(v2): " << duplicate(v2) << std::endl;
    return 0;
}
