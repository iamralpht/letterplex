// JNI to Pango Font Description
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#include "JNIRegister.h"
#include <pango/pango-font.h>

static const char* JAVA_CLASS = "com/infinite_imagination/text/PangoFontDescription";

static jlong pf_create() { return reinterpret_cast<jlong>(pango_font_description_new()); }
static void pf_destroy(JNIEnv*, jclass, jlong obj) { if (obj) pango_font_description_free(reinterpret_cast<PangoFontDescription*>(obj)); }
static void pf_setFamily(JNIEnv* env, jclass, jlong obj, jstring family) {
    if (!obj) return;
    PangoFontDescription* desc = reinterpret_cast<PangoFontDescription*>(obj);
    if (!family) {
        pango_font_description_set_family(desc, "");
        return;
    }
    const char* cfamily = env->GetStringUTFChars(family, nullptr);
    pango_font_description_set_family(desc, cfamily);
    env->ReleaseStringUTFChars(family, cfamily);
}
static void pf_setSize(JNIEnv*, jclass, jlong obj, jint size) {
    if (!obj) return;
    PangoFontDescription* desc = reinterpret_cast<PangoFontDescription*>(obj);
    pango_font_description_set_size(desc, size);
}
static void pf_setStyle(JNIEnv*, jclass, jlong obj, jint style) {
    if (!obj) return;
    PangoFontDescription* desc = reinterpret_cast<PangoFontDescription*>(obj);
    pango_font_description_set_style(desc, (PangoStyle) style);
}
static void pf_setVariant(JNIEnv*, jclass, jlong obj, jint variant) {
    if (!obj) return;
    PangoFontDescription* desc = reinterpret_cast<PangoFontDescription*>(obj);
    pango_font_description_set_variant(desc, (PangoVariant) variant);
}
static void pf_setWeight(JNIEnv*, jclass, jlong obj, jint weight) {
    if (!obj) return;
    PangoFontDescription* desc = reinterpret_cast<PangoFontDescription*>(obj);
    pango_font_description_set_weight(desc, (PangoWeight) weight);
}
static void pf_setStretch(JNIEnv*, jclass, jlong obj, jint stretch) {
    if (!obj) return;
    PangoFontDescription* desc = reinterpret_cast<PangoFontDescription*>(obj);
    pango_font_description_set_stretch(desc, (PangoStretch) stretch);
}

static LPNativeMethod methods[] = {
    "create",       "()J",      (void*) pf_create,
    "destroy",      "(J)V",     (void*) pf_destroy,
    "setFamily",    "(JLjava/lang/String;)V", (void*) pf_setFamily,
    "setSize",      "(JI)V",    (void*) pf_setSize,
    "setStyle",     "(JI)V",    (void*) pf_setStyle,
    "setVariant",   "(JI)V",    (void*) pf_setVariant,
    "setWeight",    "(JI)V",    (void*) pf_setWeight,
    "setStretch",   "(JI)V",    (void*) pf_setStretch
};

jint JNIRegisterPangoFontDescription(JNIEnv* env) {
    return LPRegisterNatives(env, env->FindClass(JAVA_CLASS), methods);
}
