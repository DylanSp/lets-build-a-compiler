# from http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

CC = g++
CFLAGS = -std=c++11 -Wall -Wextra

SRCDIR = src
TESTSRC = test
BUILDDIR = build
TARGET = bin/interactive_compiler
SRCEXT = cc
SOURCES = $(wildcard $(SRCDIR)/*.$(SRCEXT))
TESTSOURCES = $(wildcard $(TESTDIR)/*.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJS = $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o))
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
	$(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester: build/compiler.o $(TESTOBJS)
	$(CC) $(CFLAGS) $^ $(INC) $(LIB) -o bin/tester test/tester.cc 
	