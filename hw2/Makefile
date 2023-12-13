LLVM_CONFIG=../llvm_build/bin/llvm-config

CXX=`$(LLVM_CONFIG) --bindir`/clang
CXXFLAGS=`$(LLVM_CONFIG) --cppflags` -fPIC -fno-rtti
LDFLAGS=`$(LLVM_CONFIG) --ldflags`
IRFLAGS=-Xclang -disable-O0-optnone -fno-discard-value-names -S -emit-llvm
OPT=`$(LLVM_CONFIG) --bindir`/opt

ll:=test1 test2 test3 test4
target:=test4.ll


.PHONY: all test run clean 
all: hw2.so test

test: ${ll}

${ll}: *.c
	$(CXX) $(IRFLAGS) -o $@.ll $@.c

hw2.so: hw2.cpp statement.h
	$(CXX) -shared -o $@ $< $(CXXFLAGS) $(LDFLAGS)

# test1.ll: test1.c
# 	$(CXX) $(IRFLAGS) -o $@ $<

run: ${target} hw2.so
	$(OPT) -disable-output -load-pass-plugin=./hw2.so -passes=hw2 $<

clean:
	rm -f *.o *.ll *.so
