#include <stdlib.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QtWebEngine>
#include <QWebEngineSettings>
#include <QWebEngineProfile>

#include "webengineview.h"

void setupApplication()
{
    qputenv("QT_QPA_PLATFORM", "eglfs");
    qputenv("QT_QPA_EGLFS_SWAPINTERVAL", "0");
    qputenv("QT_QPA_EGLFS_HIDECURSOR", "1");

    QApplication::setApplicationName("Dreambox Webapp Browser");
    QApplication::setApplicationVersion("1.0");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
}

void setupWebengine(QString ua)
{
    QtWebEngine::initialize();
    QWebEngineProfile::defaultProfile()->setHttpUserAgent(ua);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
}

int main(int argc, char *argv[])
{
    setupApplication();
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("A Browser for webapps, defaulting to youtube.com/tv");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "URL to open");
    QCommandLineOption urlOption(QStringList()  << "u" << "url",
                                    "URL to open", "URL", "https://youtube.com/tv");
    parser.addOption(urlOption);
    QCommandLineOption userAgentOption(QStringList() << "a" << "user-agent",
                                    "User-Agent to use", "User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36; SMART-TV; Tizen 4.0");
    parser.addOption(userAgentOption);
    parser.process(app);
    const QStringList args = parser.positionalArguments();

    QString url = parser.value(urlOption);
    qInfo() << "URL: " << url;

    QString ua = parser.value(userAgentOption);
    qInfo() << "User-Agent: " << ua;

    setupWebengine(ua);

    WebEngineView view(&app);
    view.setUrl(QUrl(url));
    view.showFullScreen();

    app.setStyleSheet("QProgressBar { \
                background-color: black; \
                border: 2px solid white; \
                border-radius: 0px; \
                color: white; \
                font: 24pt; \
                padding: 2px \
            } \
            \
            QProgressBar::chunk { \
                background-color: #2596be; \
                width: 1px; \
            }\
            \
            QLabel {\
                background-color: black; \
                color: white; \
                font: 24pt; \
                border-radius: 0px; \
                border: 2px solid white; \
                padding: 2px; \
            } \
    ");

    return app.exec();
}

