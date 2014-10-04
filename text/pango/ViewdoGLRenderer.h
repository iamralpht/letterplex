// Render a set of runs using the current shader
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#ifndef VIEWDO_GL_RENDERER_H
#define VIEWDO_GL_RENDERER_H

#include <text/pango/ViewdoPangoRenderer.h>
#include <vector>

//
// Render the text runs (generated from a PangoLayout by viewdo_pango_render) using the currently
// bound shader. This shader should already have the projection (and any other uniforms) set up.
//
// @param runs          the text runs to render.
// @param colorUniform  location of the color uniform, should be an rgba vec4, will be premultiplied with opacity.
// @param opacity       the opacity to mix into the color.
//
void viewdo_pango_gl_render(const std::vector<ViewdoTextRun>& runs, int colorUniform, float opacity);

//
// The OpenGL context was lost, so all textures should be ditched.
//
void viewdo_pango_gl_context_lost();

#endif
