CXXFLAGS = -fPIC -Wall -Wextra -O2 -g  # C flags
LDFLAGS = -shared -Lipe  # linking flags
RM = rm -f   # rm command
TARGET_LIB = surfer2.so  # target lib

SRCS = surfer2.cpp  # source files
OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CXX) ${LDFLAGS} -o $@ $^

$(SRCS:.cpp=.d):%.d:%.cpp
	$(CXX) $(CXXFLAGS) -MM $< >$@

include $(SRCS:.cpp=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.cpp=.d)
