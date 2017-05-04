/*==============================================================================
Author: Harshavardhan.P.K.
Roll No: CS15B061
Compiler: gcc-6
===============================================================================*/

#include <bits/stdc++.h>


using namespace std;

typedef unsigned int uint;
typedef unsigned long long uLong;
typedef long long Long;
class NFA;

class NFA{
  public:
    uLong noStates;
    vector<set<pair<char,uLong> > > Trans;
    set<uLong> acceptStates;
    vector<char> alphabet;

    NFA(uLong nS,vector<char>& alp);

    NFA(vector<char>& alph, char c);

    void addTransition(uLong p, uLong q, char alpha);

    set<uLong> step(set<uLong>& initS, char alpha);
    bool acceptString(string st);
    
};

 NFA operator+(NFA& A,NFA& B);
 NFA Kleeny(NFA& A);
 NFA concat(NFA& A, NFA& B);
 NFA makeNFA(string& regex, set<char>& alpha);
 void printNFA(NFA& A);
 NFA emptyLang(vector<char>& v);
 NFA inputNFA();
 Long DFAtrans(NFA& A,uLong prev, char alpha);
 NFA inputDFA();


string addConcat(string& regex,set<char> alphabet);
string toPostFix(string& regex, set<char> alphabet);
