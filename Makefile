# Lix Makefile
# See ./doc/linux.txt if you want to compile yourself.

CXX      ?= g++
CXXFLAGS ?= -O2 -Wall -Wextra -std=c++0x
LD       = g++ -std=c++0x
PKG_CONFIG ?= pkg-config

STRIP    = strip

DEPGEN   = $(CXX) -MM -std=c++0x
RM       = rm -rf
MKDIR    = mkdir -p

SRCDIR   = src
OBJDIR   = obj
DEPDIR   = $(OBJDIR)
BINDIR   = bin

# verbosity switch
V=0
ifeq ($(V),0)
Q=@
endif

CLIENT_BIN  = $(BINDIR)/hanchan
CLIENT_SRCS = $(wildcard src/*.cpp)
CLIENT_OBJS = $(subst $(SRCDIR)/,$(OBJDIR)/,$(CLIENT_SRCS:%.cpp=%.o))




###############################################################################

.PHONY: all clean

all: $(CLIENT_BIN)

clean:
	$(RM) $(CLIENT_BIN)
	$(RM) $(OBJDIR)
	$(RM) $(DEPDIR)



###############################################################################

# Linux native compilation

$(CLIENT_BIN): $(CLIENT_OBJS)
	$(Q)$(MKDIR) $(BINDIR)
	@echo Linking \`$(CLIENT_BIN)\'
	$(Q)$(LD) $(CXXFLAGS) $(CLIENT_OBJS) -o $(CLIENT_BIN) > /dev/null
	$(Q)$(STRIP) $(CLIENT_BIN)

define MAKEFROMSOURCE
$(Q)$(MKDIR) `dirname $@` `dirname $(DEPDIR)/$*.d`
@echo $<
$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
@printf "%s/%s" `dirname $@` "`$(DEPGEN) $<`" > $(DEPDIR)/$*.d
endef

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(MAKEFROMSOURCE)

-include $(CLIENT_DEPS)
-include $(SERVER_DEPS)
