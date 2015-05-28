
SDIR=src
ODIR=obj
LDIR=lib
BINDIR=bin
SHAREDIR=share
PYDIR=python

SOLIBNAME  := Utils
SOLIB = $(LDIR)/lib$(SOLIBNAME).so

IDIR = -Iinclude
LIBS=-L./$(LDIR)
CXX = g++
LD  = g++
CFLAGSCINT  = -O2 -Wall -Wextra -Wno-unused-parameter $(IDIR) -fPIC
CFLAGS  = $(CFLAGSCINT) -g -c -std=c++11
LDFLAGS = -shared -fPIC -Wl,-soname,lib$(SOLIBNAME).so $(shell root-config --ldflags)

ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)

lib_src     = $(wildcard $(SDIR)/*.cxx)
lib_obj_tmp = $(patsubst %.cxx, %.o, $(lib_src))
lib_obj     = $(patsubst $(SDIR)/%, $(ODIR)/%, $(lib_obj_tmp))

share_files = $(wildcard $(SHAREDIR)/*)
share_files_bin = $(patsubst $(SHAREDIR)/%, $(BINDIR)/%, $(share_files))

python_files = $(wildcard $(PYDIR)/*)
python_files_bin = $(patsubst $(PYDIR)/%, $(BINDIR)/%, $(python_files))

all : dirs bin so
so : $(LDIR) $(DEPFILES) $(SOLIB)
bin: $(share_files_bin) $(python_files_bin)

dirs:
	  mkdir -p $(ODIR)
	  mkdir -p $(LDIR)
	  mkdir -p $(BINDIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.d $(ODIR)/*Dict* core* $(LDIR)/*.so *_C.so $(BINDIR)/*

$(ODIR)/%.o: $(SDIR)/%.cxx
	$(CXX) -o $@ $< $(ROOTCFLAGS) $(CFLAGS)

$(SOLIB): $(lib_obj)
	$(LD) $(LDFLAGS) $^ $(ROOTLIBS) -o $@

$(BINDIR)/%.py:
	ln -sf $(patsubst $(BINDIR)/%, ../$(PYDIR)/%, $@) $@

$(BINDIR)/%:
	ln -sf $(patsubst $(BINDIR)/%, ../$(SHAREDIR)/%, $@) $@

