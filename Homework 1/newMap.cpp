
#include "newMap.h"
#include <iostream>
using namespace std;

Map::Map() {
    m_size = 0;
    m_maxSize = DEFAULT_MAX_ITEMS;
    m_struct = new mapStruct[m_maxSize];
}

Map::Map(int size) {
    m_size = 0;
    m_maxSize = size;
    m_struct = new mapStruct[m_maxSize];
}

Map::~Map() {
    delete[] m_struct;
}

Map::Map(const Map &src) {                 
    m_size = src.m_size;
    m_maxSize = src.m_maxSize;
    m_struct = new mapStruct[m_maxSize];
    for (int i = 0; i < m_maxSize; i++)
        m_struct[i] = src.m_struct[i];
}

Map& Map::operator=(const Map &src) {       
    if (&src == this)
        return (*this);
    
    delete[] m_struct;
    m_size = src.m_size;
    m_maxSize = src.m_maxSize;
    m_struct = new mapStruct[m_maxSize];
    for (int i = 0; i < m_maxSize; i++)
        m_struct[i] = src.m_struct[i];
    
    return (*this);
}

bool Map::empty() const {                  
    return m_size == 0;
}

int Map::size() const {
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    if (!contains(key) && m_size < DEFAULT_MAX_ITEMS){
        m_struct[m_size].m_key = key;
        m_struct[m_size].m_value = value;
        m_size++;
        return true;
    }
    else {
        return false;
    }
}

bool Map::update(const KeyType& key, const ValueType& value) {
    for (int i = 0; i < m_size; i++) {
        if (m_struct[i].m_key == key) {
            m_struct[i].m_value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if (update(key, value))
        return true;
    else if (insert(key, value))
        return true;
    else
        return false;
}

bool Map::erase(const KeyType& key) {                   // CHECK THE NESTED FOR LOOP
    int tempSize = m_size;
    for (int i = 0; i < m_size; i++) {
        if (m_struct[i].m_key == key){
            for (int j = i; j < tempSize - 1; j++)
                m_struct[j] = m_struct[j + 1];
            m_size--;
            return true;
        }
    }
    return false;
}

bool Map::contains(const KeyType& key) const {
    for (int i = 0; i < m_size; i++) {
        if (m_struct[i].m_key == key)
            return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    for (int i = 0; i < m_size; i++) {
        if (m_struct[i].m_key == key) {
            value = m_struct[i].m_value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (0 <= i && i < m_size) {
        key = m_struct[i].m_key;
        value = m_struct[i].m_value; 
        return true;
    }
    return false;
}

void Map::swap(Map& other) {
    int sizeTemp = m_size;
    int maxSizeTemp = m_maxSize;
    mapStruct* ptrTemp = m_struct;
    
    m_size = other.m_size;
    m_maxSize = other.m_maxSize;
    m_struct = other.m_struct;
    
    other.m_size = sizeTemp;
    other.m_maxSize = maxSizeTemp;
    other.m_struct = ptrTemp;

}









