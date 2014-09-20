// Glyph cache for Viewdo
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#ifndef VIEWDO_GLYPH_CACHE_H
#define VIEWDO_GLYPH_CACHE_H

#include <stdint.h>
#include <map>
#include <vector>
#include <pango/pango-font.h>
#include <pango/pangoft2.h>
#include <ViewdoTexturePacker.h>
#include <graphics/Bitmap.h>

typedef uint32_t ViewdoGlyphCachePage;

struct ViewdoGlyph {
    ViewdoGlyphCachePage m_page;
    float m_x0, m_y0, m_x1, m_y1;
    int m_width;
    int m_height;
    int m_offsetLeft;
    int m_offsetTop;
};

struct ViewdoGlyphKey {
    unsigned int m_codePoint;
    unsigned int m_pangoHash;

    ViewdoGlyphKey(unsigned int codePoint, unsigned int pangoHash)
        : m_codePoint(codePoint)
        , m_pangoHash(pangoHash)
    {}
    bool operator<(const ViewdoGlyphKey& rhs) const {
        if (m_pangoHash == rhs.m_pangoHash)
            return m_codePoint < rhs.m_codePoint;
        return m_pangoHash < rhs.m_pangoHash;
    }
    bool operator==(const ViewdoGlyphKey& rhs) const {
        return m_pangoHash == rhs.m_pangoHash && m_codePoint == rhs.m_codePoint;
    }
};

struct ViewdoGlyphPage;

typedef std::map<ViewdoGlyphKey, ViewdoGlyph> ViewdoGlyphMap;

struct ViewdoGlyphCache {
    static ViewdoGlyphCache& instance();

    const ViewdoGlyph& findOrCreate(unsigned int codePoint, unsigned int pangoFontHash, FT_Face face);
    const Bitmap* page(ViewdoGlyphCachePage page);
private:
    ViewdoGlyphCache();
    ~ViewdoGlyphCache();

    ViewdoGlyphMap m_glyphMap;
    std::vector<ViewdoGlyphPage*> m_glyphPages;
};

#endif
