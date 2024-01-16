#include "HashMap.hpp"
using std::string;

class InvalidKey : public std::invalid_argument
{
public:
    //support two constructors
    InvalidKey(const string& msg): std::invalid_argument(msg){}
    InvalidKey(): std::invalid_argument(KEY_ERR){}
};

class Dictionary : public HashMap<string, string>
{

public:
    //getting the constructors
    using HashMap<string, string>::HashMap;
    ~Dictionary() override = default;
    /**
     * receives a string key and erase (key, value) from the dictionary,
     * if key isnt exist in the dict throw exception
     * @param key
     * @return true in success
     */
    bool erase(const string& key) override
    {
        bool res = HashMap<string,string>::erase(key);
        // wrap the base's class with exception throwing
        if(!res)
        {
            throw InvalidKey();
        }
        return res;
    }
    template<typename Iter>
    /**
     * receives a range and updates the curr dict by the iterators values
     * @tparam Iter
     * @param start
     * @param end
     */
    void update(Iter start, Iter end)
    {
        // moves on the given range and update our dict by it
        while (start != end)
        {
            //update by the curr iterator
            (*this)[start->first] = start->second;
            start++;
        }
    }
};