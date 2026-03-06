CXX = g++
CXXFLAGS = -std=c++17 -Wextra -Wall -pedantic 
LDFLAGS = -lm

ECHO = @
ifeq ($(VERBOSE),1)
    ECHO=
endif

ifeq ($(DEBUG),yes)
    CXXFLAGS += -g
else
    CXXFLAGS += -O3 -DNDEBUG
endif

EXEC = crr
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

all: 
ifeq ($(DEBUG),yes)
	@echo "Generating in debug mode"
else
	@echo "Generating in release mode"
endif
	@$(MAKE) $(EXEC)

$(EXEC): $(OBJ)
	$(ECHO)$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(ECHO)$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	$(ECHO)rm -rf *.o

mrproper: clean
	$(ECHO)rm -rf $(EXEC) documentation/html

doc:
	$(ECHO)doxygen documentation