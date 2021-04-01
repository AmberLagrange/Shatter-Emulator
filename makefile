SHELL := /bin/bash

#App name
EXE ?= shatter
EXT := .exe

#Executables
RM := rm
MKDIR_P := mkdir -p

#Directories
OBJDIR ?= ./obj
SRCDIR ?= ./src
INCDIR ?= ./include
BUILDIR ?= ./build

DIRS := $(OBJDIR) $(BUILDIR)

SRCS := $(wildcard $(SRCDIR)/*.cpp) #Get all cpp files in SRCDIR
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o) #.o file for each .cpp file
DEPS := $(OBJS:%.o=%.d)

#Compiler and flags
CXX ?= g++
CXXFLAGS ?= -g -Wall -Wextra -Werror -I$(INCDIR) -MMD -MP

#Libraries
LIBS := 

.PHONY: clean

#Main executable
$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $(BUILDIR)/$(EXE)$(EXT) $(LIBS)

#Each .o file required 
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) -r $(OBJDIR)
	$(RM) -r $(BUILDIR)

-include $(DEPS)

$(info $(shell $(MKDIR_P) $(DIRS)))