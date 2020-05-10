SOURCES=$(shell find src -type f -name "*.*")
TARGET=bin/ray

.PHONY: build

build: $(TARGET)

show: $(TARGET)
	./$(TARGET)	
	feh render.ppm

clean:
	rm $(TARGET)
	rm render.ppm

$(TARGET): src/main.cpp $(SOURCES)
	$(CXX) -o $@ src/main.cpp
