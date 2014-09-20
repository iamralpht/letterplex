// Pango text renderer for Viewdo
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#define PANGO_ENABLE_BACKEND 1
#include <ViewdoPangoRenderer.h>
#include <ViewdoGlyphCache.h>
#include <pango/pango-renderer.h>
#include <pango/pangoft2.h>
#include <unistd.h>
#include <math.h>

//! An internal-use-only struct for interfacing with Pango. Ignore this.
struct Renderer {
	PangoRenderer parent_instance;
    std::vector<ViewdoTextRun>* m_runs;
    float m_scale;
};

//! Another internal-use-only struct for interfacing with Pango. Ignore this.
struct RendererClass {
	PangoRendererClass class_instance;
};

G_DEFINE_TYPE(Renderer, renderer, PANGO_TYPE_RENDERER);

#define TYPE_RENDERER (renderer_get_type())
#define RENDERER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), TYPE_RENDERER, Renderer))

static void addPoint(std::vector<float>& buffer, float x, float y, float tx, float ty, float scale) {
    buffer.push_back(x * scale);
    buffer.push_back(y * scale);
    buffer.push_back(tx);
    buffer.push_back(ty);
}

void doDrawGlyphs(PangoRenderer* renderer, PangoFont* font, PangoGlyphString* glyphs, int px, int py) {
    Renderer* ren = RENDERER(renderer);
    if (!ren->m_runs) return;
    std::vector<ViewdoTextRun>& runs = *(ren->m_runs);
    const float s = ren->m_scale;

    PangoColor* fg = pango_renderer_get_color(renderer, PANGO_RENDER_PART_FOREGROUND);
    //PangoColor* bg = pango_renderer_get_color(renderer, PANGO_RENDER_PART_BACKGROUND);

    float red = !fg ? 0.0f : fg->red / 65536.0f;
    float green = !fg ? 0.0f : fg->green / 65536.0f;
    float blue = !fg ? 0.0f : fg->blue / 65536.0f;

    PangoFontDescription* desc = pango_font_describe(font);
    unsigned int fontHash = pango_font_description_hash(desc);
    pango_font_description_free(desc);

    FT_Face face = pango_fc_font_lock_face((PangoFcFont*) font);

    PangoGlyphUnit layoutX = px;
    PangoGlyphUnit layoutY = py;
    ViewdoGlyphCache& cache = ViewdoGlyphCache::instance();

    for (int i = 0; i < glyphs->num_glyphs; i++) {
        PangoGlyphInfo* gi = glyphs->glyphs + i;

        if (gi->glyph & PANGO_GLYPH_UNKNOWN_FLAG) {
            // XXX: Draw the fallback square.
            continue;
        }

        unsigned int glyph = gi->glyph;
        PangoRectangle r;
        
        pango_font_get_glyph_extents(font, glyph, &r, 0);
        pango_extents_to_pixels(&r, 0);

        float w = r.width;
        float h = r.height;

        if (w <= 0.f && h <= 0.f) {
            // Space.
            layoutX += gi->geometry.width;
            continue;
        }

        // Load and render the glyph. We probably need to just tell the font to cache it
        // and then grab the coordinates of it.
        const ViewdoGlyph& cachedGlyph = cache.findOrCreate(gi->glyph, fontHash, face);
        
        // Walk our existing runs and try to find one with the same color and page.
        // XXX: Fastpath try and use the run from the last glyph.
        ViewdoTextRun* run = nullptr;
        for (auto i = runs.begin(); i != runs.end(); i++) {
            if (i->m_page == cachedGlyph.m_page && i->m_red == red && i->m_green == green && i->m_blue == blue) {
                run = &(*i);
                break;
            }
        }
        if (!run) {
            runs.push_back(ViewdoTextRun(cachedGlyph.m_page, red, green, blue));
            run = &(runs[runs.size() - 1]);
        }

        float x = PANGO_PIXELS(layoutX + gi->geometry.x_offset) + cachedGlyph.m_offsetLeft;
        float y = PANGO_PIXELS(layoutY + gi->geometry.y_offset) - cachedGlyph.m_offsetTop;

        // Append ourselves onto this run.
        addPoint(run->m_geometry,
            x, y,
            cachedGlyph.m_x0, cachedGlyph.m_y0,
            s);

        addPoint(run->m_geometry,
            x + cachedGlyph.m_width, y,
            cachedGlyph.m_x1, cachedGlyph.m_y0,
            s);

        addPoint(run->m_geometry,
            x, y + cachedGlyph.m_height,
            cachedGlyph.m_x0, cachedGlyph.m_y1,
            s);

        // Triangle 2.
        addPoint(run->m_geometry,
            x + cachedGlyph.m_width, y,
            cachedGlyph.m_x1, cachedGlyph.m_y0,
            s);

        addPoint(run->m_geometry,
            x, y + cachedGlyph.m_height,
            cachedGlyph.m_x0, cachedGlyph.m_y1,
            s);

        addPoint(run->m_geometry,
            x + cachedGlyph.m_width, y + cachedGlyph.m_height,
            cachedGlyph.m_x1, cachedGlyph.m_y1,
            s);

        layoutX += gi->geometry.width;
    }

    pango_fc_font_unlock_face((PangoFcFont*) font);
}

static GObjectClass* _pangoClass = 0;

void renderer_finalize(GObject* object) {
        // Renderer* priv = RENDERER(object);

        G_OBJECT_CLASS(_pangoClass)->finalize(object);
}

void renderer_init(Renderer* priv) {
    priv->m_runs = nullptr;
}

void renderer_class_init(RendererClass* klass) {
        GObjectClass*       object_class   = G_OBJECT_CLASS(klass);
        PangoRendererClass* renderer_class = PANGO_RENDERER_CLASS(klass);

        _pangoClass = static_cast<GObjectClass*>(g_type_class_peek_parent(klass));

        object_class->finalize = renderer_finalize;

        renderer_class->draw_glyphs = &doDrawGlyphs;
}

void viewdo_pango_render(PangoLayout* layout, float x, float y, float scale, std::vector<ViewdoTextRun>& outRuns) {
    Renderer* viewdoRenderer = (Renderer*) g_object_new(TYPE_RENDERER, 0);
    viewdoRenderer->m_runs = &outRuns;
    viewdoRenderer->m_scale = scale;

    pango_renderer_draw_layout(
        PANGO_RENDERER(viewdoRenderer),
        layout,
        (int)(x * PANGO_SCALE / scale), (int)(y * PANGO_SCALE / scale));

    g_object_unref(viewdoRenderer);
}

