/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

Source code is based on Qt Example OpenGLWindow, but has been simplified a lot.

************************************************************************************/

#ifndef OpenGLWindow_H
#define OpenGLWindow_H

#include <QWindow>
#include <QOpenGLFunctions>

QT_BEGIN_NAMESPACE
class QOpenGLContext;
QT_END_NAMESPACE

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit OpenGLWindow(QWindow *parent = nullptr);

    virtual void render() = 0;      // from QOpenGLFunctions
    virtual void initialize() = 0;  // from QOpenGLFunctions
    virtual void resizeGL(int width, int height) {Q_UNUSED(width) Q_UNUSED(height)}

public slots:
	void renderLater();
	void renderNow();

protected:
    bool event(QEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

	QOpenGLContext *m_context;
};

#endif // OpenGLWindow_H
