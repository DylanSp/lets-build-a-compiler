# from http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -pedantic-errors -g

SRCDIR = src
TESTSRC = test
BUILDDIR = build
TARGET = bin/interactive_compiler
SRCEXT = cc
SOURCES = $(wildcard $(SRCDIR)/*.$(SRCEXT))
TESTSOURCES = $(wildcard $(TESTDIR)/*.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJS = $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o))

# artifacts from test program
TEST_ARTS = test/src/compiled_out.cc test/bin/*
LIB = 
INC = -I include -I src

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
	