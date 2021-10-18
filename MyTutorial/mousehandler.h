#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <QPoint>
#include <QMouseEvent>

class QMouseEvent;
class QWheelEvent;

class MouseHandler
{
public:
    MouseHandler();
    virtual ~MouseHandler();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    // When mouse button is
    bool pressButton(Qt::MouseButton btn, QPoint currentPos);
    bool releaseButton(Qt::MouseButton btn);
    bool isButtonDown(Qt::MouseButton btn) const;

    // Use this function to determine whether the mouse has been moved (by comparing it to the QCursor::pos())
    QPoint mouseDownPos() const;
    QPoint resetMouseDelta(const QPoint currentPos);
    int wheelDelta() const;
    int resetWheelDelta();

private:
    enum KeyStates {
        StateNotPressed,
        StateHeld,
        StateWasPressed
    };
    KeyStates   m_leftButtonDown;
    KeyStates   m_rightButtonDown;
    QPoint      m_mouseDownPos;
    int         m_wheelDelta;
};

#endif // MOUSEHANDLER_H
