SHELL := /bin/bash

#App name
EXE ?= shatter.exe

#Executables
RM 		:= rm
MKDIR_P := mkdir -p

#Compiler and flags
CXX ?= g++
CXXFLAGS ?= -Wall -Wextra -I$(INCDIR) -MMD -MP 
#CXXFLAGS += -Werror
CXXFLAGS += -std=c++17
CXXFLAGS += -DEXE_NAME=\"$(EXE)\"

#Libraries
LDFLAGS := -lSDL2main -lSDL2 

#Directories
INCDIR  ?= ./include
SRCDIR  ?= ./src
OBJDIR  ?= ./obj
BUILDIR ?= ./build

#Files
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=%.o)
DEPS := $(OBJS:%.o=%.d)

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
	$(CXX) $(RELOBJS) -o $(BUILDIR)/$(RELDIR)/$(EXE) $(LDFLAGS)

$(OBJDIR)/$(RELDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#Debug target
debug: mkdir $(DBGOBJS)
	$(CXX) $(DBGOBJS) -o $(BUILDIR)/$(DBGDIR)/$(EXE) $(LDFLAGS)

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

-include $(RELDEPS)
-include $(DBGDEPS)