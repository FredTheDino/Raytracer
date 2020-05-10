SOURCES=$(shell find src -type f -name "*.*")
TARGET=bin/ray

.PHONY: build show run

build: $(TARGET)

run: $(TARGET)
	./$(TARGET)	
	feh render.ppm

show: $(TARGET)
	./$(TARGET)	
	feh render.ppm

clean:
	rm $(TARGET)
	rm render.ppm

$(TARGET): src/main.cpp $(SOURCES)
	$(CXX) -o $@ src/main.cpp
