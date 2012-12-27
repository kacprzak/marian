CC = g++
CFLAGS = -Wall -Wextra -g -std=c++0x

map_test: ../TmxMap.cpp ../TmxMap.h map_test.cpp
	$(CC) $^ -o $@ $(CFLAGS) -lboost_unit_test_framework -I..	

test: map_test
	./map_test

clean:
	rm map_test
