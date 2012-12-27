CC = g++

map_test: ../Map.cpp ../Map.h map_test.cpp
	$(CC) $^ -o $@ -lboost_unit_test_framework -I..	

test: map_test
	./map_test

clean:
	rm map_test
