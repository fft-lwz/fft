TARGET = fftw_test

CXXFLAGS += -std=gnu++11
CC = g++
CXX_FILES := ./fftw-test.cpp
# fftw3 安装路径
INC_DIRS := -I ~/fftw-build/include
STATIC_LIB += -L ~/fftw-build/lib

LIBS += -lstdc++
LIBS += -ldl -lpthread
LIBS += -lfftw3


$(TARGET):$(CXX_FILES)
	$(info target: $@)
	$(info all: $^)
	$(info first: $<)
	$(info SRC_DIRS_all: $(SRC_DIRS))
	$(CC) -o $@ $^ $(INC_DIRS) $(STATIC_LIB) $(CXXFLAGS) $(LIBS)


.PHONY:clean
clean:
	rm -rf $(TARGET)
