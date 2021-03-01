CXX = g++
CPPFLAGS = -w -Wall -pedantic -Wextra -std=c++17
DEBUG = -g -fsanitize=address
RELEASE = -O3
IMPORTS = -I"./vendor/rapidjson/include" -I"./vendor/scopeGuard"
TARGETS = interpreter interpreter_debug

all: run

build_release:
	$(CXX) $(CPPFLAGS) $(RELEASE) $(IMPORTS) \
	./src/*.cpp \
	./src/visitors/*.cpp \
	-o interpreter;

build_debug:
	$(CXX) $(CPPFLAGS) $(DEBUG) $(IMPORTS) \
	./src/*.cpp \
	./src/visitors/*.cpp \
	-o interpreter_debug;

clean:
	rm -f $(TARGETS) *.o;

run: build_release
	./interpreter -r ../parsed.json;

debug: build_debug
	./interpreter_debug -r ../parsed.json;