
SOURCES = ViewdoPangoRenderer.cpp ViewdoGlyphCache.cpp
CFLAGS = -I. -I../../ -g -fPIC `pkg-config --cflags pango pangoft2 freetype2`
LDFLAGS = -lpthread `pkg-config --libs pango pangoft2 freetype2`

OBJECTS = $(addsuffix .o, $(basename $(filter %.c %.cpp %.cc,$(SOURCES))))

.c.o:
	@echo Compile C $<
	@$(CC) -Wall -Wextra $(CFLAGS) -c $< -o $@

.cpp.o:
	@echo Compile C++ $<
	@${CXX} -Wall -Wextra -std=c++0x $(CFLAGS) -c $< -o $@

.cc.o:
	@echo Compile C++ $<
	@${CXX} -std=c++11 $(CFLAGS) -c $< -o $@

all: ViewdoPangoRenderer

ViewdoPangoRenderer: $(OBJECTS)
	@echo Link $@
	@${CXX} ${OBJECTS} ${LDFLAGS} -o $@

clean:
	@rm -f $(OBJECTS) ViewdoPangoRenderer
