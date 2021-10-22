#include "teapot.h"
#include <QFile>
#include <QRegularExpression>
#include <QOpenGLShaderProgram>

void TeaPot::create(QOpenGLShaderProgram *shaderProgram)
{
    loadModel();

    m_vao.create();
    m_vao.bind();

    // create and bind vertex buffer
    m_vbo.create();
    m_vbo.bind();
    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int siz = m_vertex.size() * sizeof(QVector3D);
    m_vbo.allocate(m_vertex.data(), siz);

    // set shader attributes
    // layout(location = 0) = vec2 position
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0             // position
                                      , GL_FLOAT    // type
                                      , 0           // position offset
                                      , 3           // tuple size
                                      , 0);         // stride
    m_vao.release();
    m_vbo.release();
}

void TeaPot::destroy()
{
    m_vao.destroy();
    m_vbo.destroy();
}

void TeaPot::render()
{

}

void TeaPot::loadModel()
{
    m_triangles.clear();
    QFile file(QStringLiteral("qrc:/teapot.obj"));
    if (file.exists() && file.open(QFile::ReadOnly | QFile::Text)) {
        while(!file.atEnd()) {
            QString line = file.readLine().trimmed();
            auto lineParts = line.split(QRegularExpression("\\s+"));
            if (lineParts.count() > 0) {
                if (lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0) {
                    // comments
                    m_comments.append(line.remove(0, 1).trimmed());
                } else if (lineParts.at(0).compare("v", Qt::CaseInsensitive) == 0) {
                    // vertex
                    m_vertex.append(QVector3D(lineParts.at(1).toFloat()
                                              , lineParts.at(2).toFloat()
                                              , lineParts.at(3).toFloat()));
                } else if (lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0) {
                    // vertex normal
                    m_vertex_normal.append(QVector3D(lineParts.at(1).toFloat()
                                                     , lineParts.at(2).toFloat()
                                                     , lineParts.at(3).toFloat()));
                }  else if (lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0) {
                    // face
                    // there is an assumption here that faces are all triangles
                    Triangle3D triangle;
                    // get points from vertex
                    triangle.p1 = m_vertex.at(lineParts.at(1).split("/").at(0).toInt() - 1);
                    triangle.p2 = m_vertex.at(lineParts.at(2).split("/").at(1).toInt() - 1);
                    triangle.p3 = m_vertex.at(lineParts.at(3).split("/").at(1).toInt() - 1);

                    // get normals from vertex normal
                    triangle.p1Normal = m_vertex_normal.at(lineParts.at(1).split("/").at(0).toInt()-1);
                    triangle.p2Normal = m_vertex_normal.at(lineParts.at(2).split("/").at(0).toInt()-1);
                    triangle.p3Normal = m_vertex_normal.at(lineParts.at(3).split("/").at(0).toInt()-1);

                    m_triangles.append(triangle);
                }  else if (lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0) {
                    // texture
                    m_vertex_texture.append(QVector2D(lineParts.at(1).toFloat()
                                                      , lineParts.at(2).toFloat()));
                }  else if (lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0) {
                }  else if (lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0) {
                }
            }
        }
        file.close();
    }
}
