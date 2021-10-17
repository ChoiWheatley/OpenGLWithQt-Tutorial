/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "TriangleWindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char **argv) {
    // testing glm
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans_translate = glm::mat4(1.0f);
    glm::mat4 trans_comp = glm::mat4(1.0f);
    trans_translate = glm::translate(trans_translate, glm::vec3(1.0f, 1.0f, 0.0f));
    trans_comp = glm::rotate(trans_translate, glm::radians(90.0f), glm::vec3(0.0,0.0,1.0));
    trans_comp = glm::scale(trans_translate, glm::vec3(0.5,0.5,0.5));
    glm::vec4 tmp1 = trans_translate * vec;
    glm::vec4 tmp2 = trans_comp * vec;
    qDebug() << tmp1.x << tmp1.y << tmp1.z << '\n';
    qDebug() << tmp2.x << tmp2.y << tmp2.z << '\n';





	QGuiApplication app(argc, argv);

	// Set OpenGL Version information
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);

	TriangleWindow window;
	// Note: The format must be set before show() is called.
	window.setFormat(format);
	window.resize(640, 480);
	window.show();

	return app.exec();
}
