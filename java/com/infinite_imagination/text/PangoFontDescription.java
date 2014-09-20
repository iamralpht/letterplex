package com.infinite_imagination.text;

public class PangoFontDescription {
	public static final int STYLE_NORMAL = 0;
	public static final int STYLE_OBLIQUE = 1;
	public static final int STYLE_ITALIC = 2;
	
	public static final int WEIGHT_THIN = 100;
	public static final int WEIGHT_ULTRALIGHT = 200;
	public static final int WEIGHT_LIGHT = 300;
	public static final int WEIGHT_BOOK = 380;
	public static final int WEIGHT_NORMAL = 400;
	public static final int WEIGHT_MEDIUM = 500;
	public static final int WEIGHT_SEMIBOLD = 600;
	public static final int WEIGHT_BOLD = 700;
	public static final int WEIGHT_ULTRABOLD = 800;
	public static final int WEIGHT_HEAVY = 900;
	public static final int WEIGHT_ULTRAHEAVY = 1000;
	
	public static final int VARIANT_NORMAL = 0;
	public static final int VARIANT_SMALL_CAPS = 1;
	
	public static final int STRETCH_ULTRA_CONDENSED = 1;
	public static final int STRETCH_EXTRA_CONDENSED = 2;
	public static final int STRETCH_CONDENSED = 3;
	public static final int STRETCH_SEMI_CONDENSED = 4;
	public static final int STRETCH_NORMAL = 5;
	public static final int STRETCH_SEMI_EXPANDED = 6;
	public static final int STRETCH_EXPANDED = 7;
	public static final int STRETCH_EXTRA_EXPANDED = 8;
	public static final int STRETCH_ULTRA_EXPANDED = 9;
	
	private long mSelf;
	
	public static final PangoFontDescription HUGE_FONT;
	public static final PangoFontDescription TITLE_FONT;
	public static final PangoFontDescription SUBTITLE_FONT;
	public static final PangoFontDescription BUTTON_FONT;
	public static final PangoFontDescription MICRO_FONT;
	
	private static final String SANS = "Source Sans Pro";
	static {
		HUGE_FONT = new PangoFontDescription().setFamily(SANS).setSize(32);
		TITLE_FONT = new PangoFontDescription().setFamily(SANS).setSize(22);
		SUBTITLE_FONT = new PangoFontDescription().setFamily(SANS).setSize(18);
		BUTTON_FONT = SUBTITLE_FONT;
		MICRO_FONT = new PangoFontDescription().setFamily(SANS).setSize(12);
	}
	
	public PangoFontDescription() {
		Platform.getInstance().canUseNativeCode();
		mSelf = create();
	}
	
	public void dispose() { destroy(mSelf); mSelf = 0; }
	public void finalize() { dispose(); }
	public PangoFontDescription setFamily(String family) { setFamily(mSelf, family); return this; }
	public PangoFontDescription setSize(float size) { setSize(mSelf, Math.round(size * 1000)); return this; }
	public PangoFontDescription setStyle(int style) { setStyle(mSelf, style); return this; }
	public PangoFontDescription setVariant(int variant) { setVariant(mSelf, variant); return this; }
	public PangoFontDescription setWeight(int weight) { setWeight(mSelf, weight); return this; }
	public PangoFontDescription setStretch(int stretch) { setStretch(mSelf, stretch); return this; }
	
	/* package */ long self() { return mSelf; }
	
	private static native long create();
	private static native void destroy(long self);
	
	private static native void setFamily(long self, String family);
	private static native void setSize(long self, int size);
	private static native void setStyle(long self, int style);
	private static native void setVariant(long self, int variant);
	private static native void setWeight(long self, int weight);
	private static native void setStretch(long self, int stretch);
}
