package com.infinite_imagination.text;

import com.infinite_imagination.view.SizeF;

public class PangoTextLayout {
	public static final int ALIGN_LEFT = 0;
	public static final int ALIGN_CENTER = 1;
	public static final int ALIGN_RIGHT = 2;
	
	private long mSelf;
	
	public PangoTextLayout() {
		mSelf = create();
	}
	public PangoTextLayout(String markup) { this(); setMarkup(markup); }
	public PangoTextLayout(PangoFontDescription font, String markup) { this(); setFontDescription(font); setMarkup(markup); }
	public void dispose() {
		if (mSelf != 0) destroy(mSelf);
		mSelf = 0;
	}
	public void finalize() { dispose(); }
	
	public PangoTextLayout setMarkup(String markup) { setMarkup(mSelf, markup); return this; }
	public PangoTextLayout setWidth(int width) { setWidth(mSelf, width); return this; }
	public PangoTextLayout setJustify(boolean justify) { setJustify(mSelf, justify); return this; }
	public PangoTextLayout setAlignment(int alignment) { setAlignment(mSelf, alignment); return this; }
	public PangoTextLayout setFontDescription(PangoFontDescription d) { setFontDescription(mSelf, d.self()); return this; }
	public SizeF getPixelSize() {
		int[] out = new int[2];
		getPixelSize(mSelf, out);
		return new SizeF(out[0], out[1]);
	}
	public PangoTextLayout setColor(float r, float g, float b, float a) { System.err.println("Unimplemented: layout color"); return this; }
	public void generateGeometry(float x, float y, float scale) { generateGeometry(mSelf, x, y, scale); }
	public void render(int colorUniform, float opacity) { render(mSelf, colorUniform, opacity); }

	private static native long create();
	private static native void destroy(long self);
	private static native void setMarkup(long self, String markup);
	private static native void setWidth(long self, int width);
	private static native void setJustify(long self, boolean justify);
	private static native void setAlignment(long self, int alignment);
	private static native void setFontDescription(long self, long font);
	private static native void getPixelSize(long self, int[] wh);
	private static native void generateGeometry(long self, float x, float y, float scale);
	private static native void render(long self, int colorUniform, float opacity);
	
	public static native void contextLost();
}
