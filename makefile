CXX = g++
CXXFLAGS = -O2 -g -std=c++14 -Wall -Wconversion -Wno-deprecated-declarations

TARGET = a.exe
OBJ = obj

INCLUDES = -I E:/SDKs/boost_1_60_0 \
	-I . \

SRCS = main.cpp \
	   
OBJS_TEMP = $(SRCS:.cpp=.o)
OBJS      = $(patsubst %, $(OBJ)/%, $(OBJS_TEMP))

all: $(TARGET)
	
$(OBJ)/%.o: %.cpp
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)
	
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

clean:
	rm -R -f $(OBJ)
	rm $(TARGET)

