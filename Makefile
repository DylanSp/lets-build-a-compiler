# from http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -pedantic-errors -g

SRCDIR = src
TESTSRC = test/generated
BUILDDIR = build
TARGET = 
SRCEXT = cc
SOURCES = $(wildcard $(SRCDIR)/*.$(SRCEXT))
TESTSOURCES = $(wildcard $(TESTDIR)/*.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJS = $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o))

# artifacts from test program
TEST_ARTS = test/src/compiled_out.cc test/bin/*
LIB = -pthread 
INC = -I include -I src -I lib/googletest/googletest/include 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)
  
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
  
clean:
	@echo " Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET) $(TEST_ARTS)

# Tests
tester: build/compiler.o $(TESTOBJS)
	$(CC) $(CFLAGS) $^ $(INC) $(LIB) -o bin/tester test/tester.cc 
	
# Spikes
interactive: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(INC) $(LIB) -o bin/interactive_compiler spikes/interactive_compiler.cc	
  
full: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(INC) $(LIB) -o bin/full_compiler spikes/full_compiler.cc
	
test_generator: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(INC) $(LIB) -o bin/test_generator spikes/test_generator.cc
	
#tests: $(TESTOBJS) build/SingleConstant.o build/SingleConstantTest.o
#	$(CC) $(CFLAGS) $(LIB) $(TESTOBJS) lib/googletest/googletest/make/gtest_main.a build/SingleConstantTest.o -o bin/run_tests 
	
tests: $(TESTOBJS) build/EmptySource.o build/EmptySourceTest.o
	$(CC) $(CFLAGS) $(LIB) $(TESTOBJS) lib/googletest/googletest/make/gtest_main.a build/EmptySourceTest.o -o bin/run_tests 
	
build/SingleConstant.o: test/generated/SingleConstant.cc
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	
build/SingleConstantTest.o: test/generated/SingleConstantTest.cc build/SingleConstant.o
	$(CC) $(CFLAGS) $(INC) -I test/generated -c -o $@ $<
	
build/EmptySource.o: test/generated/EmptySource.cc
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	
build/EmptySourceTest.o: test/generated/EmptySourceTest.cc build/EmptySource.o
	$(CC) $(CFLAGS) $(INC) -I test/generated -c -o $@ $<
	
testclass:
	$(CC) $(CFLAGS) $(INC) -c test/src/TestClass.cc
	
testclass_test:
	$(CC) $(LIB) lib/googletest/googletest/make/gtest_main.a TestClass.o
	
