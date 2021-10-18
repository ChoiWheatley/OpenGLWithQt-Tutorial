/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

Source code is based on Qt Example OpenGLWindow, but has been simplified a lot.

************************************************************************************/

#include "OpenGLWindow.h"

#include <QCoreApplication>
#include <QOpenGLContext>

OpenGLWindow::OpenGLWindow(QWindow *parent) :
	QWindow(parent),
	m_context(nullptr)
{
    // Specify that you want to create a native opengl window
	setSurfaceType(QWindow::OpenGLSurface);
}


void OpenGLWindow::renderLater() {
	// Schedule an UpdateRequest event in the event loop
	// that will be send with the next VSync.
    requestUpdate();    // call public slot requestUpdate()
                        // when next VSync kicks in, new event `QEvent::UpdateRequest`
                        // will be emitted
}


bool OpenGLWindow::event(QEvent *event) {
	switch (event->type()) {
    case QEvent::UpdateRequest:     // We should draw the widget again!
		renderNow();
		return true;
	default:
		return QWindow::event(event);
	}
}


// The window (or part of it) is now visible and should be updated
void OpenGLWindow::exposeEvent(QExposeEvent * /*event*/) {
    renderNow(); // simply redirect call to renderNow()
}

void OpenGLWindow::resizeEvent(QResizeEvent *event)
{
    QWindow::resizeEvent(event);
    if(m_context == nullptr) {
        renderNow();
    }
    resizeGL(width(), height());
}


void OpenGLWindow::renderNow() {
    // explicitly loaded all stuffs under the hood
	if (!isExposed())
		return;

	bool needsInitialize = false;

	// initialize on first call
	if (m_context == nullptr) {
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();

		needsInitialize = true;
	}

    // This call transfers the contents of the context object to the opengl state
	m_context->makeCurrent(this);

	if (needsInitialize) {
        // The platform-specific opengl libs are dynamically integrated and the
        // function pointers are brought to the native opengl functions.
		initializeOpenGLFunctions();
        initialize(); // user code -- we have set this as virtual
	}

    // actual rendering of the 3D scene must then be done by the user in the function
	render(); // user code

    // Because new context has been created, you must swap from the current one
	m_context->swapBuffers(this);
}

