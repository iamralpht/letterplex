// JNI to Pango Text Layout interface
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#include "JNIRegister.h"
#include <pango/pango-layout.h>
#include <text/pango/ViewdoPangoRenderer.h>
#ifdef LP_ANDROID
#include <GLES2/gl2.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#endif

static const char* JAVA_CLASS = "com/infinite_imagination/text/PangoTextLayout";

struct Texture {
    GLuint m_textureID;
    const Bitmap* m_bitmap;

    Texture(const Bitmap* bitmap)
        : m_textureID(0)
        , m_bitmap(bitmap)
    {}
    ~Texture() { if (m_textureID) glDeleteTextures(1, &m_textureID); }

    void contextLost() { m_textureID = 0; }

    void bind() {
        if (!m_textureID) {
            glGenTextures(1, &m_textureID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_bitmap->width(), m_bitmap->height(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, m_bitmap->buffer());
        } else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textureID);
            if (m_bitmap->dirty())
                glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_bitmap->width(), m_bitmap->height(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, m_bitmap->buffer());
        }
        m_bitmap->clearDirty();
    }
};

static std::map<unsigned int, Texture*> sTextureCache; // XXX: Management!

static bool makeGlyphCachePageCurrent(ViewdoGlyphCachePage page) {
    Texture* tex = nullptr;
    auto i = sTextureCache.find(page);
    if (i == sTextureCache.end()) {
        const Bitmap* glyphs = ViewdoGlyphCache::instance().page(page);
        if (!glyphs) return false;
        tex = new Texture(glyphs);
        sTextureCache[page] = tex;
    } else {
        tex = i->second;
    }

    tex->bind();

    return true;
}

static void contextLost() {
    for (auto i = sTextureCache.begin(); i != sTextureCache.end(); i++) {
        i->second->contextLost();
    }
}

static PangoContext* sContext = nullptr;
static PangoFontMap* sFontMap = nullptr;

static PangoContext* context() {
    if (sContext) return sContext;
    sContext = pango_context_new();
    sFontMap = pango_ft2_font_map_new();
    pango_context_set_font_map(sContext, sFontMap);

    return sContext;
}

struct JNIPangoLayout {
    std::vector<ViewdoTextRun> m_runs;
    PangoLayout* m_layout;

    JNIPangoLayout() 
        : m_layout(pango_layout_new(context()))
    {}
    ~JNIPangoLayout() {
        g_object_unref(m_layout);
    }

    void setMarkup(const char* markup) { pango_layout_set_markup(m_layout, markup, -1); }
    void setWidth(int width) { pango_layout_set_width(m_layout, width); }
    void setJustify(bool justify) { pango_layout_set_justify(m_layout, justify); }
    void setAlignment(int alignment) { pango_layout_set_alignment(m_layout, (PangoAlignment) alignment); }
    void getPixelSize(int* width, int* height) { pango_layout_get_pixel_size(m_layout, width, height); }
    void setFontDescription(PangoFontDescription* d) { pango_layout_set_font_description(m_layout, d); }
    void generateGeometry(float x, float y, float scale) {
        m_runs.clear();
        viewdo_pango_render(m_layout, x, y, scale, m_runs);
    }
    void render(int colorUniform, float opacity) {
        // For each run:
        //  1. Find and make current the glyph cache texture.
        //  2. Set the color in the shader.
        //  3. Draw arrays!
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        for (auto i = m_runs.begin(); i != m_runs.end(); ++i) {
            ViewdoTextRun& run = *i;

            // Make the glyph cache texture current.
            if (!makeGlyphCachePageCurrent(run.m_page))
                continue;

            // Set the color in the shader.
            glUniform4f(colorUniform, run.m_red * opacity, run.m_green * opacity, run.m_blue * opacity, opacity);

            // Draw arrays!
            glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 4, &(run.m_geometry[0]));
            glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 4, &(run.m_geometry[2]));
            glDrawArrays(GL_TRIANGLES, 0, run.m_geometry.size() / 4);
        }

        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 4, nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 4, nullptr);
    }
};

static jlong pl_create() {
    return reinterpret_cast<jlong>(new JNIPangoLayout());
}
static void pl_destroy(JNIEnv*, jclass, jlong obj) {
    if (obj) delete reinterpret_cast<JNIPangoLayout*>(obj);
}
static void pl_setMarkup(JNIEnv* env, jclass, jlong obj, jstring markup) {
    if (!obj) return;
    JNIPangoLayout* layout = reinterpret_cast<JNIPangoLayout*>(obj);
    if (!markup) {
        layout->setMarkup("");
        return;
    }
    const char* str = env->GetStringUTFChars(markup, nullptr);
    layout->setMarkup(str);
    env->ReleaseStringUTFChars(markup, str);
}
static void pl_setWidth(JNIEnv*, jclass, jlong obj, int width) {
    if (obj) reinterpret_cast<JNIPangoLayout*>(obj)->setWidth(width);
}
static void pl_setJustify(JNIEnv*, jclass, jlong obj, bool justify) {
    if (obj) reinterpret_cast<JNIPangoLayout*>(obj)->setJustify(justify);
}
static void pl_setAlignment(JNIEnv*, jclass, jlong obj, int alignment) {
    if (obj) reinterpret_cast<JNIPangoLayout*>(obj)->setAlignment(alignment);
}
static void pl_setFontDescription(JNIEnv*, jclass, jlong obj, jlong font) {
    if (obj) reinterpret_cast<JNIPangoLayout*>(obj)->setFontDescription(reinterpret_cast<PangoFontDescription*>(font));
}
static void pl_getPixelSize(JNIEnv* env, jclass, jlong obj, jintArray out) {
    if (!obj || !out) return;
    JNIPangoLayout* layout = reinterpret_cast<JNIPangoLayout*>(obj);
    jint* outArray = env->GetIntArrayElements(out, nullptr);
    layout->getPixelSize(outArray, outArray+1);
    env->ReleaseIntArrayElements(out, outArray, 0);
}
static void pl_generateGeometry(JNIEnv*, jclass, jlong obj, float x, float y, float scale) {
    if (obj) reinterpret_cast<JNIPangoLayout*>(obj)->generateGeometry(x, y, scale);
}
static void pl_render(JNIEnv*, jclass, jlong obj, int colorUniform, float opacity) {
    if (obj) reinterpret_cast<JNIPangoLayout*>(obj)->render(colorUniform, opacity);
}


static LPNativeMethod methods[] = {
    "create",           "()J",                      (void*) pl_create,
    "destroy",          "(J)V",                     (void*) pl_destroy,
    "setMarkup",        "(JLjava/lang/String;)V",   (void*) pl_setMarkup,
    "setWidth",         "(JI)V",                    (void*) pl_setWidth,
    "setJustify",       "(JZ)V",                    (void*) pl_setJustify,
    "setAlignment",     "(JI)V",                    (void*) pl_setAlignment,
    "setFontDescription","(JJ)V",                   (void*) pl_setFontDescription,
    "getPixelSize",     "(J[I)V",                   (void*) pl_getPixelSize,
    "generateGeometry", "(JFFF)V",                  (void*) pl_generateGeometry,
    "render",           "(JIF)V",                   (void*) pl_render,
    "contextLost",      "()V",                      (void*) contextLost
};

jint JNIRegisterPangoLayout(JNIEnv* env) {
    return LPRegisterNatives(env, env->FindClass(JAVA_CLASS), methods);
}
