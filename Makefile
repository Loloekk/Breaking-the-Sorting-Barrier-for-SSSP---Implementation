CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++23 -MMD -MP \
           -Isrc/solution -Isrc/test_generators -Isrc/test_stats -Isrc/tests -Isrc/utils

TEST_GEN_SRCS = $(wildcard src/test_generators/*.cpp)
TEST_STAT_SRCS = $(wildcard src/test_stats/*.cpp)

TEST_GEN_OBJS = $(TEST_GEN_SRCS:.cpp=.o)
TEST_STAT_OBJS = $(TEST_STAT_SRCS:.cpp=.o)

COMMON_OBJS = $(TEST_GEN_OBJS) $(TEST_STAT_OBJS)

APP_SRC = src/main.cpp
APP_OBJ = $(APP_SRC:.cpp=.o)
APP_BIN = main_app.bin

CORRECTNESS_SRC = src/tests/SolutionsCorrectenessTest.cpp
CORRECTNESS_OBJ = $(CORRECTNESS_SRC:.cpp=.o)
CORRECTNESS_BIN = SolutionsCorrectenessTest.test

SPEED_SRC = src/tests/SolutionsSpeedTest.cpp
SPEED_OBJ = $(SPEED_SRC:.cpp=.o)
SPEED_BIN = SolutionsSpeedTest.test

UNIT_TEST_SRCS = $(wildcard src/tests/UnitTests/*.cpp)
UNIT_TEST_OBJS = $(UNIT_TEST_SRCS:.cpp=.o)
UNIT_TEST_BINS = $(notdir $(UNIT_TEST_SRCS:.cpp=.test))

ALL_OBJS = $(COMMON_OBJS) $(APP_OBJ) $(CORRECTNESS_OBJ) $(SPEED_OBJ) $(UNIT_TEST_OBJS)
DEPS = $(ALL_OBJS:.o=.d)

.PHONY: all clean run run_tests run_correctness_test run_speed_test run_unit_tests

all: $(APP_BIN) $(CORRECTNESS_BIN) $(SPEED_BIN) $(UNIT_TEST_BINS)

$(APP_BIN): $(APP_OBJ) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CORRECTNESS_BIN): $(CORRECTNESS_OBJ) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(SPEED_BIN): $(SPEED_OBJ) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.test: src/tests/UnitTests/%.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(APP_BIN)
	./$(APP_BIN)

run_correctness_test: $(CORRECTNESS_BIN)
	./$(CORRECTNESS_BIN)

run_speed_test: $(SPEED_BIN)
	./$(SPEED_BIN)

run_unit_tests: $(UNIT_TEST_BINS)
	@for test in $(UNIT_TEST_BINS); do \
		./$$test; \
	done

clean:
	rm -f $(APP_BIN) *.test
	find src -type f \( -name "*.o" -o -name "*.d" \) -delete

-include $(DEPS)