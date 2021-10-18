/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#ifndef TriangleWindow_H
#define TriangleWindow_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "OpenGLWindow.h"
#include "mousehandler.h"
#include "Camera.h"
#include "mytransform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::mat4 변수를 배열 포인터로 변환하는 `value_ptr()` 함수

class TriangleWindow : public OpenGLWindow {
public:
	TriangleWindow();
    ~TriangleWindow() override;

protected:
    void initialize() override;
    void render() override;
    void resizeGL(int width, int height) override;

    // Functions to handle mouse EVENTS, all the work is done in class `MouseHandler`
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    // mouse event loop. Tests if any relevant input was received and registers a state change
    void checkInput();
    // Called very first in the paintGL() routine and updates camera position
    void processInput();
    // View + Projection mat => m_world2View mat
    void updateWorldToViewMat();

private:
	// Wraps an OpenGL VertexArrayObject (VAO)
	QOpenGLVertexArrayObject	m_vao;
    // Vertex buffer
    QOpenGLBuffer				m_vbo;
    // Index buffer
    QOpenGLBuffer               m_ibo;


	// Holds the compiled shader programs.
	QOpenGLShaderProgram		*m_program;

    // texture and image
    QOpenGLTexture              *m_texture;

    // input handler that encapsulates the event handling code
    bool                        m_inputEventReceived;
    MouseHandler                m_mouseHandler;

    // View + projection matrix
    glm::mat4                   m_worldToView;

    glm::mat4 m_model = glm::mat4(1.0f);      // model matrix
    glm::mat4 m_view = glm::mat4(1.0f);       // view matrix
    glm::mat4 m_proj;                         // projection matrix
};

#endif // TriangleWindow_H
