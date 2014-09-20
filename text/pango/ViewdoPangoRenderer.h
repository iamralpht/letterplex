// Pango text renderer for Viewdo
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#ifndef VIEWDO_PANGO_RENDERER_H
#define VIEWDO_PANGO_RENDERER_H

#include <vector>
#include <ViewdoGlyphCache.h>
#include <pango/pango-layout.h>

struct ViewdoTextRun {
    std::vector<float> m_geometry;
    ViewdoGlyphCachePage m_page;
    float m_red;
    float m_green;
    float m_blue;

    ViewdoTextRun(ViewdoGlyphCachePage page, float r, float g, float b)
        : m_page(page)
        , m_red(r)
        , m_green(g)
        , m_blue(b)
    {}
};

void viewdo_pango_render(PangoLayout* layout, float x, float y, float scale, std::vector<ViewdoTextRun>& outRuns);

#endif
