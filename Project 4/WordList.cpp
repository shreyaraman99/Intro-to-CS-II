#include "provided.h"
#include <string>
#include <vector>
#include "MyHash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    vector<string> goodWords;
    MyHash<int, vector<string>> hashT;
    int makePattern(string s) const {
        int pattern = 0;
        int j = 0;
        MyHash<char, int> letterToNumber;
        for (int i = 0; i < s.size(); i++, j++) {
            if (letterToNumber.find(s[i]) != nullptr)
                pattern = (pattern * 10) + (*letterToNumber.find(s[i]));
            else
                letterToNumber.associate(s[i], j);
        }
        return pattern;
    }
    
};

bool WordListImpl::loadWordList(string filename)
{
    ifstream infile(filename);    // infile is a name of our choosing
    if (!infile)		        // Did opening the file fail?
    {
        cerr << "Error: Cannot open wordlist.txt!" << endl;     // return with failure
        return false;
    }
    
    hashT.reset();
    
    string s;
    while (getline(infile, s))
    {
        for (int i = 0; i < s.size(); i++)
            if ( !( (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') ) && s[i] != 39 )
                break;
            else {
                goodWords.push_back(s);
                hashT.associate(makePattern(s), goodWords);
            }
    }
    return true;

}

bool WordListImpl::contains(string word) const
{
    for (int i = 0; i < word.size(); i++)
        word[i] = tolower(word[i]);
    
    if (hashT.find(makePattern(word)) != nullptr)
        return true;
    return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    
    if (currTranslation.size() != cipherWord.size())
        return vector<string>();
    
    for (int i = 0; i < cipherWord.size(); i++) {
        if ( !( (cipherWord[i] >= 'a' && cipherWord[i] <= 'z') || (cipherWord[i] >= 'A' && cipherWord[i] <= 'Z') ) && cipherWord[i] != 39 )
            return vector<string>();
    }
    
    for (int i = 0; i < currTranslation.size(); i++) {
        if ( !( (currTranslation[i] >= 'a' && currTranslation[i] <= 'z') || (currTranslation[i] >= 'A' && currTranslation[i] <= 'Z') ) && currTranslation[i] != 39 && currTranslation[i] != '?')
            return vector<string>();
    }
    

    
    for (int i = 0; i < hashT.getNumItems(); i++) {
        for (int j = 0; j < currTranslation.size(); j++) {
            if ( (currTranslation[j] >= 'a' && currTranslation[j] <= 'z') || (currTranslation[j] >= 'A' && currTranslation[j] <= 'Z') ) {
                if ( !(cipherWord[j] >= 'a' && cipherWord[j] <= 'z') || (cipherWord[j] >= 'A' && cipherWord[j] <= 'Z'))
                    return vector<string>();
            }
            else if (currTranslation[j] == '?') {
                if ( !(cipherWord[j] >= 'a' && cipherWord[j] <= 'z') || (cipherWord[j] >= 'A' && cipherWord[j] <= 'Z'))
                    return vector<string>();
            }
            else if (currTranslation[j] == 39) {
                if (cipherWord[j] != 39)
                    return vector<string>();
            }
        }
    }
    
    for (int i = 0; i < cipherWord.size(); i++)
        cipherWord[i] = tolower(cipherWord[i]);
    
    vector<string> matches;
    if (hashT.find(makePattern(cipherWord)) != nullptr)
        matches = *hashT.find(makePattern(cipherWord));

    return matches;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}
