#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>

using namespace std;
using namespace chrono;

using write_sequence = vector<string>;

using test_pair = pair<uint64_t, string>;
using modify_sequence = vector<test_pair>;
using read_sequence = vector<test_pair>;

ifstream& operator >> (ifstream& _is, test_pair& _value)
{
    _is >> _value.first;
    _is >> _value.second;

    return _is;
}

template <typename S>
S get_sequence(const string& _file_name)
{
    ifstream infile(_file_name);
    S result;

    typename S::value_type item;

    while (infile >> item)
    {
        result.emplace_back(move(item));
    }

    return result;
}



class storage
{
private:
   vector<string> cont_str;
  

public:
    void sort()
    {
        //sortR( cont_str, 0, cont_str.size() - 1);
        std::sort(cont_str.begin(), cont_str.end());
    }

    void insert(const string& _str)
    {
        cont_str.push_back(_str);
    }

    void insert_sorted(const string& _str)
    {
      auto pos = upper_bound(cont_str.begin(), cont_str.end(), _str);
      if (pos == cont_str.end()) cont_str.push_back(_str);
      else  cont_str.insert(pos + 1, _str);
    }

    void erase(uint64_t _index)
    {
        
        if (cont_str.size() >= _index)
        {
            cont_str.erase(cont_str.begin() + _index);
        }
    }
    const string& get(uint64_t _index)
    {
       if (cont_str.size() >= _index)
        {
            return (cont_str[_index]);
        }
        else return ("");
    }
};

int main()
{
    write_sequence write = get_sequence<write_sequence>("write.txt");
    modify_sequence modify = get_sequence<modify_sequence>("modify.txt");
    read_sequence read = get_sequence<read_sequence>("read.txt");
    storage st;


    for (const string& item : write)
    {
        st.insert(item);
    }

    st.sort();
    uint64_t progress = 0;
    uint64_t percent = modify.size() / 100;

    time_point<system_clock> time;
    nanoseconds total_time(0);

    modify_sequence::const_iterator mitr = modify.begin();
    read_sequence::const_iterator ritr = read.begin();
    i = 0;
    for (; mitr != modify.end() && ritr != read.end(); ++mitr, ++ritr)
    {

        time = system_clock::now();
        st.erase(mitr->first);
        st.insert_sorted(mitr->second);
        
        const string& str = st.get(ritr->first);
        total_time += system_clock::now() - time;

        if (ritr->second != str)
        {
            cout << "time: " << duration_cast<milliseconds>(total_time).count()
                << "ms progress: " << progress << " / " << modify.size() << "\n";
            cout << "test failed" << endl;
            return 1;
        }

        if (++progress % (5 * percent) == 0)
        {
            cout << "time: " << duration_cast<milliseconds>(total_time).count()
                << "ms progress: " << progress << " / " << modify.size() << "\n";
        }
        
    }


    return 0;
}


