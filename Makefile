CXX    := g++
CFLAGS := -Wall -Werror -Wextra -pedantic -std=c++26
CLINK  := -lraylib

SOURCES := "src/main.cpp" "src/sound.cpp"
OUT     := keyout

keyout: src/main.cpp src/sound.cpp
	$(CXX) $(CFLAGS) $(CLINK) $(SOURCES) -o $(OUT)

.PHONY: run
run: $(OUT)
	./$(OUT)
