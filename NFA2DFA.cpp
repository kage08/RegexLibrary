/*==============================================================================
Author: Harshavardhan.P.K.
Roll No: CS15B061
Compiler: gcc-6
===============================================================================*/

#include "Resource.h"

uLong convS(uLong i,uLong max){
	if(i==pow(2,max)-1) i=0;
	else i++;
	return i;
}

uLong uconvS(uLong j,uLong max){
	if(j==0) j=pow(2,max)-1;
	else j--;
	return j;
}

set<uLong> getSubset(uLong j,uLong max){
	set<uLong> ans;
	for(uLong i=0;i<max;i++){
		if(((1<<i)&j)>0) ans.insert(i);
	}
	return ans;

}

uLong getNum(set<uLong> post){
	uLong ans=0;
	for(uLong n:post)
		ans+=(1<<n);
	return ans;
}

NFA toDFA(NFA& A){
	NFA ans(pow(2,A.noStates),A.alphabet);
	for(uLong i=0;i<pow(2,A.noStates);i++){
		uLong j=convS(i,A.noStates);
		set<uLong> prev=getSubset(j,A.noStates);
		for(char c:A.alphabet){
			set<uLong> post=A.step(prev,'@');
			post=A.step(post,c);
			ans.addTransition(i,uconvS(getNum(post),A.noStates),c);
		}
		for(uLong f: A.acceptStates){
			if(((1<<f)&j)>0){
				ans.acceptStates.insert(uconvS(j,A.noStates));
			}
		}
	}


	return ans;
}

int main(){
	NFA ans=inputNFA();
	NFA ans2=toDFA(ans);
	cout<<endl;
	printNFA(ans2);
	return 0;
}
