CXX=$(CROSS_COMPILE)g++
RM=rm

CXXSOURCES=$(wildcard *.cpp) ../lib/loguru/loguru.cpp #TODO Dont know if loguru has to be in there too

# Standard Redpitaya includes
INCLUDE =  -I$(INSTALL_DIR)/include
INCLUDE += -I$(INSTALL_DIR)/include/api2
INCLUDE += -I$(INSTALL_DIR)/include/apiApp
INCLUDE += -I$(INSTALL_DIR)/rp_sdk
INCLUDE += -I$(INSTALL_DIR)/rp_sdk/libjson

# Standard Redpitaya libraries
LIBS = -L$(INSTALL_DIR)/lib
LIBS += -L$(INSTALL_DIR)/rp_sdk

# LibSigrokDecode + python includes
INCLUDE += -I/usr/include/libsigrokdecode
INCLUDE += -I/usr/include/glib-2.0
INCLUDE += -I/usr/lib64/glib-2.0/include #Fedora path for testing
INCLUDE += -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include
INCLUDE += -I/usr/include/python3.5

# LibSigrokDecode + python3 libraries
LIBS += -L/usr/lib/
LIBS += -L/usr/lib/arm-linux-gnueabihf/

# JSON includes
INCLUDE += -I../lib/nlohmann
INCLUDE += -I../lib/nlohman/single_include
INCLUDE += -I../lib/nlohman/builddir

# Loguru includes, etc.
INCLUDE += -I../lib/loguru

COMMON_FLAGS+=-Wall -fPIC -Os -s
CXXFLAGS+=$(COMMON_FLAGS) -std=c++11 $(INCLUDE)
LDFLAGS = -shared $(COMMON_FLAGS) $(LIBS)
LDFLAGS+= -Wl,--whole-archive,--no-as-needed
LDFLAGS+= -lcryptopp -lrpapp -lrp -lrp_sdk -lsigrokdecode
LDFLAGS+= -Wl,--no-whole-archive
LDFLAGS+= pkg-config nlohmann_json --cflags

CXXOBJECTS=$(CXXSOURCES:.cpp=.o)
OBJECTS=$(CXXOBJECTS)

OUT_DIR=../
C_OUT_NAME=$(OUT_DIR)controllerhf.so

all: make_c_app

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

make_c_app: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(C_OUT_NAME) $(LDFLAGS)

clean:
	-$(RM) -f $(C_OUT_NAME) $(OBJECTS)
