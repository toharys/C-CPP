#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#define USAGE_ERR "keys vector and values vector must be in same length"
#define UPPER_FACTOR 0.75
#define LOWER_FACTOR 0.25
#define INITIAL_CAPACITY 16
#define MIN_CAPACITY 1
#define CAPACITY_INCREASE_VAL 2
#define CAPACITY_DECREASE_VAL 0.5
#define KEY_ERR "invalid key: key not exist"

using std::vector;
using std::pair;
using std::hash;

template<class KeyT, class ValueT>
class HashMap
{

    typedef vector<KeyT> k_vec;
    typedef vector<ValueT> v_vec;
    typedef vector<pair<KeyT, ValueT>> bucket;

    // HashMap fields
    int _size;
    int _capacity;
    bucket* _buckets_arr;

private:
    /**
     * get a generic key and calculates the given modulo formula
     * @param key
     * @return the formula output
     */
    size_t hash_func (const KeyT& key) const
    {
        return hash<KeyT>{}(key)&(_capacity-1);
    }
    /**
     * reallocate the buckets arr and move the buckets than update the capacity
     */
    void rehash(double resize_factor)
    {
        // allocates new bucket

        int new_capacity = MIN_CAPACITY>(_capacity*resize_factor) ?
                                MIN_CAPACITY : _capacity*resize_factor;
        // increase/decrease the capacity to the most tight possible value
        if (new_capacity!=MIN_CAPACITY && resize_factor==CAPACITY_INCREASE_VAL)
        {
            while( _size/(new_capacity*resize_factor)>UPPER_FACTOR)
            {
               new_capacity = new_capacity*resize_factor;
            }
        }
        else if (new_capacity!=MIN_CAPACITY &&
                            resize_factor==CAPACITY_DECREASE_VAL)
        {
            while( _size/(new_capacity*resize_factor)<=LOWER_FACTOR)
            {
                new_capacity=new_capacity*resize_factor;
                if(new_capacity<MIN_CAPACITY)
                {
                    new_capacity = MIN_CAPACITY;
                    break;
                }
            }
        }
        auto new_buckets_arr = new bucket [new_capacity];
        int iterations = _capacity;
        _capacity = new_capacity;
        // copy each old bucket to the new buckets arr and then delete the
        // old one
        for (int i=0;i<iterations;i++)
        {
            bucket old_bucket = _buckets_arr[i];

            for (const auto& pair : old_bucket)
            {
                // find the updated index of the (key,value) int the new
                // HashMap and inserts it
                int indx = this->hash_func(pair.first);
                new_buckets_arr[indx].push_back({pair.first, pair.second});
            }
        }
        delete[] _buckets_arr;
        // updates the arr to the new one
        _buckets_arr = new_buckets_arr;
    }

public:

    //default constructor - initialize to empty HashMap
    HashMap(): _size(0), _capacity(INITIAL_CAPACITY), _buckets_arr(nullptr)
   {
       _buckets_arr = new bucket[INITIAL_CAPACITY];
   }
    //regular constructor
    HashMap(const k_vec& keys, const v_vec& values):_size(0),
    _buckets_arr(nullptr)
    {
        if (keys.size() != values.size())
        {
            throw std::invalid_argument(USAGE_ERR);
        }
        // initialize the capacity with the default size
        _capacity = INITIAL_CAPACITY;
        _buckets_arr = new bucket[_capacity]; // allocates array of buckets
        for (size_t i=0;i<keys.size();i++)
        {
            if(!this->contains_key(keys[i]))
            {
                this->insert(keys[i], values[i]);
            }
            else
            {
                (*this)[keys[i]] = values[i];
            }
        }
    }
    //copy constructor
    HashMap(const HashMap<KeyT,ValueT>& other): _size(other._size),
        _capacity(other._capacity), _buckets_arr(nullptr)
    {
        _buckets_arr = new bucket[other._capacity]; // allocate buckets array

        for (int i = 0; i < other._capacity; i++)
        {   // move on the all buckets in other's arr
            bucket other_bucket = other._buckets_arr[i];
            for (const auto& pair: other_bucket)
            { // insert each other bucket's data (key, value) to bucket
                // in this array
                _buckets_arr[i].push_back({pair.first, pair.second});
            }
        }
    }
    // destructor
    virtual ~HashMap()
    {
        delete[] this->_buckets_arr;
    }

    //methods
    /**
     * @return the HashMap's size
     */
    int size() const
    {
        return this->_size;
    }
    /**
    * @return the HashMap's capacity
    */
    int capacity() const
    {
        return this->_capacity;
    }
    /**
     * checks if the HashMap is empty
     * @return true if empty, else false
     */
    bool empty() const
    {
        return (_size == 0);
    }
    /**
     * key and value and inserts them into the HashMap
     * @param key
     * @param val
     * @return
     */
    bool insert(KeyT key, ValueT val)
    {
        // check if the key already exist in the HashMap, if exist return false
        if(this->contains_key(key))
        {
            return false;
        }
        // if not exist insert it
        size_t indx = this->hash_func(key);
        _buckets_arr[indx].push_back({key, val});
        _size++;
        // check if rehash needed
        if (get_load_factor()>UPPER_FACTOR)
        {
            this->rehash(CAPACITY_INCREASE_VAL);
        }
        return true;
    }
    /**
     * get a key and check if it is in the HashMap
     * @param key
     * @return true if exist else false
     */
    bool contains_key(const KeyT& key) const
    {
        // if the HashMap or the bucket which should contain the key are
        // empty return false
        size_t index = hash_func(key);
        if(_size==0 || _buckets_arr[index].empty())
        {
            return false;
        }
        // if they arnt empty check if the appropriate bucket contain the key
        for (const auto& pair : _buckets_arr[index])
        {
            if(pair.first==key)
            {
                return true;
            }
        }
        return false; // if the key isn't in the appropriate bucket
    }
    /**
     * receives key an returns its value, if its not in the HashMap throws
     * exception
     * @param key
     * @return key's value
     */
    ValueT at (const KeyT& key) const
    {
        if (!this->contains_key(key))
        { // if the key isn't in HashMap throws exception
            throw std::range_error(KEY_ERR);
        }
        int indx = hash_func(key);
        for(const auto& pair: _buckets_arr[indx])
        {
            if (pair.first==key)
            {
                return pair.second;
            }
        }

        return ValueT();
    }
    /**
     * the non-const version of the method: at
     * @param key
     * @return key's value
     */
    ValueT& at (const KeyT& key)
    {
        if (!this->contains_key(key))
        { // if the key isn't in HashMap throws exception
            throw std::range_error(KEY_ERR);
        }
        int indx = hash_func(key);
        auto temp = ValueT();
        for( auto& pair: _buckets_arr[indx])
        {
            if (pair.first==key)
            {
                temp= pair.second;
                break;
            }
        }
        std::pair<KeyT, ValueT> pair_to_find({key, temp});

        return std::find(_buckets_arr[indx].begin(),
                         _buckets_arr[indx].end(), pair_to_find)->second;
    }
    /**
     * gets a key and erase it from the HashMap
     * @param key
     * @return true if erase succeed else false
     */
    virtual bool erase(const KeyT& key)
    {
        // if the key already not in the HashMap return true
        if (!this->contains_key(key))
        {
            return false;
        }
        // if the key exist removes it from his bucket
        std::pair<KeyT, ValueT> pair_to_erase({key, this->at(key)});
        int indx = hash_func(key); // the bucket that the pair is in it
        auto position_to_erase = std::find(_buckets_arr[indx].begin(),
                  _buckets_arr[indx].end(), pair_to_erase);

        _buckets_arr[indx].erase(position_to_erase);
        //checks if after we removed the key we need to rehash
        _size--;
        if (this->get_load_factor()<LOWER_FACTOR)
        {
            this->rehash(CAPACITY_DECREASE_VAL);
        }
        return true;
    }
    /**
     * @return the curr HashMap's load factor
     */
    double get_load_factor() const
    {
        if(_size==0)
        {
            return 0;
        }
        return (double) _size/_capacity;
    }
    /**
     * @param key
     * @return the size of the bucket which contains the key
     */
    int bucket_size(const KeyT& key) const
    {
        int indx = this->bucket_index(key); // bucket_index throw exception if
        // the index doesn't exist
        return _buckets_arr[indx].size();
    }
    /**
     * @param key
     * @return the index of the bucket which contains the key
     */
    int bucket_index(const KeyT& key) const
    {
        if (!contains_key(key))
        { // if the key isn't exist throw exception
            throw std::range_error(KEY_ERR);
        }
        return this->hash_func(key);
    }
    /**
     * clears all the buckets in the HashMap
     */
    void clear()
    {
        delete[] _buckets_arr;
        _buckets_arr = new bucket[_capacity];
        _size = 0;
    }

    //operators
    HashMap& operator= (const HashMap& other)
    {
        if (other == *this)
        { // for case of self assignment
            return *this;
        }
        this->_size = other._size;
        this->_capacity = other._capacity;

        auto new_buckets_arr = new bucket[other._capacity];
        // copy each other bucket to the this buckets arr
        for (int i=0;i<_capacity;i++)
        {
            bucket other_bucket = other._buckets_arr[i];
            for (const auto& pair : other_bucket)
            {
                new_buckets_arr[i].push_back({pair.first, pair.second});
            }
        }
        delete[] _buckets_arr;
        _buckets_arr = new_buckets_arr;
        return *this;
    }
    ValueT operator[] (const KeyT& key) const
    {
        if (this->contains_key(key))
        {
            return this->at(key);
        }
        ValueT initial_val = ValueT();
        return initial_val;
    }
    ValueT& operator[] (const KeyT& key)
    {   // if the key is in the HashMap return its value
        if (this->contains_key(key))
        {
            return this->at(key);
        }
        // if the key isnt in the HashMap insert it and then return its value
        ValueT initial_val = ValueT();
        this->insert(key, initial_val);
        return this->at(key);
    }
    bool operator== (const HashMap& other) const
    {
        if(other._size!=this->_size)
        { // if the size isn't equal returns false
            return false;
        }
        for (int i=0;i<other._capacity;i++)
        {   //for each exist bucket
            const auto& other_bucket = other._buckets_arr[i];

            for (const auto& pair : other_bucket)
            {   // if found difference in the two buckets return false
                if (!this->contains_key(pair.first) ||
                        this->at(pair.first)!=pair.second)
                {
                    return false;
                }
            }
        }
        // if the sizes are equal and for each pair in other there is
        return true; //identify pair in this
    }
    bool operator!= (const HashMap& other) const
    {
        return !(*this==other);
    }

    // nested class const iterator
    class ConstIterator {
        friend class HashMap;

    private:
        const HashMap<KeyT, ValueT>& _hash_map;
        size_t _bucket_index;
        size_t _pair_index;
    public:
        // Iterator traits
        typedef std::pair<KeyT, ValueT> value_type; //which type iter points on
        typedef const value_type& reference; //on what the itarator moves
        typedef value_type* pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;

    private:
        ConstIterator(const HashMap<KeyT, ValueT>& h_m, size_t b_index=0,
                      size_t p_index=0):
            _hash_map(h_m), _bucket_index(b_index),_pair_index(p_index){}
    public:
        ConstIterator& operator++ ()
        {
            if(_bucket_index>=_hash_map._capacity)
            {
                return *this;
            }
            _pair_index++;
            bucket curr_bucket = _hash_map._buckets_arr[_bucket_index];
            if(curr_bucket.size()<=_pair_index)
            {
                _pair_index = 0;
                while(++_bucket_index<_hash_map.capacity() &&
                      _hash_map._buckets_arr[_bucket_index].empty()) {}
            }
            return *this;
        }
        ConstIterator& operator++ (int)
        {
            ConstIterator iterator = *this;
            ++*this;
            return iterator;
        }
        bool operator== (const ConstIterator &rhs) const
        {
            return (_bucket_index==rhs._bucket_index &&
                    _pair_index==rhs._pair_index &&
                    &_hash_map==&rhs._hash_map);
        }
        bool operator!= (const ConstIterator &rhs) const
        {
            return !(*this==rhs);
        }
        const reference operator* () const
        {
            return _hash_map._buckets_arr[_bucket_index][_pair_index];
        }
        pointer operator-> () const
        {
            return &(_hash_map._buckets_arr[_bucket_index][_pair_index]);
        }
    };

//    using const_iterator = ConstIterator;
    typedef HashMap<KeyT, ValueT>::ConstIterator const_iterator;
    const_iterator cbegin () const
    {
        int bucket_index = 0;
        if(bucket_index==this->_capacity)
        {
            return this->cend();
        }
        while(this->_buckets_arr[bucket_index].empty() &&
                    ++bucket_index< this->_capacity){}
        return const_iterator(*this,bucket_index);
    }
    const_iterator cend () const
    {
        return const_iterator(*this,_capacity);
    }
    const_iterator begin () const
    {
        int bucket_index = 0;
        if(bucket_index==this->_capacity)
        {
            return this->cend();
        }
        while(this->_buckets_arr[bucket_index].empty() &&
              ++bucket_index<this->_capacity ){}
        return const_iterator(*this,bucket_index);
    }
    const_iterator end () const
    {
        return const_iterator(*this,_capacity);
    }
};