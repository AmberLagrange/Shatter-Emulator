SHELL := /bin/bash

#App name
EXE ?= shatter.exe

#Executables
RM 		:= rm
MKDIR_P := mkdir -p

#Directories
INCDIR  ?= ./include /usr/include/SDL2
SRCDIR  ?= ./src
OBJDIR  ?= ./obj
BUILDIR ?= ./build

#Compiler and flags
CXXFLAGS ?= -Wall -Wextra -Werror
CXXFLAGS += $(INCDIR:%=-iquote %) -MMD -MP -std=c++17 -DEXE_NAME=\"$(EXE)\"

#Libraries
LDFLAGS ?= 
LDLIBS  := -lSDL2main -lSDL2

#Files
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=%.o)
DEPS := $(OBJS:%.o=%.d)

.PHONY: all clean release debug mkdir remake debugger

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
	$(CXX) $(RELOBJS) -o $(BUILDIR)/$(RELDIR)/$(EXE) -fuse-ld=$(LD) $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/$(RELDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#Debug target
debug: mkdir $(DBGOBJS)
	$(CXX) $(DBGOBJS) -o $(BUILDIR)/$(DBGDIR)/$(EXE) -fuse-ld=$(LD) $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/$(DBGDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#Other targets

remake: clean all

debugger: debug
	gdb $(BUILDIR)/$(DBGDIR)/$(EXE)

clean:
	$(RM) -rf $(BUILDIR)
	$(RM) -rf $(OBJDIR)

mkdir:
	$(MKDIR_P) $(BUILDIR) $(BUILDIR)/$(RELDIR) $(BUILDIR)/$(DBGDIR)
	$(MKDIR_P) $(OBJDIR)  $(OBJDIR)/$(RELDIR)  $(OBJDIR)/$(DBGDIR) 

-include $(RELDEPS)
-include $(DBGDEPS)