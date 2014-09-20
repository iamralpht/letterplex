// JNI to FontConfig
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#include "JNIRegister.h"
#include <fontconfig/fontconfig.h>

static const char* JAVA_CLASS = "com/infinite_imagination/text/FontConfig";

static jint fc_addFontFile(JNIEnv* env, jclass, jstring file) {
    if (!file) return -1;
    const char* cfile = env->GetStringUTFChars(file, nullptr);
    int rv = FcConfigAppFontAddFile(FcConfigGetCurrent(), (const FcChar8*) cfile);
    env->ReleaseStringUTFChars(file, cfile);
    return rv;
}

static LPNativeMethod methods[] = {
    "addFontFile",  "(Ljava/lang/String;)I", (void*) fc_addFontFile
};

jint JNIRegisterFontConfig(JNIEnv* env) {
    return LPRegisterNatives(env, env->FindClass(JAVA_CLASS), methods);
}
