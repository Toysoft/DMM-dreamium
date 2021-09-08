#pragma once

#include <QApplication>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QWebEngineView>

#include "volume.h"

class WebEngineView : public QWebEngineView
{
    Q_OBJECT

private:
    QApplication* m_app;
    QLabel* m_closeHint;
    QProgressBar* m_volumebar;
    QTimer* m_volumeTimer;

    Volume* m_volume;

public:
    WebEngineView(QApplication* app, QWidget *parent = Q_NULLPTR);
    ~WebEngineView();

    bool eventFilter(QObject *obj, QEvent *event);

    void onKeyBack();
    void onVolumeChanged(int value);
    void hideVolumebar();
    void hideUi();
protected Q_SLOTS:
    void loadFinished(bool ok);
};