#ifndef MyHash_h
#define MyHash_h

#include <vector>
#include <list>
#include <functional>
using namespace std;

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
    
    double m_maxLoad;
    double m_loadFactor;
    int m_bucketSize;
    int m_numItems;
    
    struct Node {
        KeyType m_key;
        ValueType m_value;
        Node* next;
    };

    Node** hashTable;
    Node* head;
    Node* tail;
    Node* curr;
    
    unsigned int HashCode(int h) const {
        return h % m_bucketSize;
    }
};


#endif /* MyHash_h */




template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor) : m_numItems(0), m_bucketSize(100), tail(NULL), head(NULL){

    m_loadFactor = (m_numItems / m_bucketSize) * 1.0;
    if (maxLoadFactor <= 0)
        m_maxLoad = 0.5;
    else if (maxLoadFactor > 2)
        m_maxLoad = 2.0;
    else
        m_maxLoad = maxLoadFactor;
    
    hashTable = new Node*[m_bucketSize];
    for (int i = 0; i < m_bucketSize; i++)
        hashTable[i] = NULL;
}

template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::~MyHash() {

    Node* temp;
    Node* tempNext;
    for (int i = 0; i < m_numItems; i++) {
        temp = hashTable[i];
        while (temp != nullptr) {
            tempNext = temp->next;
            delete temp;
            temp = tempNext;
        }
        hashTable[i] = nullptr;
    }
    
    delete hashTable;
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::reset() {
    
    Node* temp;
    Node* tempNext;
    for (int i = 0; i < m_numItems; i++) {
        temp = hashTable[i];
        while (temp != nullptr) {
            tempNext = temp->next;
            delete temp;
            temp = tempNext;
        }
        hashTable[i] = nullptr;
    }
    
    delete hashTable;

    hashTable = new Node*[100];
    for (int i = 0; i < 100; i++)
        hashTable[i] = NULL;
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType &key, const ValueType &value) {
    
    unsigned int hash(const KeyType& k);
    unsigned int h = hash(key);
    unsigned int code = HashCode(h);    // gets hash code for key
    
    if (hashTable[code] == NULL) {      // if there is no item there, add new linked list
        Node* newNode = new Node();     // create new node and add it into the list
        newNode->m_key = key;
        newNode->m_value = value;
        newNode->next = NULL;
        
        hashTable[code] = newNode;
        
        if (head != NULL)               // this is not the first item added
            tail->next = newNode;       // add a copy of that pointer into the back of our separate linked list
        else                            // this is the first item added
            head = newNode;             // head now points to the new node
        tail = newNode;                 // update tail pointer (if linked list was empty, tail is set to the first key
        
        m_numItems++;                   // increment item count
    }
    else {                                                      // slot is occupied
        Node* temp = hashTable[code];
        
        while (temp->m_key != key && temp->next != NULL)        // check if key is already there
            temp = temp->next;
        
        if (temp->next == NULL && temp->m_key != key) {
            Node* newNode = new Node();
            newNode->m_key = key;
            newNode->m_value = value;
            newNode->next = NULL;
            
            temp->next = newNode;           // add new node to the end of list
            tail->next = newNode;           // add copy of that pointer into separate linked list
            tail = newNode;
            
            m_numItems++;                   // increment item count
        }
        else {                              // temp's m_key matches key
            temp->m_value = value;          // if the key is still there, update value
        }
    }
    
    m_loadFactor = (m_numItems / m_bucketSize) * 1.0;
    if (m_loadFactor > m_maxLoad) {
        m_bucketSize = m_bucketSize * 2;
        Node** newhash = new Node*[m_bucketSize];   // create temp table
        
        for (int i = 0; i < m_bucketSize; i++)
            newhash[i] = nullptr;
        
        const Node* n;
        // fill in table with old info
        for (int i = 0; i < m_bucketSize / 2; i++) {
            n = hashTable[i];
            while (n != nullptr) {
                // initialize new node
                Node* tnode = new Node;
                tnode->m_key = n->m_key;
                tnode->m_value = n->m_value;
                tnode->next = nullptr;
                Node*& bucket = newhash[hash(n->m_key) / m_bucketSize];
                tnode->next = bucket;
                bucket = tnode;
                n = n->next;
            }
        }
        
        // delete links
        for (int i = 0; i < m_bucketSize / 2; i++) {
            Node *curr = hashTable[i];
            Node *next;
            while ( curr != nullptr ) {
                next = curr->next;
                delete curr;
                curr = next;
            }
        }
        
        delete []hashTable;
        hashTable = newhash;
    }
}


template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const {

    unsigned int hash(const KeyType& k);
    unsigned int h = hash(key);
    unsigned int code = HashCode(h);    // gets hash code for key
    Node* temp = hashTable[code];
    
    if (temp == NULL)
        return NULL;
    else {
        while (temp->m_key != key) {
            if (temp->next == NULL)
                return NULL;
            temp = temp->next;
        }
        return &(temp->m_value);
    }
}

template <class KeyType, class ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const {
    return m_numItems;
}

template <class KeyType, class ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const {
    return m_loadFactor;
}

