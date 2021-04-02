#include <iostream>
#include <map>
#include <list>
#include <iterator>
#include <string>
#include <utility>
#include <sstream>


class MRUCache
{
public:
    MRUCache(int size) : m_size(size)
    {
    }

    void Add(const int key, const std::string value)
    {
        // check if the key already exists
        auto map_iter = m_map.find(key);
        if (map_iter != m_map.end())
        {
            std::cout << "ADD: DUP KEY" << std::endl;
            return;
        }

        std::cout << "ADD: " << key << "[" << value << "]" << std::endl;
        // if cache is full delete the last item
        if (m_list.size() >= m_size)
        {
            int k = m_list.back().first;
            m_map.erase(k);
            m_list.pop_back();
        }
        std::pair<int, std::string> p;
        p.first = key;
        p.second = value;
        auto iter = m_list.insert(m_list.begin(), p);
        m_map[key] = iter;
    }

    void Delete(int key)
    {
        auto map_iter = m_map.find(key);
        if (map_iter == m_map.end())
        {
            std::cout << "DELETE: NOT FOUND" << std::endl;
            return;
        }
        m_list.erase(m_map[key]);
        m_map.erase(map_iter);
        std::cout << "DELETE: " << key << std::endl;
    }

    bool TryGetValue(int key, std::string & value)
    {
        auto map_iter = m_map.find(key);
        if (map_iter == m_map.end())
        {
            return false;
        }
        value = map_iter->second->second;

        std::pair<int,std::string> p;
        p.first = map_iter->second->first;
        p.second = map_iter->second->second;
        m_list.erase(map_iter->second);
        auto iter = m_list.insert(m_list.begin(), p);
        m_map.erase(map_iter);
        m_map[key] = iter;
        return true;
    }

    void printKeysInOrder()
    {
        std::stringstream ss;
        ss << "CACHE = [";
        for (auto k : m_list)
        {
            ss << k.first << ",";
        }
        ss << "]" << std::endl;
        std::cout << ss.str();
    }

private:
    int m_size;
    std::map<int, std::list<std::pair<int,std::string>>::iterator> m_map;
    std::list<std::pair<int,std::string>> m_list;
};

void helper_tryget(MRUCache * c, const int k, std::string  & v)
{
    bool gotValue;
    gotValue = c->TryGetValue(k,v);
    if (gotValue)
    {
        std::cout << "TRYGET map[" << k << "] = " << v << std::endl;
    }
    else {
        std::cout << "TRYGET map[" << k << "] = NOT FOUND" << std::endl;
    }

}

int main()
{

    MRUCache c(2);

    c.Add(1,std::string("test1"));
    c.printKeysInOrder();
    c.Add(2,std::string("test2"));
    c.printKeysInOrder();

    std::string v;
    helper_tryget(&c,1,v);
    c.printKeysInOrder();

    c.Add(3,std::string("test3"));
    c.printKeysInOrder();


    c.Delete(1);
    c.printKeysInOrder();
    helper_tryget(&c,1,v);
    return 0;
}
