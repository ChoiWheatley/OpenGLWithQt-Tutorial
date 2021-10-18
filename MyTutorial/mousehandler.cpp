#include "mousehandler.h"

MouseHandler::MouseHandler() :
    m_leftButtonDown(StateNotPressed),
    m_rightButtonDown(StateNotPressed),
    m_wheelDelta(0)
{
}

MouseHandler::~MouseHandler()
{

}

void MouseHandler::mousePressEvent(QMouseEvent *event)
{
    pressButton(static_cast<Qt::MouseButton>(event->button()), event->globalPos());
}

void MouseHandler::mouseReleaseEvent(QMouseEvent *event)
{
    releaseButton(static_cast<Qt::MouseButton>(event->button()));
}

void MouseHandler::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numPixels.isNull()) {
        m_wheelDelta += numPixels.y();
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        m_wheelDelta += numSteps.y();
    }

    event->accept();
}

bool MouseHandler::pressButton(Qt::MouseButton btn, QPoint currentPos)
{
    switch (btn) {
    case Qt::LeftButton :
        m_leftButtonDown = StateHeld;
        break;
    case Qt::RightButton :
        m_rightButtonDown = StateHeld;
        break;
    default: return false;
    }
    m_mouseDownPos = currentPos;
    return true;
}

bool MouseHandler::releaseButton(Qt::MouseButton btn)
{
    switch (btn) {
    case Qt::LeftButton :
        m_leftButtonDown = StateWasPressed;
        break;
    case Qt::RightButton :
        m_rightButtonDown = StateWasPressed;
        break;
    default: return false;
    }
    return true;
}

bool MouseHandler::isButtonDown(Qt::MouseButton btn) const
{
    switch (btn) {
    case Qt::LeftButton: return m_leftButtonDown == StateHeld;
    case Qt::RightButton: return m_rightButtonDown == StateHeld;
    default: return false;
    }
}

QPoint MouseHandler::mouseDownPos() const
{
    return m_mouseDownPos;
}

QPoint MouseHandler::resetMouseDelta(const QPoint currentPos)
{
    QPoint dist = currentPos - m_mouseDownPos;
    m_mouseDownPos = currentPos;
    return dist;
}

int MouseHandler::wheelDelta() const
{
    return m_wheelDelta;
}

int MouseHandler::resetWheelDelta()
{
    int wd = m_wheelDelta;
    m_wheelDelta = 0;
    return wd;
}
