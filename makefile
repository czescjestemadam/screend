CXXFLAGS = -O2

SRCS = $(shell find src/ -type f -name '*.cc')
OBJS = $(subst .cc,.o,$(SRCS))

build: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o screend

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $(subst .cc,.o,$<)

clean:
	rm $(OBJS)
