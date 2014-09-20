// Bitmap definition
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#ifndef VIEWDO_BITMAP_H
#define VIEWDO_BITMAP_H

#include <string.h>

struct Bitmap {
    Bitmap(unsigned int width, unsigned int height, unsigned int bpp)
        : m_width(width)
        , m_height(height)
        , m_pitch(width * bpp)
        , m_bpp(bpp)
        , m_dirty(true)
    {
        m_buffer = new unsigned char[width * height * bpp];
        memset(m_buffer, 0, width * height * bpp);
    }
    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;

    ~Bitmap() { delete[] m_buffer; }

    unsigned char* buffer() const { return m_buffer; }
    unsigned int width() const { return m_width; }
    unsigned int height() const { return m_height; }
    unsigned int pitch() const { return m_pitch; }
    unsigned int bpp() const { return m_bpp; }
    bool dirty() const { return m_dirty; }
    void clearDirty() const { m_dirty = false; }
    void setDirty() { m_dirty = true; }
private:
    unsigned char* m_buffer;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_pitch;
    unsigned int m_bpp;
    mutable bool m_dirty;
};

#endif
