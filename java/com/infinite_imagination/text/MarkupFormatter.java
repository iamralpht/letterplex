package com.infinite_imagination.text;

import com.infinite_imagination.view.Color;

public class MarkupFormatter {
	private StringBuilder mSB = new StringBuilder();
	

	public MarkupFormatter text(String s) { mSB.append(s); return this; }
	public MarkupFormatter beginColor(Color c) {
		mSB.append("<span fgcolor=\"");
		mSB.append(c.toHex());
		mSB.append("\">");
		return this;
	}
	public MarkupFormatter endColor() { mSB.append("</span>"); return this; }
	public MarkupFormatter beginBold() { mSB.append("<b>"); return this; }
	public MarkupFormatter endBold() { mSB.append("</b>"); return this; }
	
	public MarkupFormatter color(Color c, String s) {
		mSB.append("<span fgcolor=\"");
		mSB.append(c.toHex());
		mSB.append("\">");
		mSB.append(s);
		mSB.append("</span>");
		return this;
	}
	public MarkupFormatter bold(String s) {
		mSB.append("<b>");
		mSB.append(s);
		mSB.append("</b>");
		return this;
	}
	public MarkupFormatter font(String font, String s) {
		mSB.append("<span font=\"");
		mSB.append(font);
		mSB.append("\">");
		mSB.append(s);
		mSB.append("</span>");
		return this;
	}
	
	public String toString() { return mSB.toString(); }
}
