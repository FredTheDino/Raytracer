SOURCES=$(shell find src -type f -name "*.*")
TARGET=bin/ray

.PHONY: build show run debug

CXX=clang++
build: $(TARGET)

run: $(TARGET)
	$(shell $$(time $(TARGET)))
	feh render.ppm

debug: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm $(TARGET)
	rm render.ppm
	rm tags

$(TARGET): src/main.cpp $(SOURCES) tags
	$(CXX) -std=c++17 -O3 -o $@ src/main.cpp -pthread

tags: $(SOURCES)
	ctags -R src

