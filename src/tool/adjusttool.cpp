#include "editor.h"

#include "adjusttool.h"

AdjustTool::AdjustTool(QObject *parent) :
    BaseTool(parent)
{
}

ToolType AdjustTool::type()
{
    return ADJUST;
}

QCursor AdjustTool::cursor()
{
    QPixmap pixmap ( m_currentValue, m_currentValue );
    pixmap.fill( QColor(255,255,255,0) );

    QPainter painter(&pixmap);
    painter.setPen( QColor(0,0,0,190) );
    painter.setBrush( QColor(255,255,255,100) );
    painter.drawLine( QPointF(m_currentValue/2 - 2, m_currentValue/2), QPointF(m_currentValue/2+2, m_currentValue/2) );
    painter.drawLine( QPointF(m_currentValue/2, m_currentValue/2-2), QPointF(m_currentValue/2, m_currentValue/2+2) );
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setPen( QColor(0,0,0,100) );
    painter.drawEllipse( QRectF(1, 1, m_currentValue - 2, m_currentValue - 2) );
    painter.end();

    return QCursor(pixmap);
}

void AdjustTool::mousePressEvent(QMouseEvent *)
{
}

void AdjustTool::mouseMoveEvent(QMouseEvent *)
{
    QPointF offset = getCurrentPoint() - getLastPressPoint();
    qreal incx = pow(m_value * 100, 0.5);
    qreal newValue = incx + offset.x();

    if (newValue < 0)
    {
        newValue = 0;
    }
    newValue = pow(newValue, 2) / 100;

    if (newValue<0.2)
    {
        newValue = 0.2;
    }
    else if (newValue>200)
    {
        newValue = 200;
    }

    m_currentValue = newValue;

    switch (m_mode)
    {
    case ADJUST_WIDTH:
        m_pEditor->applyWidth( newValue );
        break;

    case ADJUST_FEATHER:
        m_pEditor->applyFeather( newValue );
        break;

    case ADJUST_TRANSPARENT:
        // XXX todo
        break;
    }
}

void AdjustTool::mouseReleaseEvent(QMouseEvent *)
{

}
