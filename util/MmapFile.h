// Memory-mapped file
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#ifndef VIEWDO_MMAP_FILE_H
#define VIEWDO_MMAP_FILE_H

#ifdef LP_ANDROID
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

struct MmapFile {
    MmapFile(JNIEnv* env, jobject assetManager, const char* path)
        : m_asset(0)
        , m_buffer(0)
        , m_length(0)
    {
        AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
        if (!mgr) return;
        m_asset = AAssetManager_open(mgr, path, AASSET_MODE_BUFFER);
        if (!m_asset) return;
        m_buffer = AAsset_getBuffer(m_asset);
        m_length = AAsset_getLength(m_asset);
    }

    ~MmapFile() { AAsset_close(m_asset); }

    const void* buffer() const { return m_buffer; }
    off_t length() const { return m_length; }
private:
    AAsset* m_asset;
    const void* m_buffer;
    off_t m_length;
};

#else
// Regular Linux; just use the filesystem.
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>


struct MmapFile {
    MmapFile(const char* path) 
        : m_fd(-1)
        , m_buffer(0)
        , m_length(0)
    {
        struct stat st;
        m_fd = open(path, O_RDONLY);
        if (m_fd == -1) return;

        fstat(m_fd, &st);
        m_length = st.st_size;
        m_buffer = mmap(nullptr, m_length, PROT_READ, MAP_SHARED, m_fd, 0);
    }

    ~MmapFile() {
        if (m_buffer) munmap(m_buffer, m_length);
        if (m_fd != -1) close(m_fd);
    }

    const void* buffer() const { return m_buffer; }
    off_t length() const { return m_length; }
private:
    int m_fd;
    void* m_buffer;
    off_t m_length;
};

#endif
#endif

