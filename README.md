Letterplex
==========

Letterplex is a turn-based word game for Android devices, inspired by Loren Brichter's wonderful iOS game "Letterpress".

Letterplex is based on a custom UI toolkit that calls OpenGL directly to render. Its direct open source dependencies, included in this repository are:

*   Dawgdic &mdash; fast dictionary lookups from a memory mapped asset
*   Pango &mdash; complex text paragraph layout
*   Freetype &mdash; glyph rasterization
*   FontConfig &mdash; font management

Also included here are all of Pango's dependencies.

Letterplex also uses the following sounds from Freesound:
*   Freesound <a href="https://www.freesound.org/people/Feegle/sounds/44061/">clip 44061</a>, titled Dicewood, is licensed under the <a href="http://creativecommons.org/licenses/sampling+/1.0/">Sampling+ License</a>
*   Freesound <a href="https://www.freesound.org/people/mich3d/sounds/12658/">clip 12658</a>, titled Whoosh_Swish_03, is licensed under the <a href="http://creativecommons.org/licenses/by/3.0/">Attribution License</a>

Why Pango?
==========

Letterplex calls OpenGL directly, so to render text it draws one quad per glyph. In order to generate the quad geometry Letterplex needs to know all of the glyphs in a string and thier x-y offsets. Unfortunately the Android API does not distinguish between glyphs and characters -- so it can render a complete string to a buffer, but there's no way to extract the glyphs and offsets. For simple (western) writing systems one can easily lay text out manually by treating each character as a separate glyph and accumulating the advance of that glyph, and this is what many Android game engines do. This means that you lose ligatures and kerning pairs (e.g.: in the string "To" the "o" should move to the left, to be slightly under the "T").

I really like ligatures and correct kerning, so I built Pango for Android and wrote a new `PangoRenderer` subclass to generate geometry and populate a glyph cache suitable for rendering with OpenGL. That code is included in `text/pango/*`. The code also builds for desktop Linux and would be suitable for use in any OpenGL project that needs high-quality text layout.

Example
=======

There's an example of rendering text using the `ViewdoPangoRenderer` code in `examples/text.cpp`. If everything works out, you should have a window that looks like this:
![example](https://github.com/iamralpht/raw/master/examples/text-screenshot.png)

Building for Android
====================

Download and install the Android NDK, and follow the instructions to create a standalone toolchain. I used NDK version r9d. Clone this repository into your `jni/` directory. Then:

*   set the `ANDROID_NDK` environment variable to point to your NDK installation.
*   include the standalone toolchain in your `PATH` so that `arm-linux-androideabi-gcc` is found.
*   cd into `build/` and run `build.sh`
*   wait a few minutes while everything (hopefully) compiles cleanly.

Now that you've built the dependencies, you can run `ndk-build` in your `jni/` directory and build the `ViewdoPangoRenderer` as well as the JNI wrapper.

Building for Linux
==================

If you have the Pango development dependencies installed, as well as the JDK, you can just run `make` and build a shared object suitable for loading into a Java program.

Calling from Java
=================

The Java code that Letterplex uses is included in `java/`. There are no samples, although I hope to add some in the future.
