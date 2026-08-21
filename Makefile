CXX := g++
CXXFLAGS := -Ofast

.PHONY: all clean

all: decoder main

decoder: decode/decode.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

main: source/main.cpp source/hash.cpp source/symbolcounter.cpp source/treegen.cpp
	$(CXX) $(CXXFLAGS) -pthread -o $@ $^

clean:
	rm -f decoder main