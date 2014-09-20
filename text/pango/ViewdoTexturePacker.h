// Viewdo Texture Packer
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#ifndef VIEWDO_TEXTURE_PACKER_H
#define VIEWDO_TEXTURE_PACKER_H

struct TexturePackerNode {
    TexturePackerNode(int x, int y, int w, int h)
        : m_left(nullptr)
        , m_right(nullptr)
        , m_x(x)
        , m_y(y)
        , m_w(w)
        , m_h(h)
        , m_used(false)
    {}
    TexturePackerNode(const TexturePackerNode&) = delete;
    TexturePackerNode& operator=(const TexturePackerNode&) = delete;
    ~TexturePackerNode() {
        if (m_left) delete m_left;
        if (m_right) delete m_right;
    }

    TexturePackerNode* insert(int w, int h, int padding) {
        if (m_left) {
            TexturePackerNode* inserted = m_left->insert(w, h, padding);
            if (!inserted)
                inserted = m_right->insert(w, h, padding);
            return inserted;
        }

        w += padding * 2;
        h += padding * 2;

        // No space, children are smaller than we are. Bail out.
        if (m_used || w > m_w || h > m_h)
            return nullptr;

        // Perfect fit.
        if (w == m_w && h == m_h)  {
            m_used = true;
            return this;
        }

        // Subdivide
        if ((m_w - w) > (m_h - h)) {
            // Horizontal
            m_left = new TexturePackerNode(m_x, m_y, w, m_h);
            m_right = new TexturePackerNode(m_x + w, m_y, m_w - w, m_h);
        } else {
            // Vertical
            m_left = new TexturePackerNode(m_x, m_y, m_w, h);
            m_right = new TexturePackerNode(m_x, m_y + h, m_w, m_h - h);
        }

        return m_left->insert(w - padding * 2, h - padding * 2, padding);
    }

    int x() const { return m_x; }
    int y() const { return m_y; }
    int width() const { return m_w; }
    int height() const { return m_h; }
private:
    TexturePackerNode* m_left;
    TexturePackerNode* m_right;
    const int m_x;
    const int m_y;
    const int m_w;
    const int m_h;
    bool m_used;
};

struct TexturePacker {
    TexturePacker(int w, int h, int padding)
        : m_root(new TexturePackerNode(0, 0, w, h))
        , m_w(w)
        , m_h(h)
        , m_padding(padding)
    {}
    TexturePacker(const TexturePacker&) = delete;
    TexturePacker& operator=(const TexturePacker&) = delete;
    ~TexturePacker() { delete m_root; m_root = nullptr; }

    bool insert(int w, int h, int& outx, int& outy, int& outw, int& outh) {
        TexturePackerNode* node = m_root->insert(w, h, m_padding);
        if (!node) {
            outx = outy = outw = outh = 0;
            return false;
        }
        outx = node->x() + m_padding; outy = node->y() + m_padding;
        outw = node->width() - m_padding * 2; outh = node->height() - m_padding * 2;
        return true;
    }
private:
    TexturePackerNode* m_root;
    int m_w;
    int m_h;
    int m_padding;
};

#endif
