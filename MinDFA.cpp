/*==============================================================================
Author: Harshavardhan.P.K.
Roll No: CS15B061
Compiler: gcc-6
===============================================================================*/

#include "Resource.h"

void NFADFS(NFA& A,uLong par,vector<bool>& accessible){
  for(auto pb: A.Trans[par]){
    if(!accessible[pb.second]){
      accessible[pb.second]=true;
      NFADFS(A,pb.second,accessible);
    }
  }
}

void removeState(NFA& A, uLong st){
  set<pair<char,uLong> >temp;
  for(uLong i=0;i<A.noStates;i++){
    temp.clear();
    for(auto& pb: A.Trans[i]){
      if(pb.second==st);
      else if(pb.second>st){
        auto pb2=pb;
        pb2.second--;
        temp.insert(pb2);
      }
      else temp.insert(pb);

    }
    A.Trans[i].clear();
    for(auto pb: temp)
      A.Trans[i].insert(pb);
  }
  for(uLong i=st;i<A.noStates-1;i++)
    A.Trans[i]=A.Trans[i+1];
  A.Trans.pop_back();
  A.noStates--;
  A.acceptStates.erase(st);
  for(uLong f:A.acceptStates){
    if(f>st){
      A.acceptStates.erase(f);
      A.acceptStates.insert(f-1);
    }
  }
}

void removeInaccess(NFA& A){
  vector<bool> accessible(A.noStates,false);
  accessible[0]=true;
  NFADFS(A,0,accessible);
  for(uLong i=A.noStates-1;i>0;i--){
    if(!accessible[i]) removeState(A,i);
  }

}

NFA minimizeDFA(NFA& A){
  NFA ans=A;
  removeInaccess(ans);
  vector<vector<bool> > stateTable=vector<vector<bool> >(A.noStates,vector<bool>(A.noStates,true));
  for(uLong f: A.acceptStates){
    for(uLong i=0;i<f;i++)
      if(A.acceptStates.find(i)==A.acceptStates.end()) stateTable[f][i]=stateTable[i][f]=false;
  }
  
  while(1){
    bool present=false;
    for(uLong i=0;i<A.noStates;i++)
      for(uLong j=0;j<i;j++){
        if(!stateTable[i][j]) continue;
        for(char c:A.alphabet)
          if(!stateTable[DFAtrans(A,i,c)][DFAtrans(A,j,c)])
            {stateTable[i][j]=stateTable[j][i]=false; present=true;break;}
      }
    if(!present) break;
  }
  uLong ct=A.noStates;

  vector<Long> sets(A.noStates,-1);
  for(uLong i=0;i<A.noStates;i++)
    for(uLong j=0;j<i;j++)
      if(stateTable[i][j]){
        ct--;
        if(sets[j]==-1) sets[i]=j;
        else sets[i]=sets[j];
        break;
      }

  NFA minians(ct,A.alphabet);
  uLong ct2=0;

  vector<uLong> stateMap(A.noStates);
  for(uLong i=0;i<A.noStates;i++){
    if(sets[i]==-1) stateMap[i]=ct2++;
    else stateMap[i]=stateMap[sets[i]];
  }

  for(uLong i=0;i<A.noStates;i++){
  	if(sets[i]==-1)
    	for(auto pb:A.Trans[i])
      		minians.addTransition(stateMap[i],stateMap[pb.second],pb.first);
  }

  for(uLong f:A.acceptStates)
    minians.acceptStates.insert(stateMap[f]);

  return minians;
}

int main(int argc, char const *argv[])
{
  NFA inp=inputDFA();
  NFA ans=minimizeDFA(inp);
  printNFA(ans);
  return 0;
}

