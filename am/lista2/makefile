# Kompilator i flagi
CXX = g++
CXXFLAGS = -g -O3 -fopenmp -g -Wall -std=c++17 
INPUT_FILES = $(wildcard input/*.txt);

# Nazwa pliku wykonywalnego
TARGET = main
OBJS = main.o utils.o

# Reguła domyślna
all: $(TARGET)

# Linkowanie plików obietkowych w jeden program
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Kompilacja main.cpp 
main.o: main.cpp utils.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Kompilacja utils.cpp
utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp

# Czyszczenie plików binarnych
clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET) input/westernSahara.txt input/djibouti.txt input/qatar.txt input/uruguay.txt input/zimbabwe.txt input/oman.txt input/canada.txt input/tanzania.txt input/egypt.txt input/ireland.txt

tanzania: $(TARGET)
	./$(TARGET) input/tanzania.txt 

egypt: $(TARGET)
	./$(TARGET) input/egypt.txt 

ireland: $(TARGET)
	./$(TARGET) input/ireland.txt