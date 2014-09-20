// DawgDic JNI interface
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#include "JNIRegister.h"
#include <util/MmapFile.h>
#include <dawgdic/dictionary.h>

static const char* JAVA_CLASS = "com/infinite_imagination/util/Dictionary";


struct Dictionary {
    Dictionary(MmapFile* file) {
        m_dictionary.Map(file->buffer());//, file->length());
    }
    ~Dictionary() { if (m_file) delete m_file; }

    bool find(const char* word) const {
        return m_dictionary.Contains(word);
    }
private:
    MmapFile* m_file;
    dawgdic::Dictionary m_dictionary;
};

static Dictionary* g_dict = 0;

static jlong dictionary_open(JNIEnv* env, jobject, jobject assetManager, jstring path) {
    if (!path) return 0;
    const char* cpath = env->GetStringUTFChars(path, 0);
#ifdef LP_ANDROID
    MmapFile* file = new MmapFile(env, assetManager, cpath);
#else
    UNUSED_PARAM(assetManager);
    MmapFile* file = new MmapFile(cpath);
#endif
    env->ReleaseStringUTFChars(path, cpath);
    if (!file->buffer()) {
        delete file;
        return 0;
    }

    g_dict = new Dictionary(file);
    return reinterpret_cast<jlong>(g_dict);
}

static void dictionary_close(JNIEnv*, jobject, Dictionary* dict) {
    UNUSED_PARAM(dict);
    delete g_dict;
    g_dict = 0;
}

static jboolean dictionary_find(JNIEnv* env, jobject, jlong dict, jstring word) {
    UNUSED_PARAM(dict);
    if (!word || !g_dict) return false;
    const char* cword = env->GetStringUTFChars(word, 0);
    if (!cword) return false;
    bool found = g_dict->find(cword);
    env->ReleaseStringUTFChars(word, cword);
    return found;
}

static LPNativeMethod methods[] = {
    { "open", "(Ljava/lang/Object;Ljava/lang/String;)J", (void*) dictionary_open },
    { "close", "(J)V", (void*) dictionary_close },
    { "find", "(JLjava/lang/String;)Z", (void*) dictionary_find }
};

jint JNIRegisterDawgDic(JNIEnv* env) {
    return LPRegisterNatives(env, env->FindClass(JAVA_CLASS), methods);
}
