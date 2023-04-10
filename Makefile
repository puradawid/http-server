CXXFLAGS =	-g -Wall -fmessage-length=0 -std=c++11

SRC = $(shell find . -name "*.cpp" -path "./server/src/*")

SRC_OBJS	=	$(SRC:.cpp=.o)

TEST_SRC =	$(shell find . -name "*.cpp" -path "./server/test/*")

TEST_OBJS = $(TEST_SRC:.cpp=.o)

LIBS =		

TEST_LIBS =	-L/usr/lib/x86_64-linux-gnu -lboost_unit_test_framework

TEST_TARGET = unit_tests.a

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

server.o: $(SRC_OBJS) $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC_OBJS) server.cpp -o $@

$(TEST_TARGET): server.o $(TEST_OBJS)
	$(CXX) -o $(TEST_TARGET) $(SRC_OBJS) $(TEST_OBJS) $(LIBS) $(TEST_LIBS)

build_tests: $(TEST_TARGET)

all:	test
	make $(TARGET)

test: $(TEST_TARGET)
	SERVER_LOGGER=stdout ./unit_tests.a

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_TARGET) server.o
	
	rm -f $(SRC_OBJS)
