#include "provided.h"
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const string& s) const;
private:
    string m_text;
    string m_separators;
    
    bool isSep(char c) const {
        string seps(m_separators);
        for (int i = 0; i < seps.size(); i++)
            if (seps[i] == c)
                return true;
        return false;
    }
};

TokenizerImpl::TokenizerImpl(string separators): m_separators(separators) {}


vector<string> TokenizerImpl::tokenize(const string& s) const {
    stringstream sS(s);
    string word;
    vector<string> result;
    char c;
    
    while (sS) {
        word.clear();
        
        while (!isSep((c = sS.get())))
            word.push_back(c);
        if (c != EOF)            
            sS.unget();
        
        result.push_back(word);
        
        while(isSep((c = sS.get())));
        if (c != EOF)
            sS.unget();
    }
    return result;
}


//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
