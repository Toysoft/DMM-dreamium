#include "webengineview.h"

#include <QKeyEvent>
#include <QScreen>
#include <QWebEnginePage>
#include <QWebEngineHistory>

WebEngineView::WebEngineView(QApplication* app, QWidget *parent)
    : QWebEngineView(parent)
    , m_app(app)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    m_closeHint = new QLabel("Press power key to quit!");
    m_closeHint->setGeometry(0, 0, 520, 80);
    m_closeHint->setAlignment(Qt::AlignCenter);
    int x = (screenGeometry.width() - m_closeHint->width()) / 2;
    int y = (screenGeometry.height() - m_closeHint->height()) / 2;
    m_closeHint->setGeometry(x, y, 520, 80);
    m_closeHint->hide();

    m_volumebar = new QProgressBar(parent);
    m_volumebar->setMinimum(0);
    m_volumebar->setMaximum(100);

    m_volumebar->setGeometry(0, 0, 600, 60);
    m_volumebar->setAlignment(Qt::AlignCenter);
    x = (screenGeometry.width() - m_volumebar->width()) / 2;
    y = (screenGeometry.height() - m_volumebar->height()) / 2;
    m_volumebar->setGeometry(x, y, 600, 60);
    m_volumebar->hide();

    m_volume = new Volume(this);
    m_volumeTimer = new QTimer();

    connect(m_volumeTimer, &QTimer::timeout, this, &WebEngineView::hideVolumebar);    
    connect(this, &QWebEngineView::loadFinished, this, &WebEngineView::loadFinished);
    connect(page(), &QWebEnginePage::windowCloseRequested, this, &WebEngineView::windowCloseRequested);

    m_app->installEventFilter(this);
}

WebEngineView::~WebEngineView()
{
    m_app->removeEventFilter(this);
    delete m_volumeTimer;
    delete m_closeHint;
    delete m_volumebar;
}

bool WebEngineView::eventFilter(QObject *obj, QEvent *event)
{
	if(event->type() == QEvent::KeyPress)  {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() != Qt::Key_Back)
            hideUi();
		switch(keyEvent->key()) {
            case Qt::Key_Escape:
            case Qt::Key_Exit:
                onKeyBack();
                return false;
			case Qt::Key_PowerOff:
				qApp->exit(0);
				return true;
			case Qt::Key_VolumeMute:
				m_volume->toggleMute();
				return true;
			case Qt::Key_VolumeUp:
				m_volume->up();
				return true;
			case Qt::Key_VolumeDown:
				m_volume->down();
				return true;
		}
	}
	return false;
}

void WebEngineView::onVolumeChanged(int value)
{
    m_volumebar->setValue(value);
    m_volumebar->show();
    m_volumeTimer->start(2000);
}

void WebEngineView::hideVolumebar()
{
    m_volumebar->hide();
}

void WebEngineView::hideUi()
{
    m_closeHint->hide();
}

void WebEngineView::onKeyBack()
{
    if (!page()->history()->canGoBack()) {
        m_closeHint->show();
    }
}

void WebEngineView::loadFinished(bool ok)
{
}

void WebEngineView::windowCloseRequested()
{
    qApp->exit(0);
}