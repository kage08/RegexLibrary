/*==============================================================================
Author: Harshavardhan.P.K.
Roll No: CS15B061
Compiler: gcc-6
===============================================================================*/

#include "Resource.h"

int main(){
  set<char> alphabet;
  string line;
  getline(cin,line);
  for(char c:line){
    if(!(c==' ' || c=='\n')) alphabet.insert(c);
  }
  cin>>line;
  string ln;
  for(char c:line) if(c!=' ') ln.append(string(1,c));
  line=ln;
  //cout<<line<<endl;
  line=addConcat(line,alphabet);
  //cout<<line<<endl;
  line=toPostFix(line,alphabet);
  //cout<<line<<endl;
  NFA C=makeNFA(line,alphabet);
  printNFA(C);
  return 0;
}
