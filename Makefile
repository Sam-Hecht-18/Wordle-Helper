CXX = clang++
CXXFLAGS = -Wall -Wextra -Wshadow -Wpedantic

wordle: wordle.cpp
	${CXX} ${CXXFLAGS} -o $@ $^