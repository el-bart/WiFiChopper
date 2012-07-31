.PHONY: all
all: helloworld helloworldpp

helloworld: helloworld.c
	$(CC) $(CFLAGS) $(LD_LIBS) -Wall -O3 -o $@ $^

helloworldpp: helloworldpp.cpp
	$(CXX) $(CXXFLAGS) -Wall -O3 -o $@ $^ $(LD_LIBS)

.PHONY: clean
clean:
	rm -fv *.o helloworld helloworldpp
