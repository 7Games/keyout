CXX    := g++
CFLAGS := -Wall -Werror -Wextra -pedantic -std=c++26
CLINK  := -lraylib

SOURCES := "src/main.cpp" "src/sound.cpp" "src/ball.cpp"
OUT     := keyout

keyout: src/main.cpp src/sound.cpp src/ball.cpp
	$(CXX) $(CFLAGS) $(CLINK) $(SOURCES) -o $(OUT)

.PHONY: run clean
clean: $(out)
	rm ./$(OUT)
run: $(OUT)
	./$(OUT)
