CXX=g++
CXX_FLAGS=-Wall -Wextra -lncurses

OUT=metronome

FILES=main.cpp

all:
	$(CXX) $(CXX_FLAGS) -o $(OUT) $(FILES)

clean:
	rm $(OUT)
