# Makefile for UNIX systems

JAVA_HOME ?= /usr/lib/jvm/default-java

VIEWDO_SOURCES = \
            text/pango/ViewdoPangoRenderer.cpp \
            text/pango/ViewdoGlyphCache.cpp \
            text/pango/ViewdoGLRenderer.cpp

SOURCES =	JNIRegister.cpp \
			JNIDawgDic.cpp \
            JNIPangoLayout.cpp \
            JNIPangoFontDescription.cpp \
            JNIFontConfig.cpp 

all: examples/text libletterplex.so

VIEWDO_OBJECTS = $(addsuffix .o, $(basename $(filter %.c %.cpp %.cc,$(VIEWDO_SOURCES))))
OBJECTS = $(addsuffix .o, $(basename $(filter %.c %.cpp %.cc,$(SOURCES)))) ${VIEWDO_OBJECTS}

CFLAGS = \
    -g \
	-fvisibility=hidden -fno-exceptions -fPIC \
	-I. -Ithird_party/dawgdic/src -I$(JAVA_HOME)/include \
    -Itext/pango \
    `pkg-config --cflags pango freetype2 glib-2.0`

LDFLAGS = -lpthread `pkg-config --libs pango pangoft2 freetype2 glib-2.0 gobject-2.0 gthread-2.0 gmodule-2.0 glfw3` -lGL

.c.o:
	@echo Compile C $<
	@$(CC) -Wall -Wextra -Wno-unused-parameter $(CFLAGS) -c $< -o $@

.cpp.o:
	@echo Compile C++ $<
	@${CXX} -Wall -Wextra -std=c++0x $(CFLAGS) -c $< -o $@

.cc.o:
	@echo Compile C++ $<
	@${CXX} -std=c++11 $(CFLAGS) -c $< -o $@

examples/text: ${VIEWDO_OBJECTS} examples/text.o
	@echo Link $@
	@${CXX} ${VIEWDO_OBJECTS} examples/text.o ${LDFLAGS} -o $@

libletterplex.so: ${OBJECTS}
	@echo Link $@
	@${CXX} -Wl,--no-undefined ${OBJECTS} ${LDFLAGS} -shared -o $@

clean:
	@rm -f ${OBJECTS} libletterplex.so examples/text examples/text.o


