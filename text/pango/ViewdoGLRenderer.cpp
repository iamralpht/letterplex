// Render a set of runs using the current shader
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#include <text/pango/ViewdoGLRenderer.h>

#ifdef LP_ANDROID
#include <GLES2/gl2.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#endif

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

void viewdo_pango_gl_context_lost() {
    for (auto i = sTextureCache.begin(); i != sTextureCache.end(); i++) {
        i->second->contextLost();
    }
}

void viewdo_pango_gl_render(const std::vector<ViewdoTextRun>& runs, int colorUniform, float opacity) {
    // For each run:
    //  1. Find and make current the glyph cache texture.
    //  2. Set the color in the shader.
    //  3. Draw arrays!
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    for (auto i = runs.begin(); i != runs.end(); ++i) {
        const ViewdoTextRun& run = *i;

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
