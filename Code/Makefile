CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic 
LDFLAGS = -lm

ifeq ($(DEBUG),yes)
    CXXFLAGS += -g
else
    CXXFLAGS += -O3 -DNDEBUG
endif

EXEC = crr
TEST_EXEC = test

SRC = node.cpp crr.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

TEST_SRC = test.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(TEST_EXEC): $(TEST_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

doc:
	@mkdir -p documentation
	@if [ ! -f Doxyfile ]; then doxygen -g Doxyfile; fi
	@sed -i 's|^OUTPUT_DIRECTORY.*|OUTPUT_DIRECTORY = documentation|' Doxyfile
	@sed -i 's|^EXTRACT_ALL.*|EXTRACT_ALL = YES|' Doxyfile
	@sed -i 's|^RECURSIVE.*|RECURSIVE = YES|' Doxyfile
	@sed -i 's|^PROJECT_NAME.*|PROJECT_NAME = "Modèle Binomial CRR"|' Doxyfile
	@doxygen Doxyfile

clean:
	@rm -f *.o $(EXEC) $(TEST_EXEC)

distclean: clean
	@find . -type f ! -name "*.cpp" ! -name "*.hpp" ! -name "makefile" ! -name "Doxyfile" -delete
	@rm -rf documentation/

.PHONY: all clean distclean doc