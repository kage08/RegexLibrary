/*==============================================================================
Author: Harshavardhan.P.K.
Roll No: CS15B061
Compiler: gcc-6
===============================================================================*/

#include "Resource.h"

int main(){
  NFA ans=inputDFA();
  string x;
  cin>>x;
  uLong present=0;
  for(char c:x)
    present=DFAtrans(ans,present,c);
  if(ans.acceptStates.find(present)!=ans.acceptStates.end())
    cout<<"YES\n";
}