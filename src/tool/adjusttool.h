#ifndef ADJUSTTOOL_H
#define ADJUSTTOOL_H

#include "basetool.h"

class AdjustTool : public BaseTool
{
    Q_OBJECT
public:
    explicit AdjustTool(QObject *parent = 0);
    ToolType type();
    QCursor cursor();

    void loadSettings() { }

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    typedef enum {
        ADJUST_WIDTH,
        ADJUST_FEATHER,
        ADJUST_TRANSPARENT
    } AdjustmentMode;
    void setToolAdjustmentMode(AdjustmentMode mode, qreal value)
    {
        m_mode = mode;
        m_currentValue = m_value = value;
    }
    
signals:
    
public slots:

protected:
    AdjustmentMode m_mode;
    qreal m_value;
    qreal m_currentValue;
    
};

#endif // ADJUSTTOOL_H
