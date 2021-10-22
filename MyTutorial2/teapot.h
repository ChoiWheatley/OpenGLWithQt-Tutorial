#ifndef TEAPOT_H
#define TEAPOT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector2D>
#include <QList>
#include <QVector>

class QOpenGLShaderProgram;

// 삼각형 하나를 정의하는 구조체
struct Triangle3D {
    QVector3D p1;
    QVector3D p2;
    QVector3D p3;

    QVector3D p1Normal;
    QVector3D p2Normal;
    QVector3D p3Normal;

    QVector2D p1UV;
    QVector2D p2UV;
    QVector2D p3UV;
};

class TeaPot
{
public:
    void create(QOpenGLShaderProgram * shaderProgram);
    void destroy();

    void render();

    unsigned int m_bufferSize;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;

    QList<Triangle3D> m_triangles;
    // vertex, normal coord
    QVector<QVector3D> m_vertex, m_vertex_normal;
    // texture coord
    QVector<QVector2D> m_vertex_texture;
    // comments
    QStringList m_comments;

    // obj load functions
    void loadModel();


};

#endif // TEAPOT_H
