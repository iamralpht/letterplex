# Makefile for UNIX systems

JAVA_HOME ?= /usr/lib/jvm/default-java

SOURCES =	JNIRegister.cpp \
			JNIDawgDic.cpp \
            JNIPangoLayout.cpp \
            JNIPangoFontDescription.cpp \
            JNIFontConfig.cpp \
            text/pango/ViewdoPangoRenderer.cpp \
            text/pango/ViewdoGlyphCache.cpp \
            text/pango/ViewdoGLRenderer.cpp

all: libletterplex.so

OBJECTS = $(addsuffix .o, $(basename $(filter %.c %.cpp %.cc,$(SOURCES))))
GTEST_OBJECTS = $(addsuffix .o, $(basename $(filter %.c %.cpp %.cc,$(GTEST_SOURCE))))

CFLAGS = \
    -g \
	-fvisibility=hidden -fno-exceptions -fPIC \
	-I. -Ithird_party/dawgdic/src -I$(JAVA_HOME)/include \
    -Itext/pango \
    `pkg-config --cflags pango freetype2 glib-2.0`

LDFLAGS = -lpthread `pkg-config --libs pango pangoft2 freetype2 glib-2.0 gobject-2.0 gthread-2.0 gmodule-2.0` -lGL

.c.o:
	@echo Compile C $<
	@$(CC) -Wall -Wextra -Wno-unused-parameter $(CFLAGS) -c $< -o $@

.cpp.o:
	@echo Compile C++ $<
	@${CXX} -Wall -Wextra -std=c++0x $(CFLAGS) -c $< -o $@

.cc.o:
	@echo Compile C++ $<
	@${CXX} -std=c++11 $(CFLAGS) -c $< -o $@

libletterplex.so: ${OBJECTS}
	@echo Link $@
	@${CXX} -Wl,--no-undefined ${OBJECTS} ${LDFLAGS} -shared -o $@

clean:
	@rm -f ${OBJECTS} libletterplex.so text/test/TestTextLayout.o text/test/TestTextLayout


