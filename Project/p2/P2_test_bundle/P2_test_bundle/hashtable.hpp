#include "hash_prime.hpp"

#include <exception>
#include <functional>
#include <vector>
#include <forward_list>
#include <algorithm>
#include <cmath>


/**
 * The Hashtable class
 * The time complexity of functions are based on n and k
 * n is the size of the hashtable
 * k is the length of Key
 * @tparam Key          key type
 * @tparam Value        data type
 * @tparam Hash         function object, return the hash value of a key
 * @tparam KeyEqual     function object, return whether two keys are the same
 */
template<
        typename Key, typename Value,
        typename Hash = std::hash<Key>,
        typename KeyEqual = std::equal_to<Key>
>
class HashTable {
public:
    typedef std::pair<const Key, Value> HashNode;
    typedef std::forward_list<HashNode> HashNodeList;
    typedef std::vector<HashNodeList> HashTableData;

    /**
     * A single directional iterator for the hashtable
     * ! DO NOT NEED TO MODIFY THIS !
     */
    class Iterator {
    private:
        typedef typename HashTableData::iterator VectorIterator;
        typedef typename HashNodeList::iterator ListIterator;

        const HashTable *hashTable;
        VectorIterator bucketIt;    // an iterator of the buckets
        ListIterator listItBefore;  // a before iterator of the list, here we use "before" for quick erase and insert
        bool endFlag = false;       // whether it is an end iterator

        /**
         * Increment the iterator
         * Time complexity: Amortized O(1)
         */
        void increment() {
            if (bucketIt == hashTable->buckets.end()) {
                endFlag = true;
                return;
            }
            auto newListItBefore = listItBefore;
            ++newListItBefore;
            if (newListItBefore != bucketIt->end()) {
                if (++newListItBefore != bucketIt->end()) {
                    // use the next element in the current forward_list
                    ++listItBefore;
                    return;
                }
            }
            while (++bucketIt != hashTable->buckets.end()) {
                if (!bucketIt->empty()) {
                    // use the first element in a new forward_list
                    listItBefore = bucketIt->before_begin();
                    return;
                }
            }
            endFlag = true;
        }

        explicit Iterator(HashTable *hashTable) : hashTable(hashTable) {
            bucketIt = hashTable->buckets.begin();
            listItBefore = bucketIt->before_begin();
            endFlag = bucketIt == hashTable->buckets.end();
        }

        Iterator(HashTable *hashTable, VectorIterator vectorIt, ListIterator listItBefore) :
                hashTable(hashTable), bucketIt(vectorIt), listItBefore(listItBefore) {
            endFlag = bucketIt == hashTable->buckets.end();
        }

    public:
        friend class HashTable;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            if (endFlag && that.endFlag) return true;
            if (bucketIt != that.bucketIt) return false;
            return listItBefore == that.listItBefore;
        }

        bool operator!=(const Iterator &that) const {
            if (endFlag && that.endFlag) return false;
            if (bucketIt != that.bucketIt) return true;
            return listItBefore != that.listItBefore;
        }

        HashNode *operator->() {
            auto listIt = listItBefore;
            ++listIt;
            return &(*listIt);
        }

        HashNode &operator*() {
            auto listIt = listItBefore;
            ++listIt;
            return *listIt;
        }
    };

protected:                                                                  // DO NOT USE private HERE!
    static constexpr double DEFAULT_LOAD_FACTOR = 0.5;                      // default maximum load factor is 0.5
    static constexpr size_t DEFAULT_BUCKET_SIZE = HashPrime::g_a_sizes[0];  // default number of buckets is 5

    HashTableData buckets;                                                  // buckets, of singly linked lists
    typename HashTableData::iterator firstBucketIt;                         // help get begin iterator in O(1) time                             
    size_t tableSize;                                                       // number of elements
    double maxLoadFactor;                                                   // maximum load factor
    Hash hash;                                                              // hash function instance
    KeyEqual keyEqual;                                                      // key equal function instance

    /**
     * Time Complexity: O(k)
     * @param key
     * @param bucketSize
     * @return the hash value of key with a new bucket size
     */
    inline size_t hashKey(const Key &key, size_t bucketSize) const {
        return hash(key) % bucketSize;
    }

    /**
     * Time Complexity: O(k)
     * @param key
     * @return the hash value of key with current bucket size
     */
    inline size_t hashKey(const Key &key) const {
        return hash(key) % buckets.size();
    }

    /**
     * Find the minimum bucket size for the hashtable
     * The minimum bucket size must satisfy all of the following requirements:
     * - It is not less than (i.e. greater or equal to) the parameter bucketSize
     * - It is greater than floor(tableSize / maxLoadFactor)
     * - It is a (prime) number defined in HashPrime (hash_prime.hpp)
     * - It is minimum if satisfying all other requirements
     * Time Complexity: O(1)
     * @throw std::range_error if no such bucket size can be found
     * @param bucketSize lower bound of the new number of buckets
     */
    size_t findMinimumBucketSize(size_t bucketSize) const {
        // TODO: implement this function
        // DONE
        // use std::lower_bound to find the smallest item *lower >= bucketSize
        auto lower = std::lower_bound(HashPrime::g_a_sizes, HashPrime::g_a_sizes + HashPrime::num_distinct_sizes_64_bit, bucketSize);
        // iterate through the array and return if satisfies the condition
        while  (lower != HashPrime::g_a_sizes + HashPrime::num_distinct_sizes_64_bit) {
            if ((double)*lower > floor((double)tableSize / maxLoadFactor)) return (*lower);
            else lower++;
        }
        // if not find, throw this error
        throw std::range_error("no such bucket size can be found!");
    }

    // TODO: define your helper functions here if necessary
    /**
     * >>Helper Function<<   
     * EFFECTS: called by copy constructor & assignment operator
     * @param that target HashTable
     */
    void copyFrom(const HashTable &that) {
        // clear original items
        clearAll();
        // resize buckets
        buckets.resize(that.buckets.size());
        // reset other attributes
        firstBucketIt = buckets.end();
        tableSize = 0;
        maxLoadFactor = that.maxLoadFactor;
        hash = that.hash;
        keyEqual = that.keyEqual;
        // use double loop to traverse all the items
        typename HashTableData::const_iterator bucketIt = that.firstBucketIt;
            // use bucketIt to iterate each bucket of "that"
        for (; bucketIt != that.buckets.end(); bucketIt++) { // iterate each bucket
            if (bucketIt->empty()) continue;
            typename HashNodeList::const_iterator nodeIt = bucketIt->before_begin();
            ++nodeIt;
            //use nodeIt to iterate all the nodes in HashNodeList
            while (nodeIt != bucketIt->end()) { // iterate each node of the node list
                insert((*nodeIt).first, (*nodeIt).second);
                nodeIt++;
            }
        }
    }

    /**
     * >>Helper Function<<
     * EFFECTS: Erase all the items in the hashtable
     */
    void clearAll() {
        Iterator it = begin();
        while (it != end()) {
            it = erase(it);
        }
    }

public:
    HashTable() :
            buckets(DEFAULT_BUCKET_SIZE), tableSize(0), maxLoadFactor(DEFAULT_LOAD_FACTOR),
            hash(Hash()), keyEqual(KeyEqual()) {
        firstBucketIt = buckets.end();
    }

    explicit HashTable(size_t bucketSize) :
            tableSize(0), maxLoadFactor(DEFAULT_LOAD_FACTOR),
            hash(Hash()), keyEqual(KeyEqual()) {
        bucketSize = findMinimumBucketSize(bucketSize);
        buckets.resize(bucketSize);
        firstBucketIt = buckets.end();
    }

    HashTable(const HashTable &that) {
        // TODO: implement this function
        copyFrom(that);
    }

    HashTable &operator=(const HashTable &that) {
        // TODO: implement this function
        if (this != &that)
            copyFrom(that);
        return *this;
    };

    ~HashTable() = default;

    Iterator begin() {
        if (firstBucketIt != buckets.end()) {
            return Iterator(this, firstBucketIt, firstBucketIt->before_begin());
        }
        return end();
    }

    Iterator end() {
        return Iterator(this, buckets.end(), buckets.begin()->before_begin());
    }

    /**
     * Find whether the key exists in the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @return whether the key exists in the hashtable
     */
    bool contains(const Key &key) {
        return find(key) != end();
    }

    /**
     * Find the value in hashtable by key
     * If the key exists, iterator points to the corresponding value, and it.endFlag = false
     * Otherwise, iterator points to the place that the key were to be inserted, and it.endFlag = true
     * Time Complexity: Amortized O(k)
     * @param key
     * @return a pair (success, iterator of the value)
     */
    Iterator find(const Key &key) {
        // TODO: implement this function
        size_t hashValue = hashKey(key);

        // Iterator "it" points at the bucket specified by hash value of "key"
        Iterator it(this);
        it.endFlag = true; // will set the endflag as false if the key is found
        it.bucketIt = buckets.begin() + hashValue;
        it.listItBefore = it.bucketIt->before_begin();
        
        // "next" is the succeeding list iterator of it.listItBefore
        auto next = it.listItBefore; ++next; 
        
        // iterate over the node list to find if the key exists
        while (next != it.bucketIt->end()) { 
            // case 1: the key exists
            if ((*next).first == key){ 
                it.endFlag = false;
                break;
            }
            it.listItBefore++; next++;
        }
        
        // case 2: the key doesn't exist
        //  set "it" at the place that the key were to be inserted
        if (it.endFlag) it.listItBefore = it.bucketIt->before_begin();
        
        return it;
    }

    /**
     * Insert value into the hashtable according to an iterator returned by find
     * the function can be only be called if no other write actions are done to the hashtable after the find
     * If the key already exists, overwrite its value
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: O(k)
     * @param it an iterator returned by find
     * @param key
     * @param value
     * @return whether insertion took place (return false if the key already exists)
     */
    bool insert(const Iterator &it, const Key &key, const Value &value) {
        // TODO: implement this function
        //update key & value
        if (!it.endFlag) // delete the original item if already exists
            it.bucketIt->erase_after(it.listItBefore);
        it.bucketIt->insert_after(it.listItBefore, HashNode(key, value)); // add new item
        
        // update tableSize if a new key is inserted
        if (it.endFlag) tableSize++; 
        
        // update firstBucketIt
        if (it.bucketIt < firstBucketIt) firstBucketIt = it.bucketIt;
        
        // rehash if exceeds L_max
        if (loadFactor() > maxLoadFactor) rehash(buckets.size());

        return it.endFlag;
    }

    /**
     * Insert <key, value> into the hashtable
     * If the key already exists, overwrite its value
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @param value
     * @return whether insertion took place (return false if the key already exists)
     */
    bool insert(const Key &key, const Value &value) {
        // TODO: implement this function
        // find the Iterator "it" to insert "key"
        Iterator it = find(key);

        // use "it" to insert (key, value) pair
        return insert(it, key, value);
    }

    /**
     * Erase the key if it exists in the hashtable, otherwise, do nothing
     * DO NOT rehash in this function
     * firstBucketIt should be updated
     * Time Complexity: Amortized O(k)
     * @param key
     * @return whether the key exists
     */
    bool erase(const Key &key) {
        // TODO: implement this function
        // find the Iterator "it" to insert "key"
        Iterator it = find(key);

        // case 1: key doesn't exist
        if (it.endFlag) return false; 

        // case 2: key exists
        erase(it);
        return true;
    }

    /**
     * Erase the key at the input iterator
     * If the input iterator is the end iterator, do nothing and return the input iterator directly
     * firstBucketIt should be updated
     * Time Complexity: O(1)
     * @param it
     * @return the iterator after the input iterator before the erase
     */
    Iterator erase(const Iterator &it) {
        // TODO: implement this function
        // case 1: input iterator is the end iterator
        if (it == end()) return it; 

        // case 2: input iterator is not the end iterator
        //  erase the key at the input iterator;
        //  update firstBucketIt;
        //  decrease tableSize;
        tableSize--;
        Iterator returnIt(it);
        auto listIt = it.listItBefore;
        listIt++; listIt++;
        if (listIt == it.bucketIt->end()) returnIt++;

        // erase the item pointed by "it"
        it.bucketIt->erase_after(it.listItBefore);
        
        // update firstBucketIt if necessary
        //  only when erase results in empty first bucket
        if (it.bucketIt->empty() && firstBucketIt == it.bucketIt) {
            Iterator newIt(it); newIt++;
            firstBucketIt = newIt.bucketIt; // update firstBucketIt
        }
            
        return returnIt;
    }

    /**
     * Get the reference of value by key in the hashtable
     * If the key doesn't exist, create it first (use default constructor of Value)
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @return reference of value
     */
    Value &operator[](const Key &key) {
        // TODO: implement this function
        Iterator it = find(key);

        // case: key doesn't exist
        if (it.endFlag) { 
            HashNode newNode;
            insert(it, key, newNode.second); // insert new item
        }
        
        // update iterator 
        //  in case of rehash caused by insert
        it = find(key); 
        
        return it->second;
    }

    /**
     * Rehash the hashtable according to the (hinted) number of buckets
     * The bucket size after rehash need not be same as the parameter bucketSize
     * Instead, findMinimumBucketSize is called to get the correct number
     * firstBucketIt should be updated
     * Do nothing if the bucketSize doesn't change
     * Time Complexity: O(nk)
     * @param bucketSize lower bound of the new number of buckets
     */
    void rehash(size_t bucketSize) {
        bucketSize = findMinimumBucketSize(bucketSize);
        if (bucketSize == buckets.size()) return;
        // TODO: implement this function
        // create a new table to hold rehashed items
        HashTable newTable(bucketSize);

        // scan the current table, insert items into new table
        for (auto it = begin(); it != end(); ++it) {
            newTable.insert(it->first, it->second);
            if (it == begin()) firstBucketIt = it.bucketIt; //update firstBucketIt
        }

        // replace the current hashtable
        *this = newTable;
    }

    /**
     * @return the number of elements in the hashtable
     */
    size_t size() const { return tableSize; }

    /**
     * @return the number of buckets in the hashtable
     */
    size_t bucketSize() const { return buckets.size(); }

    /**
     * @return the current load factor of the hashtable
     */
    double loadFactor() const { return (double) tableSize / (double) buckets.size(); }

    /**
     * @return the maximum load factor of the hashtable
     */
    double getMaxLoadFactor() const { return maxLoadFactor; }

    /**
     * Set the max load factor
     * @throw std::range_error if the load factor is too small
     * @param loadFactor
     */
    void setMaxLoadFactor(double loadFactor) {
        if (loadFactor <= 1e-9) {
            throw std::range_error("invalid load factor!");
        }
        maxLoadFactor = loadFactor;
        rehash(buckets.size());
    }

};

