// Glyph Cache
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#include "ViewdoGlyphCache.h"
#include "ViewdoTexturePacker.h"

static const int kTextureSize = 512;

struct ViewdoGlyphPage {
    Bitmap m_bitmap;
    TexturePacker m_packer;

    ViewdoGlyphPage(int w, int h)
        : m_bitmap(w, h, 1)
        , m_packer(w, h, 1)
    {}
    ViewdoGlyphPage(const ViewdoGlyphPage&) = delete;
    ViewdoGlyphPage& operator=(const ViewdoGlyphPage&) = delete;
};

static ViewdoGlyphCache* s_glyphCache = nullptr;
ViewdoGlyphCache& ViewdoGlyphCache::instance() {
    if (!s_glyphCache) s_glyphCache = new ViewdoGlyphCache();
    return *s_glyphCache;
}

// XXX: Now just implement the ViewdoGlyphCache type.
//      Then:
//       1. Call it from ViewdoPangoRenderer
//       2. Generate geometry in ViewdoPangoRenderer
//       3. Write a glut app that draws pango text in GL, with colors, etc.
//       4. Package it for Android.
//       5. Support custom fonts.

ViewdoGlyphCache::ViewdoGlyphCache() {}
ViewdoGlyphCache::~ViewdoGlyphCache() {
    for (auto i = m_glyphPages.begin(); i != m_glyphPages.end(); ++i)
        delete *i;
}

const ViewdoGlyph& ViewdoGlyphCache::findOrCreate(unsigned int codePoint, unsigned int pangoFontHash, FT_Face face) {
    ViewdoGlyphKey key(codePoint, pangoFontHash);

    const auto& i = m_glyphMap.find(key);
    if (i != m_glyphMap.end()) return i->second;

    // We should create it then.
    FT_Load_Glyph(face, codePoint, FT_LOAD_NO_BITMAP | FT_LOAD_TARGET_LIGHT);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    FT_Bitmap& rendered = face->glyph->bitmap;

    const int glyphw = rendered.width;
    const int glyphh = rendered.rows;

    int tx = 0, ty = 0, tw = 0, th = 0;
    int pageW = kTextureSize; int pageH = kTextureSize;
    int glyphCachePageIndex = -1;
    ViewdoGlyphPage* page = nullptr;
    // If it's enormous then just give it its own texture. This is probably horrible.
    if (glyphw > kTextureSize / 2 || glyphh > kTextureSize / 2) {
        page = new ViewdoGlyphPage(glyphw, glyphh);
        page->m_packer.insert(glyphw, glyphh, tx, ty, tw, th);
        m_glyphPages.push_back(page);
        glyphCachePageIndex = m_glyphPages.size() - 1;
        pageW = glyphw;
        pageH = glyphh;
    } else {
        // Try to insert the glyph into an existing texture.
        for (size_t i = 0; i < m_glyphPages.size(); ++i) {
            ViewdoGlyphPage* candidatePage = m_glyphPages[i];

            if (candidatePage->m_packer.insert(glyphw, glyphh, tx, ty, tw, th)) {
                page = candidatePage;
                glyphCachePageIndex = i;
                break;
            }
        }
    }

    if (glyphCachePageIndex == -1) {
        page = new ViewdoGlyphPage(kTextureSize, kTextureSize);
        page->m_packer.insert(glyphw, glyphh, tx, ty, tw, th);
        m_glyphPages.push_back(page);
        glyphCachePageIndex = m_glyphPages.size() - 1;
    }

    // Ok, now we've got the page sorted. We can copy the texture to the bounds we just got.
    unsigned char* buffer = page->m_bitmap.buffer() + ty * page->m_bitmap.pitch() + tx;
    unsigned char* outEnd = page->m_bitmap.buffer() + page->m_bitmap.pitch() * page->m_bitmap.height();

    for (int by = 0; by < rendered.rows; by++) {
        for (int bx = 0; bx < rendered.width; bx++) {
            unsigned char* in = rendered.buffer + by * rendered.pitch + bx;
            unsigned char* out = buffer + by * page->m_bitmap.pitch() + bx;

            if (out > outEnd) break;

            *out = *in;
        }
    }
    page->m_bitmap.setDirty();

    ViewdoGlyph glyph;
    glyph.m_page = glyphCachePageIndex;
    glyph.m_x0 = tx / (float) pageW;
    glyph.m_y0 = ty / (float) pageH;
    glyph.m_x1 = (tx + tw) / (float) pageW;
    glyph.m_y1 = (ty + th) / (float) pageH;
    glyph.m_width = glyphw;
    glyph.m_height = glyphh;
    glyph.m_offsetLeft = face->glyph->bitmap_left;
    glyph.m_offsetTop = face->glyph->bitmap_top;

    m_glyphMap[key] = glyph;

    return m_glyphMap[key];
}

const Bitmap* ViewdoGlyphCache::page(ViewdoGlyphCachePage page) {
    if (page < m_glyphPages.size())
        return &(m_glyphPages[page]->m_bitmap);
    return nullptr;
}
