#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    Translator* m_translator;
    Tokenizer* m_tokenizer;
    vector<string> helperCrack(string cipher, vector<string>& v) {
        bool cracked = true;
        string translationText = m_translator->getTranslation(cipher);
        for (int i = 0; i < translationText.size(); i++) {
            if (translationText[i] == '?')
                cracked = false;
        }
        
        vector<string> tokenizedCipher = m_tokenizer->tokenize(cipher);
        vector<string> tokenizedTranslation = m_tokenizer->tokenize(translationText);
        
        vector<string> crackedWords;
        
        if (cracked)    // if sentence is cracked
            return tokenizedTranslation;
        
        else { // if sentence isn't cracked
            vector<string>::iterator it;
            int qCount = 0;
            for (it = tokenizedCipher.begin(); it != tokenizedCipher.end(); it++) {
                for (int i = 0; i < (*it).size(); i++) {
                    if ((*it)[i] == '?')
                        qCount++;
                }
            }
    
            /*
             get the word with the most question marks
             find all the possible candidates for that word
             for each candidate
                push the mapping
                translate the rest of the cipher
                    if the mapping gives a bad word, pop that mapping
                    if it translates the entire word, add the translation to the vector crackedWords and pop the mapping
                    if the cipher is partially translated, crack the cipher
             
             */
        }
            return crackedWords;
    }
};

DecrypterImpl::DecrypterImpl() {}

bool DecrypterImpl::load(string filename)
{
    WordList wl;
    if (wl.loadWordList(filename))
        return true;
    return false;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    vector<string> cracked;
    return helperCrack(ciphertext, cracked);
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}
