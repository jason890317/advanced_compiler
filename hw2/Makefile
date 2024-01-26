LLVM_CONFIG=../llvm_build/bin/llvm-config

CXX=`$(LLVM_CONFIG) --bindir`/clang
CXXFLAGS=`$(LLVM_CONFIG) --cppflags` -fPIC -fno-rtti
LDFLAGS=`$(LLVM_CONFIG) --ldflags`
IRFLAGS=-Xclang -disable-O0-optnone -fno-discard-value-names -S -emit-llvm
OPT=`$(LLVM_CONFIG) --bindir`/opt

ll:=icpp icpp2 icpp3 foo # the testcase list. If there is any new testcase, just put the name of the new testcase here

target:=foo.ll # the one sent to run. modify the target, when wanting to change to another testcase


.PHONY: all test run clean 
all: hw2.so test 



test: ${ll} #make all testcases
${ll}: *.c 
	$(CXX) $(IRFLAGS) -o $@.ll $@.c



hw2.so: hw2.cpp 
	$(CXX) -shared -o $@ $< $(CXXFLAGS) $(LDFLAGS)



run: ${target} hw2.so #run the target .ll file with hw2.so
	$(OPT) -disable-output -load-pass-plugin=./hw2.so -passes=hw2 $<



clean:
	rm -f *.o *.ll *.so
