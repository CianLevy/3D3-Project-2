CXX=g++
CXXOPTIMIZE= -O2
BOOST_ROOT = ../boost_1_66_0
CXXFLAGS= -Wall -pthread -std=c++11 $(CXXOPTIMIZE) -I$(BOOST_ROOT)
LDFLAGS := -lboost_system

USERID=EDIT_MAKE_FILE
CLASSES=

all: server client

server: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp $(LDFLAGS)

client:  $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp $(LDFLAGS)

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM server client *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
