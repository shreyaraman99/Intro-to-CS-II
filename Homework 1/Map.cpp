
#include "Map.h"
#include <iostream>

Map::Map(){
    m_size = 0;
}

bool Map::empty() const {
    return m_size == 0;
}

int Map::size() const {
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    if (!contains(key) && m_size < DEFAULT_MAX_ITEMS){
        m_map[m_size].m_key = key;
        m_map[m_size].m_value = value;
        m_size++;
        return true;
    }
    else {
        return false;
    }
}

bool Map::update(const KeyType& key, const ValueType& value) {
    for (int i = 0; i < m_size; i++) {
        if (m_map[i].m_key == key) {
            m_map[i].m_value = value;
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
        if (m_map[i].m_key == key){
            for (int j = i; j < tempSize - 1; j++)
                m_map[j] = m_map[j + 1];
            m_size--;
            return true;
        }
    }
    return false;
}

bool Map::contains(const KeyType& key) const {
    for (int i = 0; i < m_size; i++) {
        if (m_map[i].m_key == key)
            return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    for (int i = 0; i < m_size; i++) {
        if (m_map[i].m_key == key) {
            value = m_map[i].m_value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (0 <= i && i < m_size) {
        key = m_map[i].m_key;
        value = m_map[i].m_value;
        return true;
    }
    return false;
}

void Map::swap(Map& other) {
    Map temp = *this;
    *this = other;
    other = temp;
}



