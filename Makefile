CXX=g++
CXXOPTIMIZE= -O2
BOOST_ROOT = boost_1_66_0
CXXFLAGS= -Wall -pthread -std=c++11 $(CXXOPTIMIZE) -I$(BOOST_ROOT)
LDFLAGS := -lboost_system

USERID=EDIT_MAKE_FILE
CLASSES=

all: my-router

my-router: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp $(LDFLAGS)

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM my-router *.tar.gz

tarball: clean
	tar -cvf $(USERID).tar.gz *
