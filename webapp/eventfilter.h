#pragma once
#include <QObject>
#include <QEvent>

class DreamboxEventFilter : public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};