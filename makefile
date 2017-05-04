CXXFLAGS= -std=c++11 -Wall -O
CXX= g++

.cpp.o:
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $<

cs15b061a: Resource.o REGEX2NFA.o
	$(CXX) $(CXXFLAGS) Resource.o REGEX2NFA.o -o $@

cs15b061b: Resource.o NFA2DFA.o
	$(CXX) $(CXXFLAGS) Resource.o NFA2DFA.o -o $@

cs15b061c: Resource.o MinDFA.o
	$(CXX) $(CXXFLAGS) Resource.o MinDFA.o -o $@

cs15b061d: Resource.o acceptDFA.o
	$(CXX) $(CXXFLAGS) Resource.o acceptDFA.o -o $@

cs15b061e: Resource.o grepcopy.o
	$(CXX) $(CXXFLAGS) Resource.o grepcopy.o -o $@

all: cs15b061d cs15b061b cs15b061c cs15b061a cs15b061e

clean:
	-rm -f cs15b061* *.o