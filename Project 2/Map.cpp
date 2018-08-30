#include "Map.h"
#include <iostream>

Map::Map() {
    m_size = 0;         // creates empty map
    head = nullptr;
    tail = nullptr;
}

Map::~Map() {
    Node *p = head;         // temporary pointer to traverse through list
    while (p != nullptr) {
        Node *t = p;
        p = p->next;
        delete t;       // deletes node
    }
}

Map::Map(const Map &other) {
    m_size = 0;
    head = nullptr;
    tail = nullptr;
    
    for (int i = 0; i < other.size(); i++) {
        KeyType key;
        ValueType val;
        
        other.get(i, key, val);    // gets and then inserts the val
        insert(key, val);
    }
}

Map& Map::operator=(const Map &other) {
    if (&other != this) {
        Map temp(other);    // creates identical temporary map
        swap(temp);         // swaps the temp map with map
    }
    return *this;           // do nothing
}

bool Map::empty() const {
    return m_size == 0;     // returns true if map is empty
}

int Map::size() const {
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    if (contains(key))
        return false;
    
    Node *p = new Node;         // creates new node
    p->m_struct.m_key = key;
    p->m_struct.m_value = value;
    
    if (empty()) {
        head = p;
        tail = p;
    }
    
    // adds node to the beginning of the map
    
    p->prev = tail;
    tail->next = p;
    tail = p;
    p->next = nullptr;
    m_size++;
    
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
    Node *p = head;
    while (p != nullptr) {
        if (p->m_struct.m_key == key) {     // if the map contains key, update the value
            p->m_struct.m_value = value;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if (update(key, value))
        return true;
    return (insert(key, value));    // will always return true
}

bool Map::erase(const KeyType& key) {
    if (!contains(key))     // if list doesn't contain key, return false
        return false;
    
    if (head == nullptr)    // if list is empty, return false
        return false;
    
    Node *p = head;
    
    if (m_size == 1) {      // if list only has one node
        delete head;
        m_size--;
        head = nullptr;
        tail = nullptr;
        return true;
    }
    
    while (p != nullptr) {
        if (p->m_struct.m_key == key) {
            if (p == head) {
                head = p->next;
                head->prev = nullptr;   // changes prev to nullptr
                delete p;
            }
            else if (p == tail) {
                tail = p->prev;
                tail->next = nullptr;
                delete p;
            }
            else {
                Node *temp = p->prev;
                Node *temp2 = p->next;
                temp->next = temp2;     // sets next to the node after
                temp2->prev = temp;     // sets prev to the node before
                delete p;
            }
            m_size--;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool Map::contains(const KeyType& key) const {
    Node *p = head;
    while (p != nullptr) {
        if (p->m_struct.m_key == key)
            return true;        // if the key is there, return true
        p = p->next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    Node *p = head;
    while (p != nullptr) {
        if (p->m_struct.m_key == key) {     // if the key exists, change value and return true
            value = p->m_struct.m_value;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (i < 0 || i >= size())       // if i is out of bounds
        return false;
    Node *p = head;
    for (int j = 0; j < i; j++)
        p = p->next;
    
    key = p->m_struct.m_key;        // changes key
    value = p->m_struct.m_value;    // changes value
    
    return true;
}

void Map::swap(Map& other) {
    Node *tempHead = head;
    Node *tempTail = tail;
    int tempSize = size();
    
    head = other.head;
    tail = other.tail;
    
    other.head = tempHead;
    other.tail = tempTail;
    
    m_size = other.size();
    other.m_size = tempSize;
}

bool combine(const Map& m1, const Map& m2, Map& result) {
    Map temp1 = m1;
    Map temp2 = m2;
    temp1.swap(result);     // has all nodes of m1
    bool comb = true;
    
    for (int i = 0; i < temp2.size(); i++) {
        KeyType key;
        ValueType val;
        temp2.get(i, key, val);
        
        if (!result.contains(key))      // if m1 doesn't contain m2 node, add node
            result.insert(key, val);
        else {
            ValueType val2;
            result.get(key, val2);
            if (val != val2) {          // if m1 and m2 have same key but different values
                result.erase(key);      // remove result and return false
                comb = false;
            }
        }
    }
    return comb;
}

void subtract(const Map& m1, const Map& m2, Map& result) {
    Map temp1 = m1;
    Map temp2 = m2;
    Map empty;
    empty.swap(result);     // swaps result with empty map
    
    for (int i = 0; i < temp1.size(); i++) {
        KeyType key;
        ValueType val;
        temp1.get(i, key, val);
        
        if (!temp2.contains(key))       // if m2 does not have key of m1
            result.insert(key, val);    // insert into result 
    }
}



