CXX = clang++
CXXFLAGS = -O2 -Wall -DVDBG
LDFLAGS = -lpthread
OBJS = file.o file_name.o request.o response.o ui.o log.o main.o

all: http_parser

%.o:%.c
	$(CXX) $(CXXFLAGS) $< -c -o $@

http_parser: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o http_parser

clean:
	rm -rf *.o http_parser
