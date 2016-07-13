# from http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -pedantic-errors -g
SRCEXT = cc

SRCDIR = src
TESTDIR = test/generated
BUILDDIR = build
TESTBUILDDIR = test/build
TARGET = 
SOURCES = $(wildcard $(SRCDIR)/*.$(SRCEXT))
TESTSOURCES = $(wildcard $(TESTDIR)/*.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJS = $(patsubst $(TESTDIR)/%,$(TESTBUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o))

LIB = -pthread 
INC = -I include -I src -I lib/googletest/googletest/include 

#all: $(TARGET)
all: interactive full tests

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)
  
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
  
$(TESTBUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT) 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
  
.PHONY: clean
  
clean:
	@echo " Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TESTBUILDDIR) $(TARGET) $(TESTDIR)

# Tests; builds and runs test_generator, then builds run_tests
tests: test_generator
	mkdir -p $(TESTDIR)
	bin/test_generator
	$(MAKE) run_tests
	
run_tests: $(TESTOBJS)
	$(CC) $(CFLAGS) $(LIB) $(TESTOBJS) lib/googletest/googletest/make/gtest_main.a -o bin/run_tests 
	
# Spikes
interactive: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(INC) $(LIB) -o bin/interactive_compiler spikes/interactive_compiler.cc	
  
full: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(INC) $(LIB) -o bin/full_compiler spikes/full_compiler.cc
	
test_generator: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(INC) $(LIB) -o bin/test_generator spikes/test_generator.cc
	
	