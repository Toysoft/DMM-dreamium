#include <stdlib.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QScreen>
#include <QRect>
#include <QtWebEngine>
#include <QWebEngineSettings>
#include <QWebEngineProfile>

#include "webengineview.h"

void setupApplication()
{
    qputenv("QT_QPA_PLATFORM", "eglfs");
    qputenv("QT_QPA_EGLFS_SWAPINTERVAL", "0");
    qputenv("QT_QPA_EGLFS_HIDECURSOR", "1");
    qputenv("QT_QPA_EGLFS_PHYSICAL_WIDTH", "400");
    qputenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT", "225");

    QApplication::setApplicationName("Dreambox Webapp Browser");
    QApplication::setApplicationVersion("1.0");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
}

void setupWebengine(QString ua)
{
    QWebEngineProfile::defaultProfile()->setHttpUserAgent(ua);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true);
//    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
//    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, false);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
}

int main(int argc, char *argv[])
{
    setupApplication();

    QByteArrayList args = QByteArrayList()
        << QByteArrayLiteral("--disable-web-security")
        << QByteArrayLiteral("--no-sandbox")
        << QByteArrayLiteral("--log-level=0");
    const int count = args.size() + argc;
    QVector<char *> qargv(count);

    qargv[0] = argv[0];
    for (int i = 0; i < args.size(); ++i)
        qargv[i + 1] = args[i].data();
    for (int i = args.size() + 1; i < count; ++i)
        qargv[i] = argv[i - args.size()];

    int qAppArgCount = qargv.size();

    QApplication app(qAppArgCount, qargv.data());
    QtWebEngine::initialize();

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
    parser.parse(QCoreApplication::arguments());
    const QStringList pargs = parser.positionalArguments();

    QString url = parser.value(urlOption);
    qInfo() << "URL: " << url;

    QString ua = parser.value(userAgentOption);
    qInfo() << "User-Agent: " << ua;

    setupWebengine(ua);

    WebEngineView view(&app);
    view.setUrl(QUrl(url));
    view.showFullScreen();

    QFile file(":/res/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    app.setStyleSheet(styleSheet);

    return app.exec();
}

