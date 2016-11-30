# binary output name
OUT := tekvisa

# additional args for the compiler
ARGS := -Wall -W -g -I include -pedantic


build: dirs
	g++ -o bin/$(OUT) $(ARGS) src/*.cc

dirs:
	mkdir -p src bin include

# debug:
# 	gdb bin/tekvisa \
# 		-ex set substitude-path \
# 		/build/gcc/src/gcc-build/x86_64-unknown-linux-gnu/libstdc++-v3/include \
# 		/usr/include/c++/5.2.0

clean:
	rm bin/*
