/*==============================================================================
Author: Harshavardhan.P.K.
Roll No: CS15B061
Compiler: gcc-6
===============================================================================*/


#include <bits/stdc++.h>
#include "Resource.h"

    NFA::NFA(uLong nS,vector<char>& alp){
      noStates=nS;
      Trans=vector<set<pair<char,uLong> > >(noStates,set<pair<char,uLong> >());
      alphabet=alp;
    }

    NFA::NFA(vector<char>& alph, char c){
      if(find(alph.begin(),alph.end(),c)==alph.end() && c!='@'){
        cerr<<"Invalid initialization of simple NFA\n";
        exit(0);
      }


      noStates=2;

      Trans=vector<set<pair<char,uLong> > >(noStates,set<pair<char,uLong> >());
      alphabet=alph;
      addTransition(0,1,c);
      acceptStates.insert(1);
    }

    void NFA::addTransition(uLong p, uLong q, char alpha){
      Trans[p].insert(make_pair(alpha,q));
    }

    
NFA emptyLang(vector<char>& alph){
  NFA ans(0,alph);
  return ans;
}

set<uLong> NFA::step(set<uLong>& initS, char alpha){
  set<uLong> finalS;
  if(alpha=='@') finalS=initS;
  for(uLong u:initS){
    set<uLong> temp;
    for(auto pb:Trans[u]){
      if(pb.first==alpha){
        finalS.insert(pb.second);
        temp.insert(pb.second);
      }
    }
    while(!(temp.size()==0)){
      int v=(*(temp.begin()));
      temp.erase(temp.begin());
      for(auto pb:Trans[v]){
        if(pb.first=='@' && finalS.find(pb.second)==finalS.end()){
          temp.insert(pb.second);finalS.insert(pb.second);}
      }
    }
  }
  return finalS;
}

NFA operator+(NFA& A, NFA& B){
  NFA C(A.noStates+B.noStates+1,A.alphabet);
  C.addTransition(0,1,'@');
  for(uLong i=0;i<A.noStates;i++){
    for(auto pb: A.Trans[i])
      C.addTransition(i+1,pb.second+1,pb.first);
  }
  C.addTransition(0,A.noStates+1,'@');
  for(uLong i=0;i<B.noStates;i++){
    for(auto pb: B.Trans[i])
      C.addTransition(i+A.noStates+1,pb.second+A.noStates+1,pb.first);
  }
  for(uLong f:A.acceptStates)
    C.acceptStates.insert(f+1);
  for(uLong f: B.acceptStates)
    C.acceptStates.insert(f+1+A.noStates);
  return C;
}

NFA Kleeny(NFA& A){
  NFA B=A;
  for(uLong f:B.acceptStates)
    B.addTransition(f,0,'@');
  B.acceptStates.insert(0);
  return B;
}

NFA concat(NFA& A, NFA& B){
  NFA C(A.noStates+B.noStates,A.alphabet);
  for(uLong i=0;i<A.noStates;i++){
    for(auto pb: A.Trans[i])
      C.addTransition(i,pb.second,pb.first);
  }
  for(uLong i=0;i<B.noStates;i++){
    for(auto pb: B.Trans[i])
      C.addTransition(i+A.noStates,pb.second+A.noStates,pb.first);
  }
  for(uLong f:A.acceptStates)
    C.addTransition(f,A.noStates,'@');
  for(uLong f: B.acceptStates)
    C.acceptStates.insert(f+A.noStates);

  return C;
}
NFA makeNFA(string& regex,set<char>& alpha){
  stack<NFA> stNFA;
  vector<char> alphac;
  for(char c:alpha)
    alphac.push_back(c);
  for(char c:regex){
    if(alpha.find(c)!=alpha.end() || c=='@')
      stNFA.push(NFA(alphac,c));
    else if(c=='%')
      stNFA.push(emptyLang(alphac));
    else if(c=='*'){
      NFA temp=Kleeny(stNFA.top());
      stNFA.pop();
      stNFA.push(temp);
    }
    else if(c=='.'){
      NFA temp=stNFA.top();
      stNFA.pop();
      NFA temp2=concat(stNFA.top(),temp);
      stNFA.pop();
      stNFA.push(temp2);
    }
    else if(c=='+'){
       NFA temp=stNFA.top();
      stNFA.pop();
      NFA temp2=(stNFA.top()+temp);
      stNFA.pop();
      stNFA.push(temp2);

    }
  }
  return stNFA.top();

}

void printNFA(NFA& A){
  cout<<A.noStates<<endl;
  for(char c:A.alphabet)
    cout<<c<<" ";
  cout<<endl;
  for(uLong f: A.acceptStates)
    cout<<f<<" ";
  cout<<endl;
  for(uLong i=0;i<A.noStates;i++){
    for(auto pb: A.Trans[i])
      cout<<i<<" "<<pb.second<<" "<<pb.first<<endl;
  }
}

bool NFA::acceptString(string st){
  set<uLong> currStates;
  currStates.insert(0);
  currStates=step(currStates,'@');
  for(char c:st){
    currStates=step(currStates,c);
  }
  for(uLong f: acceptStates)
    if(currStates.find(f)!=currStates.end()) return true;
  return false;
}


string addConcat(string& regex,set<char> alphabet){
  if(regex=="%") return regex;
  string ans="";
  uint len=regex.length();
  for(uLong i=0;i<len;i++){
    if(alphabet.find(regex[i])!=alphabet.end() || regex[i]=='@' || regex[i]=='(' || regex[i]=='%'){
      if(i>0 && (regex[i-1]!='+' && regex[i-1]!='(')) ans.append(".");
    }
    ans.append(string(1,regex[i]));
  }
  return ans;
}

string toPostFix(string& regex, set<char> alphabet){
  if(regex=="%") return regex;
  stack<char> opst;
  string ans="";
  for(char c:regex){
    if(alphabet.find(c)!=alphabet.end() || c=='@' || c=='%')
      ans.append(string(1,c));
    else if(c=='(')
      opst.push(c);
    else if(c==')'){
      while(!opst.empty() && opst.top()!='('){
        ans.append(string(1,opst.top()));
        opst.pop();
      }
      opst.pop();
    }
    else if(c=='*')
      opst.push('*');
    else if(c=='.'){
      while(!opst.empty() && opst.top()=='*'){
        ans.append(string(1,'*'));
        opst.pop();
      }
      opst.push(c);
    }
    else if(c=='+'){

      while(!opst.empty() && (opst.top()=='*' || opst.top()=='.')){
        ans.append(string(1,'*'));
        opst.pop();
      }
      opst.push(c);
    }
  }

  while(!opst.empty()){
    ans.append(string(1,opst.top()));
    opst.pop();
  }
  return ans;
}


NFA inputNFA(){
  int nS;
  std::vector<char> alpha;
  string line;
  cin>>nS;
  getline(cin,line);
  getline(cin,line);
  for(char c:line)
    if(!(c==' '||c=='\n')) alpha.push_back(c);
  NFA ans(nS,alpha);
  getline(cin,line);
  istringstream is {line};
  while(is>>nS){
    ans.acceptStates.insert(nS);
  }
  int p,q; char al;
  while(cin>>p>>q>>al)
    ans.Trans[p].insert(make_pair(al,q));
  return ans;
}


Long DFAtrans(NFA& A,uLong prev, char alpha){
  for(auto pb: A.Trans[prev])
    if(pb.first==alpha) return pb.second;
  return LONG_MAX;
}

NFA inputDFA(){
  int nS;
  std::vector<char> alpha;
  string line;
  cin>>nS;
  getline(cin,line);
  getline(cin,line);
  for(char c:line)
    if(!(c==' '||c=='\n')) alpha.push_back(c);
  NFA ans(nS,alpha);
  getline(cin,line);
  istringstream is {line};
  while(is>>nS){
    ans.acceptStates.insert(nS);
  }
  int p,q; char al;
  uLong ct=0;
  while((ct++)<ans.noStates*ans.alphabet.size() && cin>>p>>q>>al)
    ans.Trans[p].insert(make_pair(al,q));
  return ans;
}
