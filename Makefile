SOURCES=$(shell find src -type f -name "*.*")
TARGET=bin/ray

.PHONY: build show run

build: $(TARGET)

run: $(TARGET)
	$(shell $$(time $(TARGET)))
	feh render.ppm

clean:
	rm $(TARGET)
	rm render.ppm

$(TARGET): src/main.cpp $(SOURCES)
	$(CXX) -O2 -o $@ src/main.cpp
