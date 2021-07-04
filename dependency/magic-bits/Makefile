CXX = g++

all: magic_bits slider_attacks.o slider_attacks_test

magic_bits: magic_bits.cpp common.h
	$(CXX) -std=c++14 -O2 magic_bits.cpp -o $@

slider_attacks.o: slider_attacks.cpp slider_attacks.h common.h
	$(CXX) -std=c++14 -O2 -c slider_attacks.cpp -o $@

slider_attacks_test: slider_attacks_test.cpp slider_attacks.o
	$(CXX) -std=c++14 -O2 slider_attacks_test.cpp slider_attacks.o -o $@

#clean:
#rm magic_bits slider_attacks.o slider_attacks_test *.magic
