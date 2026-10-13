CXX    := g++
CFLAGS := -Wall -Werror -Wextra -pedantic -std=c++26
CLINK  := -lraylib

SOURCES := "main.cpp"
OUT     := "keyout"

keyout: main.cpp
	$(CXX) $(CFLAGS) $(CLINK) $(SOURCES) -o $(OUT)

.PHONY: run
run: keyout
	./$(OUT)
