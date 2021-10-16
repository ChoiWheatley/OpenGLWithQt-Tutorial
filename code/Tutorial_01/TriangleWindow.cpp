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
#include <QColor>
#include <vector>

TriangleWindow::TriangleWindow() :
	m_program(nullptr)
{
}


TriangleWindow::~TriangleWindow() {
    // resource cleanup
    // --------------------------

    // Since we release resources related to an opengl context,
    // _we make this context current before cleaning up our resources
    m_context->makeCurrent(this);

    m_vao.destroy();
    m_vbo.destroy();
    m_ibo.destroy();
    delete m_program;

}


void TriangleWindow::initialize() {
	// this function is called once, when the window is first shown, i.e. when
    // _the the window content is first rendered

	// build and compile our shader program
    // ------------------------------------------------------------------

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
        0.8f, 0.8f, 0.0f,   // top right
        0.8f, -0.8f, 0.0f,  // bottom right
        -0.8f, -0.8f, 0.0f, // bottom left
        -0.8f, 0.8f, 0.0f   // top left
	};

    QColor vertexColors[] = {
        QColor(0xf6a509) ,
        QColor(0xcb2dde),
        QColor(0x0eeed1),
        QColor(0x068918),
    };


    // The still separate data is now copied to a common storage area
    // VBO 안에 interleaved 형태로 저장을 할 것임.
    // 0 + 4*n 번째 인덱스 = 버텍스 좌표
    // 1 + 4*n 번째 인덱스 = 색상 값
    // ----------------------------------------------------------------

    // Create buffer for 2 interleaved attributes: position and color,
    // _4 vertices, 3 floats each
    std::vector<float> vertexBufferData(2*4*3);
    // Create new data buffer
    // Copy data in interleaved mode with pattern
    // p0c0 | p1c1 | p2c2 | p3c3
    float *buf = vertexBufferData.data();
    for (int v=0; v<4; ++v, buf += 6) {
        // coordinates
        buf[0] = vertices[3*v];
        buf[1] = vertices[3*v+1];
        buf[2] = vertices[3*v+2];
        // colors
        buf[3] = vertexColors[v].redF();
        buf[4] = vertexColors[v].greenF();
        buf[5] = vertexColors[v].blueF();
    }

    // create a new buffer for the vertices and colors, in a interleaved manner
    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);      // create VBO
    m_vbo.create();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);        // will be used in `allocate()`

    m_vbo.bind();    // Set it active in the context, so that we can write to it
    // int bufSize = sizeof(vertices) = 9*sizeof(float) = 9*4=36bytes
    // static array with 9 floats(3x3 vectors) is first defined.
    m_vbo.allocate(vertexBufferData.data(),
                   vertexBufferData.size() * sizeof(float));      // Copy data into buffer

    // Initialize the VAO to record and remember subsequent attribute associations with
    // _generated vertex buffers
    m_vao.create();     // create underlying opengl obj
    // NOTICE: VAO manages not only the attributes, but also bound buffers.
    //         Therefore, the VAO must be bound `before` the element buffer in order
    //         to store the state correctly
    m_vao.bind();       // sets the VAO current to the opengl context so it monitors
                        // attribute assignments


    // Element Buffer
    // -------------------------------------------------------------------------
    unsigned int indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };
    m_ibo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ibo.create();
    m_ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ibo.bind();
    m_ibo.allocate(indices, sizeof(indices));




    // Now, all following calls are `recorded` in the currently bound VAO.
    // When using mixed vertex arrays (several attributes per vertex), you have to
    // _specify the memory structure and the mapping of the attributes to the shader
    // _program
    // ---------------------------------------------------------------------------
    int stride = 6 * sizeof(float);             // Number of bytes for one vertex
    // layout location 0 - vec3 with coordinates
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0             // location is equal to 0
                                  , GL_FLOAT    // type
                                  , 0           // offset: in the currently bound vertex buffer
                                  , 3           // tuple_size: the number components per index
                                  , stride);    // stride: vertices are densely packed in the value array
    // layout location 1 - vec3 with colors
    m_program->enableAttributeArray(1);
    int colorOffset = 3*sizeof(float);
    m_program->setAttributeBuffer(1, GL_FLOAT, colorOffset, 3, stride);

	// Release (unbind) all
    // NOTICE: the ORDER of binding and unbinding is crucial
    m_vbo.release();
    m_vao.release();
}


void TriangleWindow::render() {
    // This function is called for every frame to be rendered on screen
    // ------------------------------------------------------------------
    const qreal retinaScale = devicePixelRatio();       // needed for Macs with retina display
    glViewport(0,0,width() * retinaScale, height() * retinaScale);      // set the viewport

    // set the background color = clear color
    // As long as the background color does not change, you could also move this call to
    // _initialization
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // use our shader program
    m_program->bind();
    // bind the vao, which in turn binds the vbo and sets the attribute buffer in the opengl context
    m_vao.bind();
    m_ibo.bind();

    // Now draw the two triangles via index drawing
    glDrawElements(GL_TRIANGLES         // mode
                   , 6                  // count
                   , GL_UNSIGNED_INT    // type
                   , nullptr);          // indices

    // Finally, release(=unbind) vao again
    m_vao.release();
}
