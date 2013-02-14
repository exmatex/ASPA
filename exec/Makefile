#
# ===== Build options begin here =====
#

#
# Specify the C++ compiler.  This is also used for linking the executable.
#

CXX = g++

#
# Specify compiler options (optimization level, debugging options, etc.)
#

CXXFLAGS = -g

#
# Specify the location of the BLAS and LAPACK libraries.  Depending upon the
# directories searched by the linker by default, it may not be necessary to
# specify these, in which case they can be left undefined (empty).  You can first
# try building with these macros undefined (empty) to find out.  If the libraries
# aren't found, or if a specific implementation of BLAS/LAPACK located elsewhere
# is desired, set these macros to the location of libblas.a and liblapack.a,
# respectively.
#

LAPACK_LOC =
BLAS_LOC =

#
# The M-tree database can be read/written from/to a file in HDF5 format, but
# this is optional.  If such functionality is desired, then specify the location
# of the HDF5 library (i.e., the top level directory containing the bin, include
# and lib subdirectories).  Otherwise just leave this macro undefined (empty).
#

HDF5_LOC = /usr/local/tools/hdf5-gnu-serial-1.8.5
#HDF5_LOC =
HDF5_DEFINES = -DH5_USE_16_API

#
# ===== Build options end here =====
#

CXXFLAGS += -I../src/mtl_headers -I../src/interpolation_database -I../src/interpolation -I../src/database -I../src/utils  -Dincluded_MPTCOUPLER_config -Dincluded_config -DDBL_SNAN_IS_BROKEN -DFLT_SNAN_IS_BROKEN

LIBS =
ifneq ($(strip $(LAPACK_LOC)),)
LIBS += -L$(LAPACK_LOC)
endif
LIBS += -llapack

ifneq ($(strip $(BLAS_LOC)),)
LIBS += -L$(BLAS_LOC)
endif
LIBS += -lblas

ifeq ($(strip $(HDF5_LOC)),)
CXXFLAGS += -UHAVE_PKG_hdf5
else
CXXFLAGS += -DHAVE_PKG_hdf5 -I$(HDF5_LOC)/include $(HDF5_DEFINES)
LIBS += -L$(HDF5_LOC)/lib -lhdf5
endif

INTERPDB_SRC  = $(shell ls ../src/interpolation_database/*/*.cc)
INTERPDB_OBJS = $(INTERPDB_SRC:.cc=.o)
INTERPDB_DEPS = $(INTERPDB_SRC:.cc=.d)

INTERP_SRC  = $(shell ls ../src/interpolation/*/*.cc)
INTERP_OBJS = $(INTERP_SRC:.cc=.o)
INTERP_DEPS = $(INTERP_SRC:.cc=.d)

DB_SRC  = $(shell ls ../src/database/mtreedb/*.cc)
DB_OBJS = $(DB_SRC:.cc=.o)
DB_DEPS = $(DB_SRC:.cc=.d)

UTILS_SRC  = $(shell ls ../src/utils/toolbox/*/*.cc)
UTILS_OBJS = $(UTILS_SRC:.cc=.o)
UTILS_DEPS = $(UTILS_SRC:.cc=.d)

MAKEDEPEND = gcc -MM -MT $*.o $(CXXFLAGS) -o $*.d.tmp $<

default: aspa

aspa:  main.o $(INTERPDB_OBJS) $(INTERP_OBJS) $(DB_OBJS) $(UTILS_OBJS)
	$(CXX) $(CXXFLAGS) main.o $(INTERPDB_OBJS) $(INTERP_OBJS) $(DB_OBJS) $(UTILS_OBJS) $(LIBS) -o aspa

%.o : %.cc
	@$(MAKEDEPEND)
	cp $*.d.tmp $*.d
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.d.tmp >> $*.d
	rm -f $*.d.tmp
	$(CXX) -c $(CXXFLAGS) -c -o $@ $< 

-include $(INTERPDB_DEPS)
-include $(INTERP_DEPS)
-include $(DB_DEPS)
-include $(UTILS_DEPS)

clean:
	$(RM) aspa main.o main.d $(INTERPDB_OBJS) $(INTERPDB_DEPS) $(INTERP_OBJS) $(INTERP_DEPS) \
              $(DB_OBJS) $(DB_DEPS) $(UTILS_OBJS) $(UTILS_DEPS)
