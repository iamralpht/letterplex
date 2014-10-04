// Example of text usage, using glfw for the window system binding.
// Copyright 2014 (C) Ralph Thomas, Palo Alto, CA

#define GL_GLEXT_PROTOTYPES 1
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <pango/pango-layout.h>
#include <text/pango/ViewdoPangoRenderer.h>
#include <text/pango/ViewdoGLRenderer.h>

// OpenGL boilerplate.
GLuint loadShader ( GLenum type, const char *shaderSrc )
{
    GLuint shader;
    GLint compiled;
         
    // Create the shader object
    shader = glCreateShader (type);
    if (!shader) return 0;

    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) return 0;
    return shader;
}
void ortho2D(float* mat, float left, float right, float bottom, float top)
{
    // this is basically from
    // http://en.wikipedia.org/wiki/Orthographic_projection_(geometry)
    const float zNear = -1.0f;
    const float zFar = 1.0f;
    const float inv_z = 1.0f / (zFar - zNear);
    const float inv_y = 1.0f / (top - bottom);
    const float inv_x = 1.0f / (right - left);

    //first column
    *mat++ = (2.0f*inv_x);
    *mat++ = (0.0f);
    *mat++ = (0.0f);
    *mat++ = (0.0f);

    //second
    *mat++ = (0.0f);
    *mat++ = (2.0*inv_y);
    *mat++ = (0.0f);
    *mat++ = (0.0f);

    //third
    *mat++ = (0.0f);
    *mat++ = (0.0f);
    *mat++ = (-2.0f*inv_z);
    *mat++ = (0.0f);

    //fourth
    *mat++ = (-(right + left)*inv_x);
    *mat++ = (-(top + bottom)*inv_y);
    *mat++ = (-(zFar + zNear)*inv_z);
    *mat++ = (1.0f);
}
void setProjection(int uniform, int width, int height) {
    float mat[16];
    ortho2D(mat, 0, width, height, 0);
    glUniformMatrix4fv(uniform, 1, false, mat);
}
static const char* VERTEX_SHADER =
R"(
#ifndef GL_ES
#define highp
#else
precision mediump float;
#endif
attribute vec4 position;
attribute vec2 uv;

varying vec2 v_texCoord;

uniform mat4 projection;

void main(void) {
    v_texCoord = uv;
    gl_Position = projection * position;
}
)";

static const char* FRAGMENT_SHADER =
R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;

uniform vec4 color;
uniform sampler2D texture;

void main(void) {
    gl_FragColor = texture2D(texture, v_texCoord).a * color;
}
)";


// Actually interesting text stuff

static PangoContext* sContext = nullptr;
static PangoFontMap* sFontMap = nullptr;

static PangoContext* context() {
    if (sContext) return sContext;
    sContext = pango_context_new();
    sFontMap = pango_ft2_font_map_new();
    pango_context_set_font_map(sContext, sFontMap);

    return sContext;
}

void build_runs(const char* markup, std::vector<ViewdoTextRun>& out) {
    PangoLayout* layout(pango_layout_new(context()));

    pango_layout_set_markup(layout, markup, -1);
    pango_layout_set_width(layout, 400 * PANGO_SCALE);

    PangoFontDescription* font(pango_font_description_new());
    pango_font_description_set_family(font, "Ubuntu");
    pango_font_description_set_size(font, 32 * 1000);
    pango_layout_set_font_description(layout, font);
    pango_font_description_free(font);

    viewdo_pango_render(layout, 10, 10, 1.0f, out);

    g_object_unref(layout);
}

int main() {
    GLFWwindow* window;
    if (!glfwInit()) return 1;

    window = glfwCreateWindow(640, 480, "Pango with OpenGL and a glyph cache", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glClearColor(1, 1, 1, 1);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "uv");
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(program);
    GLuint projectionLocation = glGetUniformLocation(program, "projection");
    GLuint colorLocation = glGetUniformLocation(program, "color");


    std::vector<ViewdoTextRun> runs;
    build_runs("Hello <b>World</b>\nThis is some <span fgcolor=\"#ff0000\">markup</span> that has been <i>parsed</i> and laid out by <b>Pango!</b>", runs);

    while (!glfwWindowShouldClose(window))
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        setProjection(projectionLocation, width, height);

        glClear(GL_COLOR_BUFFER_BIT);

        viewdo_pango_gl_render(runs, colorLocation, 1.f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
