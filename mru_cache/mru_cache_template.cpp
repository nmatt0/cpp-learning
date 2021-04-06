#include <iostream>
#include <map>
#include <list>
#include <sstream>

template<typename K, typename V>
class mru_cache
{
public:
    mru_cache(const int size) : m_size(size)
    {
    }

    int size()
    {
        return m_map.size();
    }

    void add(const K key, const V value)
    {
        auto map_iter = m_map.find(key);

        // if key already exists: update value and move to front
        if (map_iter != m_map.end())
        {
            map_iter->second.value = value;
            auto ln = map_iter->second.list_iter;
            m_list.erase(ln);
            m_list.push_front(*ln);
            return;
        }
        // if cache is full: delete last item in list
        else if (m_map.size() == m_size)
        {
            m_map.erase(m_map.find(m_list.back().key));
            m_list.pop_back();
        }

        // add the new item
        map_node mn;
        list_node ln;
        ln.key = key;
        auto list_iter = m_list.insert(m_list.begin(),ln);
        mn.list_iter = list_iter;
        mn.value = value;
        m_map[key] = mn;
    }

    void erase(const K key)
    {
        auto map_iter = m_map.find(key);
        if (map_iter == m_map.end())
        {
            return;
        }
        m_list.erase(m_map[key].list_iter);
        m_map.erase(map_iter);
    }

    bool trygetvalue(const K key, V & value)
    {
        auto map_iter = m_map.find(key);
        if (map_iter == m_map.end())
        {
            return false;
        }
        value = map_iter->second.value;
        auto ln = map_iter->second.list_iter;
        m_list.erase(ln);
        m_list.push_front(*ln);
        return true;
    }

    void printListKeys()
    {
        std::stringstream ss;
        ss << "LIST = [";
        for (list_node ln : m_list)
        {
            ss << ln.key << ",";
        }
        std::cout << ss.str() << "]" << std::endl;
    }
    
private:
    class list_node
    {
    public:
        list_node()
        {
        }

        K key;
    };
    class map_node
    {
    public:
        map_node()
        {
        }

        typename std::list<list_node>::iterator list_iter;
        V value;
    };


    int m_size;
    std::map<K,map_node> m_map;
    std::list<list_node> m_list;
};

int main()
{
    mru_cache<int,std::string> c(5);
    c.add(1,"test1");
    c.add(2,"test2");
    c.add(3,"test3");
    c.add(4,"test4");
    c.add(5,"test5");
    c.add(6,"test6");
    std::cout << "cache size: " << c.size() << std::endl;
    c.printListKeys();
    c.erase(4);
    c.printListKeys();
    std::string try2;
    c.trygetvalue(2,try2);
    std::cout << "got value: " << try2 << std::endl;
    c.printListKeys();
    return 0;
}
