#include "eventfilter.h"

#include <QDebug>
#include <QApplication>
#include <QKeyEvent>

bool DreamboxEventFilter::eventFilter(QObject *obj, QEvent *event)
{
	if(event->type() == QEvent::KeyPress)  {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		qInfo() << "Key Pressed: " << keyEvent->key();
		if (keyEvent->key() == Qt::Key_PowerOff)
			qApp->exit(0);
	}
	return QObject::eventFilter(obj, event);
}
