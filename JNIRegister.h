// JNI Utilities for safe registration of native function in modern C++.
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#ifndef JNI_REGISTER_H
#define JNI_REGISTER_H

#include <jni.h>
#include <stddef.h>

// This structure mirrors JNINativeMethod, but uses const correctly. The JVM
// treats the values as const anyway, so we perform one unsafe cast on LPNativeMethod
// instead of having lots of warnings for implicitly casting const away.
typedef struct {
    const char* name;
    const char* signature;
    const void* fnPtr;
} LPNativeMethod;

template<size_t SIZE>
static int LPRegisterNatives(JNIEnv* env, jclass cls, LPNativeMethod (&methods)[SIZE]) {
    return env->RegisterNatives(cls, reinterpret_cast<JNINativeMethod*>(methods), SIZE);
}

#define UNUSED_PARAM(x) (void)(x)
#define EXPORT __attribute__ ((visibility ("default")))

#endif
