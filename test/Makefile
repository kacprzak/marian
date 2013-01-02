CC = g++
CFLAGS = -Wall -Wextra -g -std=c++0x

VPATH=../base64

test: map_test math_test
	./map_test
	./math_test

map_test: ../TmxMap.cpp ../TmxMap.h map_test.cpp base64.cpp
	$(CC) $^ -o $@ $(CFLAGS) -lboost_unit_test_framework -I..	

math_test: ../Math.h math_test.cpp
	$(CC) $^ -o $@ $(CFLAGS) -lboost_unit_test_framework -I..	

clean:
	rm -f map_test
