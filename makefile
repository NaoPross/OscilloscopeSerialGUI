# binary output name
OUT := tekvisa

# additional args for the compiler
ARGS := -Wall -W -g -I include


build: dirs
	g++ -o bin/$(OUT) $(ARGS) src/*.cc

dirs:
	mkdir -p src bin include

clean:
	rm bin/*
