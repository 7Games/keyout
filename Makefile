CXX    := g++
CFLAGS := -Wall -Werror -Wextra -pedantic -std=c++26
CLINK  := -lraylib

SOURCES := "src/main.cpp" "src/sound.cpp" "src/ball.cpp" "src/key.cpp"
OUT     := keyout

keyout: src/main.cpp src/sound.cpp src/ball.cpp src/key.cpp
	mkdir -p out
	$(CXX) $(CFLAGS) $(CLINK) $(SOURCES) -o out/$(OUT)

.PHONY: clean run

clean: $(OUT)
	rm ./out/$(OUT)

run: $(OUT)
	./out/$(OUT)
