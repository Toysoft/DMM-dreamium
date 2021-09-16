#pragma once

extern "C" {
#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
}

#include <QTimer>
#include <QProgressBar>
#include <QObject>

class WebEngineView;

class Volume : public QObject
{
    WebEngineView* m_webView;
    snd_mixer_t* m_hdmiMixer;
    snd_mixer_elem_t *m_hdmiElem;
    bool m_muted;
    int m_volume;
    long m_min;
    long m_max;

    void init();
    void reset();

    int get();
    void set(int value);
public:
    Volume(WebEngineView* webView);
    ~Volume();

    bool available();
    bool isMuted();
    bool toggleMute();
    void up();
    void down();
};