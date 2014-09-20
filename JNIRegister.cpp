// Main JNI entrypoint
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#include <JNIRegister.h>

extern jint JNIRegisterDawgDic(JNIEnv*);
extern jint JNIRegisterPangoLayout(JNIEnv*);
extern jint JNIRegisterPangoFontDescription(JNIEnv*);
extern jint JNIRegisterFontConfig(JNIEnv*);

extern "C" {
JavaVM* gJavaVM = nullptr;


EXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
    gJavaVM = vm;

    JNIEnv* env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    JNIRegisterDawgDic(env);
    JNIRegisterPangoLayout(env);
    JNIRegisterPangoFontDescription(env);
    JNIRegisterFontConfig(env);

    return JNI_VERSION_1_6;
}

}


