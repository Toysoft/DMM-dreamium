#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QtWebEngine>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QEvent>
#include <QKeyEvent>
#include "eventfilter.h"

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("Dreambox Webapp Browser");
    QApplication::setApplicationVersion("1.0");
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication app(argc, argv);
    QtWebEngine::initialize();

    QCommandLineParser parser;
    parser.setApplicationDescription("A Browser for webapps, defaulting to youtube.com/tv");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "URL to open");
    QCommandLineOption urlOption(QStringList() << "u"
                                                     << "url",
                                       "URL to open", "URL", "https://youtube.com/tv");
    parser.addOption(urlOption);
    QCommandLineOption userAgentOption(QStringList() << "a"
                                                     << "user-agent",
                                       "User-Agent to use", "User-Agent", "SMART-TV; Tizen 4.0");
    parser.addOption(userAgentOption);
    parser.process(app);
    const QStringList args = parser.positionalArguments();

    QString url = parser.value(urlOption);
    qInfo() << "URL: " << url;

    QString ua = parser.value(userAgentOption);
    qInfo() << "User-Agent: " << ua;

    QWebEngineProfile::defaultProfile()->setHttpUserAgent(ua);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);

    QWebEngineView view;
    DreamboxEventFilter *filter = new DreamboxEventFilter();
    app.installEventFilter(filter);

    view.setUrl(QUrl(url));
    view.showFullScreen();
    return app.exec();
}
