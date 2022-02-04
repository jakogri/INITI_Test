#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include  <iterator>

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
    multiset<string> cont_str;
  

public:
    
    void insert(const string& _str)
    {
        cont_str.insert(_str);
    }

    void erase(uint64_t _index)
    {
        
        if (cont_str.size() >= _index)
        {
           cont_str.erase(next(cont_str.begin(), _index));
        }
    }
    const string& get(uint64_t _index)
    {
       if (cont_str.size() >= _index)
        {
           multiset<string>::iterator nh = next(cont_str.begin(), _index);
           return (*nh);
           
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

    
    uint64_t progress = 0;
    uint64_t percent = modify.size() / 100;

    time_point<system_clock> time;
    nanoseconds total_time(0);

    modify_sequence::const_iterator mitr = modify.begin();
    read_sequence::const_iterator ritr = read.begin();
    
    for (; mitr != modify.end() && ritr != read.end(); ++mitr, ++ritr)
    {

        
        st.erase(mitr->first);
        st.insert(mitr->second);
        time = system_clock::now();
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
