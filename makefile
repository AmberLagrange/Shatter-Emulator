SHELL := /bin/bash

#App name
EXE ?= shatter.exe

#Executables
RM 		:= rm
MKDIR_P := mkdir -p

#Directories
INCDIR  ?= ./include ./src
SRCDIR  ?= ./src
OBJDIR  ?= ./obj
BUILDIR ?= ./build

#Compiler and flags
CXXFLAGS ?= -Wall -Wextra -Werror
CXXFLAGS += $(INCDIR:%=-iquote %) -MMD -MP -std=c++17 -DEXE_NAME=\"$(EXE)\"

#Libraries
LDLIBS  := -lSDL2 -lSDL2main

#Files
DIRS 	:= audio cart cpu logging video .
SRCS 	:= $(foreach dir, $(DIRS), $(wildcard $(SRCDIR)/$(dir)/*.cpp))
OBJS 	:= $(SRCS:$(SRCDIR)/%.cpp=%.o)
DEPS 	:= $(OBJS:%.o=%.d)

.PHONY: all clean release debug mkdir remake

all: mkdir release

#Release build settings
RELDIR  ?= release
RELOBJS := $(addprefix $(OBJDIR)/$(RELDIR)/, $(OBJS))
RELDEPS := $(RELOBJS:%.o=%.d)
release: CXXFLAGS += -DNDEBUG

#Debug build settings
DBGDIR  ?= debug
DBGOBJS := $(addprefix $(OBJDIR)/$(DBGDIR)/, $(OBJS))
DBGDEPS := $(DBGOBJS:%.o=%.d)
debug: CXXFLAGS += -g

#Release target
release: $(RELOBJS)
	$(CXX) $(RELOBJS) -o $(BUILDIR)/$(RELDIR)/$(EXE) $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/$(RELDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#Debug target
debug: mkdir $(DBGOBJS)
	$(CXX) $(DBGOBJS) -o $(BUILDIR)/$(DBGDIR)/$(EXE) $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/$(DBGDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#Other targets

remake: clean all

clean:
	$(RM) -rf $(BUILDIR)
	$(RM) -rf $(OBJDIR)

mkdir:
	$(MKDIR_P) $(BUILDIR) $(BUILDIR)/$(RELDIR) $(BUILDIR)/$(DBGDIR)
	$(MKDIR_P) $(OBJDIR)  $(OBJDIR)/$(RELDIR)  $(OBJDIR)/$(DBGDIR) 
	$(foreach dir, $(DIRS), $(MKDIR_P) $(OBJDIR)/$(RELDIR)/$(dir) $(OBJDIR)/$(DBGDIR)/$(dir))

-include $(RELDEPS)
-include $(DBGDEPS)