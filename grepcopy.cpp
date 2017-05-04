/*==============================================================================
Author: Harshavardhan.P.K.
Roll No: CS15B061
Compiler: gcc-6
===============================================================================*/

#include "Resource.h"


int main(int argc, char const *argv[])
{
	if(argc<=2){
    cerr<<"Format: \"[REGEX]\" [filename]"<<endl;
    exit(1);
  }
  
  set<char> alphabet;
  for(char c='a';c<='z';c++) alphabet.insert(c);
  for(char c='A';c<='Z';c++) alphabet.insert(c);
  string reg=argv[1];
  //cin>>reg;
  reg=addConcat(reg,alphabet);
  reg=toPostFix(reg,alphabet);
  NFA ans=makeNFA(reg,alphabet);
  string x=argv[2];
  ifstream inp;
  inp.open(x,ios_base::in);

  if(!inp){
    cerr<<"Can't open file "<<x<<endl;
    exit(0);
  }
  string input;
  string temp;
  //set<char> delimiters={' ','\n','.',';',',',':','?','\\','/'};

  while(inp>>temp){
    temp.append(string(1,'.'));
    
    input="";
    for(char c:temp){
      if(isalpha(c)) input.append(string(1,c));
      else{
        if(input.length()>0 && ans.acceptString(input)) cout<<input<<endl;
        input="";
      }

    }
  }
  

  return 0;
}