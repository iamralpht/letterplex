# Android makefile

LP_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_PATH := $(LP_PATH)
DEPEND_PATH := $(LOCAL_PATH)/build/out
DEPEND_INCLUDES := \
	$(DEPEND_PATH)/include \
	$(DEPEND_PATH)/include/pango-1.0 \
	$(DEPEND_PATH)/include/glib-2.0 $(DEPEND_PATH)/lib/glib-2.0/include \
    $(DEPEND_PATH)/include/freetype2
DEPEND_LIBS := \
    $(DEPEND_PATH)/lib/libpango-1.0.a \
    $(DEPEND_PATH)/lib/libpangoft2-1.0.a \
    $(DEPEND_PATH)/lib/libgobject-2.0.a \
    $(DEPEND_PATH)/lib/libgmodule-2.0.a \
    $(DEPEND_PATH)/lib/libglib-2.0.a \
    $(DEPEND_PATH)/lib/libharfbuzz.a \
    $(DEPEND_PATH)/lib/libfontconfig.a \
    $(DEPEND_PATH)/lib/libfreetype.a \
    $(DEPEND_PATH)/lib/libffi.a \
    $(DEPEND_PATH)/lib/libexpat.a \
    $(DEPEND_PATH)/lib/libintl.a \
    $(DEPEND_PATH)/lib/libiconv.a

LOCAL_MODULE    := letterpressed
LOCAL_C_INCLUDES:= third_party/dawgdic/src/ text/pango $(DEPEND_INCLUDES)
LOCAL_CFLAGS    := -DLP_ANDROID -fvisibility=hidden $(FREETYPE_DEFINES) $(HARFBUZZ_DEFINES)
LOCAL_SRC_FILES :=  \
    JNIRegister.cpp \
    JNIDawgDic.cpp  \
    JNIFontConfig.cpp \
    JNIPangoLayout.cpp \
    JNIPangoFontDescription.cpp \
    text/pango/ViewdoGlyphCache.cpp \
    text/pango/ViewdoPangoRenderer.cpp

#LOCAL_STATIC_LIBRARIES := $(DEPEND_LIBS)
LOCAL_LDLIBS    := $(DEPEND_LIBS) -llog -landroid -lGLESv2 -lz

include $(BUILD_SHARED_LIBRARY)
