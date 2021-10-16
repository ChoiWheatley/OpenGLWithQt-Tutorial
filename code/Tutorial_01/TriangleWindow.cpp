/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "TriangleWindow.h"

#include <QDebug>

TriangleWindow::TriangleWindow() :
	m_program(nullptr)
{
}


TriangleWindow::~TriangleWindow() {
    // resource cleanup
    // --------------------------

    // Since we release resources related to an opengl context,
    // -we make this context current before cleaning up our resources
    m_context->makeCurrent(this);

    m_vao.destroy();
    m_vbo.destroy();
    delete m_program;

}


void TriangleWindow::initialize() {
	// this function is called once, when the window is first shown, i.e. when
	// the the window content is first rendered

	// build and compile our shader program
	// ------------------------------------

	m_program = new QOpenGLShaderProgram();

	// read the shader programs from the resource
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/pass_through.vert"))
		qDebug() << "Vertex shader errors:\n" << m_program->log();

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/uniform_color.frag"))
		qDebug() << "Fragment shader errors:\n" << m_program->log();

	if (!m_program->link())
		qDebug() << "Shader linker errors:\n" << m_program->log();


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

    // create a new buffer for the vertices
    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);      // create VBO
    m_vbo.create();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);        // will be used in `allocate()`

    m_vbo.bind();    // Set it active in the context, so that we can write to it
    // int bufSize = sizeof(vertices) = 9*sizeof(float) = 9*4=36bytes
    // static array with 9 floats(3x3 vectors) is first defined.
    m_vbo.allocate(vertices, sizeof(vertices));      // Copy data into buffer

    // Initialize the VAO to record and remember subsequent attribute associations with
    // generated vertex buffers
    m_vao.create();     // create underlying opengl obj
    m_vao.bind();       // sets the VAO current to the opengl context so it monitors
                        // attribute assignments
    // Now, all following calls are `recorded` in the currently bound VAO
    // Enable attribute array at layout location 0
    // In vertex shader, we have seen the code `layout (location = 0) in vec3 position`
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0             // location is equal to 0
                                  , GL_FLOAT    // type
                                  , 0           // offset: in the currently bound vertex buffer
                                  , 3           // tuple_size: the number components per index
                                  , 0);         // stride: vertices are densely packed in the value array

	// Release (unbind) all
    m_vbo.release();
	m_vao.release();
}


void TriangleWindow::render() {
    // This function is called for every frame to be rendered on screen
    // ----------------------
    const qreal retinaScale = devicePixelRatio();       // needed for Macs with retina display
    glViewport(0,0,width() * retinaScale, height() * retinaScale);      // set the viewport

    // set the background color = clear color
    // As long as the background color does not change, you could also move this call to
    // -initialization
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // use our shader program
    m_program->bind();
    // bind the vao, which in turn binds the vbo and sets the attribute buffer in the opengl context
    m_vao.bind();

    // Now, draw the triangles
    glDrawArrays(GL_TRIANGLES   // draw individual triangles
                 , 0            // index of first triangle to draw
                 , 3);          // number of vertices to process

    // Finally, release(=unbind) vao again
    m_vao.release();
}
