/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "TriangleWindow.h"
#include "Vertices.h"
#include <vector>

#include <QDebug>
#include <QColor>
#include <QTimer>
#include <QImage>

QList< glm::vec3 > cubePositions = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

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
    // m_ibo.destroy();
    delete m_program;

}


void TriangleWindow::initialize() {
	// this function is called once, when the window is first shown, i.e. when
	// build and compile our shader program
    // ------------------------------------------------------------------

    QTimer *timer = new QTimer(this);
    // add update request to next VSync
    QObject::connect(timer, &QTimer::timeout, this, &TriangleWindow::requestUpdate);
    timer->start(10);

    // load image and bind it to the texture
    QImage img;
    if (!img.load(":/penguin.jpg")) {
        qWarning("ERROR, IMG FILE LOAD FAILED");
    }
    m_texture = new QOpenGLTexture(img.mirrored());


	m_program = new QOpenGLShaderProgram();

	// read the shader programs from the resource
	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/pass_through.vert"))
		qDebug() << "Vertex shader errors:\n" << m_program->log();

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/uniform_color.frag"))
		qDebug() << "Fragment shader errors:\n" << m_program->log();

	if (!m_program->link())
		qDebug() << "Shader linker errors:\n" << m_program->log();


    // Initialize the VAO to record and remember subsequent attribute associations with
    // _generated vertex buffers
    m_vao.create();     // create underlying opengl obj
    // NOTICE: VAO manages not only the attributes, but also bound buffers.
    //         Therefore, the VAO must be bound `before` the element buffer in order
    //         to store the state correctly
    m_vao.bind();       // sets the VAO current to the opengl context so it monitors
                        // attribute assignments

    // create a new buffer for the vertices and colors, in a interleaved manner
    m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);      // create VBO
    m_vbo.create();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);        // will be used in `allocate()`

    m_vbo.bind();    // Set it active in the context, so that we can write to it
    // int bufSize = sizeof(vertices) = 9*sizeof(float) = 9*4=36bytes
    // static array with 9 floats(3x3 vectors) is first defined.
    m_vbo.allocate(vertices,
                   sizeof(vertices) * sizeof(float));      // Copy data into buffer



    // Now, all following calls are `recorded` in the currently bound VAO.
    // When using mixed vertex arrays (several attributes per vertex), you have to
    // _specify the memory structure and the mapping of the attributes to the shader
    // _program
    // ---------------------------------------------------------------------------
    int stride = 5 * sizeof(float);             // Number of bytes for one vertex
    int textureOffset = 3 * sizeof(float);
    // layout location 0 - vec3 with coordinates
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0             // location is equal to 0
                                  , GL_FLOAT    // type
                                  , 0           // offset: in the currently bound vertex buffer
                                  , 3           // tuple_size: the number components per index
                                  , stride);    // stride: vertices are densely packed in the value array
    // layout location 1 - vec2 with textures
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(1, GL_FLOAT, textureOffset, 2, stride);

	// Release (unbind) all
    // NOTICE: the ORDER of binding and unbinding is crucial
    m_vbo.release();
    m_vao.release();
    // m_ibo.release();
}


void TriangleWindow::render() {
    // This function is called for every frame to be rendered on screen
    // _the the window content is first rendered
    // ------------------------------------------------------------------

    // Check if any keys have been pressed
    if (m_inputEventReceived) {
        processInput();
    }

    static int frame = 0;

    const qreal retinaScale = devicePixelRatio();       // needed for Macs with retina display
    glViewport(0,0,width() * retinaScale, height() * retinaScale);      // set the viewport

    // set the background color = clear color
    // As long as the background color does not change, you could also move this call to
    // _initialization
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // combine those matrices, multiplying each transformations one by one, right to left
    // model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f,1.0f,0.0f));
    // view matrix does exactly oppsite than we are thinking
    const float radius = 10.0f;
    float camX = sin(glm::radians((float)frame)) * radius;
    float camZ = cos(glm::radians((float)frame)) * radius;
    m_view = glm::lookAt(glm::vec3(camX, 0.0f, camZ)
                       , glm::vec3(0.0f, 0.0f, 0.0f)
                       , glm::vec3(0.0f, 1.0f, 0.0f)
                       );
    // We want to use perspect projection for our scene so we'll declare the projection matrix like this
    m_proj = glm::perspective(glm::radians(45.0f)                     // FOV of frustum
                            , (float)width() / (float)height()      // aspect ratio
                            , 0.1f                                  // near plane
                            , 100.0f);                              // far plane


    // use our shader program
    m_program->bind();
    // bind the vao, which in turn binds the vbo and sets the attribute buffer in the opengl context
    m_vao.bind();
    // m_ibo.bind();

    // Get their uniform location and set matrix (using glm::value_ptr)
    int model_loc = glGetUniformLocation(m_program->programId(), "model");
    int view_loc = glGetUniformLocation(m_program->programId(), "view");
    int proj_loc = glGetUniformLocation(m_program->programId(), "proj");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(m_model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(m_proj));


    // Now draw the two triangles via index drawing
    m_texture->bind();
    // glDelwElements() 함수는 Index Buffer Object의 인덱스를 사용하여 그림을 그린다.
    // glDrawElements(GL_TRIANGLES         // mode
                   // , 36                  // count
                   // , GL_UNSIGNED_INT    // type
                   // , nullptr);          // indices

    for (unsigned int i = 0; i < cubePositions.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians((float)angle), glm::vec3(1.0f, 0.3f, 0.5f));

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Finally, release(=unbind) vao again
    m_vao.release();

    frame = (frame+1) % 360;

    checkInput();
}

void TriangleWindow::resizeGL(int width, int height)
{
    // 매 프레임 마다 render() 를 호출하고 있기 때문에 굳이 여기에서 m_proj 값을 변경할
    // _이유가 없다.
}

void TriangleWindow::mousePressEvent(QMouseEvent *event)
{
    m_mouseHandler.mousePressEvent(event);
    checkInput();
}

void TriangleWindow::mouseReleaseEvent(QMouseEvent *event)
{

    m_mouseHandler.mouseReleaseEvent(event);
    checkInput();
}

void TriangleWindow::wheelEvent(QWheelEvent *event)
{
    m_mouseHandler.wheelEvent(event);
    checkInput();
}

void TriangleWindow::checkInput()
// This function is called whenever any mouse event was issued
// we test  if the current state of the key handler requires a scene update
// (aka camera move) and if so, we just set a flag to do that upon next repaint
// and we schedule a repaint
{
    // mouse L btn
    if (m_mouseHandler.isButtonDown(Qt::LeftButton)) {
        // has the mouse been moved?
        if (m_mouseHandler.mouseDownPos() != QCursor::pos()) {
            m_inputEventReceived = true;
            // qDebug() << QCursor::pos();
            renderLater();
            return;
        }
    }

    // wheel
    if (m_mouseHandler.wheelDelta() != 0) {
        m_inputEventReceived = true;
        // qDebug() << m_mouseHandler.wheelDelta();
        renderLater();
        return;
    }
}

void TriangleWindow::processInput()
{
}

void TriangleWindow::updateWorldToViewMat()
{

}
