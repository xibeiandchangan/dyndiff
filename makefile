CXXFLAGS = -Wall

SRC=$(wildcard src/*.cpp)

dyndiff: $(SRC)
	arm-linux-gnueabihf-gcc -fPIE -pie -o $@ $^ $(CXXFLAGS) -I include
clean:
	rm dyndiff








