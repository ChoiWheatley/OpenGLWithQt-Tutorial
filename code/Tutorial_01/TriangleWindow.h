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

/*	This is the window that shows the triangle.
	We derive from our OpenGLWindow base class and implement the
	virtual initialize() and render() functions.
*/
class TriangleWindow : public OpenGLWindow {
public:
	TriangleWindow();
	~TriangleWindow() Q_DECL_OVERRIDE;

    void initialize() Q_DECL_OVERRIDE;
	void render() Q_DECL_OVERRIDE;

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
};

#endif // TriangleWindow_H
