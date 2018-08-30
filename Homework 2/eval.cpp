
#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool isOperator(const char operand);
bool validOperator(const char operand);
bool checkOrder(char one, char two);
bool isValidInfix(string infix);
bool checkOperands(string infix, const Map& values);
string convertInfixToPostfix(string infix);


int evaluate(string infix, const Map& values, string& postfix, int& result){
    
    if (!isValidInfix(infix))
        return 1;
    postfix = convertInfixToPostfix(infix);
    
    if (!checkOperands(infix, values))
        return 2;
    
    stack<int> valStack;
    int answer = 0;
    
    for (int i = 0; i < postfix.size(); i++) {
        char t = postfix[i];
        if (isalpha(t)) {
            int temp = 0;
            values.get(t, temp);
            valStack.push(temp);
        }
        else {
            int a2 = valStack.top();
            valStack.pop();
            int a1 = valStack.top();
            valStack.pop();
            
            switch (t) {
                case '+':
                    answer = a1 + a2;
                    break;
                case '-':
                    answer = a1 - a2;
                    break;
                case '*':
                    answer = a1 * a2;
                    break;
                case '/':
                    if (a2 == 0)
                        return 3;
                    answer = a1 / a2;
                    break;
            }
            valStack.push(answer);
        }
    }
    result = valStack.top();
    return 0;
}
    
bool isValidInfix(string infix) {
    char t = ' ';
    int countLetters = 0, countOperators = 0, countOpeners = 0, countClosers = 0;
    
    if(infix.size() == 0)
        return false;
    if(infix.size() == 1 && islower(infix[0]) && isalpha(infix[0]))
        return true;
    
    for (int i = 0; i < infix.size(); i++) {
        t = infix[i];
        if(isalpha(t) && !islower(t))
            return false;
        if(!isalpha(t) && !validOperator(t) && t != ' ')
            return false;
        
        if (t == '(')
            countOpeners++;
        if (t == ')')
            countClosers++;
        if (isOperator(t))
            countOperators++;
        if (isalpha(t))
            countLetters++;
    }
    
    if (countLetters - countOperators != 1 || countOpeners != countClosers || (countOpeners >= 1 && countClosers >= 1 && countLetters == 0))
        return false;
    
    string noSpace = "";
    for (int i = 0; i < infix.size(); i++) {
        char t = infix[i];
        if (isalpha(t) || isOperator(t))
            noSpace += t;
    }
    if (noSpace.size() == 0)
        return false;
    
    char first, second;
    for (int i = 0; i < noSpace.size() - 1; i++) {
        first = noSpace[i];
        second = noSpace[i + 1];
        if ((isalpha(first) && isalpha(second)) || (!isalpha(first) && !isalpha(second)))
            return false;
    }
    
    if ((!isalpha(noSpace[0]) && t != '(') || (!isalpha(noSpace[noSpace.size() - 1]) && t != ')'))
        return false;
    
    string noSpace1 = "";
    for (int i = 0; i < infix.size(); i++) {
        char t = infix[i];
        if (isalpha(t) || validOperator(t))
            noSpace1 += t;
    }
    char first1, second1;
    for (int i = 0; i < noSpace1.size() - 1; i++) {
        first1 = noSpace1[i];
        second1 = noSpace1[i + 1];
        if (first1 == '(' && second1 == ')')
            return false;
    }
    return true;
}
 

bool isOperator(const char operand) {
    if (operand == '+' || operand == '-' || operand == '*' || operand == '/')
        return true;
    return false;
}

bool validOperator(const char operand) {
    if (isOperator(operand) || operand == '(' || operand == ')')
        return true;
    return false;
}


bool checkOperands(string infix, const Map& values) {
    for (int i = 0; i < infix.size(); i++) {
        char t = infix[i];
        if(!values.contains(t) && isalpha(t))
            return false;
    }
    return true;
}


bool checkOrder(char one, char two) {
    if ((one == '*' || one == '/') && (two == '+' || two == '-'))
        return false;
    return true;
}


string convertInfixToPostfix(string infix) {
    string postfix = "";
    stack<char> postStack;
    
    for(int i = 0; i < infix.size(); i++) {
        char t = infix[i];
        
        if (isalpha(t)) {
            postfix += t;
            continue;
        }
        if (t == ' ')
            continue;
        switch(t) {
            case '(':
                postStack.push(t);
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                while (!postStack.empty() && postStack.top() != '(' && checkOrder(t, postStack.top())) {
                    postfix += postStack.top();
                    postStack.pop();
                }
                postStack.push(t);
                break;
            case ')':
                while (!postStack.empty() && postStack.top() != '(') {
                    postfix += postStack.top();
                    postStack.pop();
                }
                postStack.pop();
                break;
        }
    }
    while (!postStack.empty()) {
        postfix += postStack.top();
        postStack.pop();
    }   
    return postfix;
}


int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
           pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
           pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
           pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
           pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
}
