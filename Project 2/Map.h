#ifndef Map_h
#define Map_h

#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
public:
    Map();
    ~Map();
    Map(const Map &other);
    Map& operator=(const Map &other);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    
private:
    struct mapStruct {          
        KeyType m_key;
        ValueType m_value;
    };
    
    struct Node {
        mapStruct m_struct;
        Node *next;
        Node *prev;
    };
    
    int m_size;
    Node *head;
    Node *tail;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif
