#include "provided.h"
#include <string>
using namespace std;

class TranslatorImpl
{
public:
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
};

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    if (ciphertext.size() != plaintext.size())
        return false;
    for (int i = 0; i < ciphertext.size(); i++)
        if ( !( (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') || (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') ) )
            return false;
    for (int i = 0; i < plaintext.size(); i++)
        if ( !( (plaintext[i] >= 'a' && plaintext[i] <= 'z') || (plaintext[i] >= 'A' && plaintext[i] <= 'Z') ) )
            return false;
    
    return true;
}


bool TranslatorImpl::popMapping()
{
    /*
     if # of times popMapping() returns true == # of times pushMapping() returns true
        return false
     else
        pop most-recently pushed mapping table
        make it current mapping table
        return true
     */
    
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    /* The getTranslation() function translates its argument string according to the current
     mapping table and returns the resulting string of the same length. Each character in the
     ciphertext argument results in a character appearing in the corresponding position of the
     result string according to the following:
     • If the ciphertext character is a letter that maps to a plaintext letter in the current
     mapping, then that plaintext letter appears, in the same case as in the ciphertext
     string. (Thus, if DàE in the current mapping, ciphertext D results in E, while
     ciphertext d results in e.
     • If the ciphertext character is a letter with an unknown translation in the current
     mapping, a ? appears.
     • If the ciphertext character is not a letter, that character appears, unchanged. */
    
    return "";
}



//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
