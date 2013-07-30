#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "basetool.h"


class SelectTool : public BaseTool
{
    Q_OBJECT
public:
    SelectTool(QObject *parent = NULL);
    ToolType type();
    void loadSettings();
    QCursor cursor();

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
};

#endif
