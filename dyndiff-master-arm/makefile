CXXFLAGS = -Wall

SRC=$(wildcard src/*.cpp)

dyndiff: $(SRC)
	armv8l-linux-gnueabihf-gcc -fPIE -pie -o $@ $^ $(CXXFLAGS) -I include
clean:
	rm dyndiff
