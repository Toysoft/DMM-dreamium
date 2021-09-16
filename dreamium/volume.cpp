#include "volume.h"
#include <algorithm>

#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

#include "webengineview.h"

Volume::Volume(WebEngineView* view)
    : m_webView(view)
    , m_hdmiMixer(NULL)
    , m_hdmiElem(NULL)
    , m_muted(false)
{
    init();
}

Volume::~Volume()
{
    if (m_hdmiMixer)
        snd_mixer_close(m_hdmiMixer);
}

void Volume::init()
{
    if (m_hdmiElem != 0) {
        qInfo() << "Existing Mixer: " << m_hdmiMixer << " :: Mixer Elem: " << m_hdmiElem;
        return;
    }
    const char *card = "default";
    if (snd_mixer_open(&m_hdmiMixer, 0)) {
        qWarning() << "Opening Mixer FAILED!";
        reset();
        return;
    }

    if (snd_mixer_attach(m_hdmiMixer, card)) {
        qWarning() << "Attaching Mixer FAILED!";
        reset();
        return;
    }
    if (snd_mixer_selem_register(m_hdmiMixer, NULL, NULL)) {
        qWarning() << "Registering mixer selem FAILED!";
        reset();
        return;
    }

    if (snd_mixer_load(m_hdmiMixer)) {
        qWarning() << "Loading mixer FAILED!";
        reset();
        return;
    }

    const char *selem_name = "Master";
    snd_mixer_selem_id_t *sid;

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(m_hdmiMixer, sid);
    if (elem == NULL) {
        qWarning() << "Volume control element not available!";
        reset();
        return;
    }

    m_hdmiElem = elem;

    snd_mixer_selem_get_playback_volume_range(m_hdmiElem, &m_min, &m_max);
    long volume;
    snd_mixer_selem_get_playback_volume(m_hdmiElem, SND_MIXER_SCHN_MONO, &volume);
    m_volume = volume * 100 / m_max;
    qInfo() << "Mixer: " << m_hdmiMixer << " :: Mixer Elem: " << m_hdmiElem;
    qInfo() << "Volume: " << m_volume;
}

void Volume::reset()
{
    if (m_hdmiMixer) {
        snd_mixer_close(m_hdmiMixer);
        m_hdmiMixer = NULL;
    }
}

bool Volume::available()
{
    if (m_hdmiElem == NULL)
        init();
    bool avail = m_hdmiElem != NULL;
    if (!avail)
        m_webView->onVolumeChanged(-1);
    return avail;
}

bool Volume::isMuted()
{
    return m_muted;
}

int Volume::get()
{
    return m_volume;
}

void Volume::set(int volume)
{
    if (!available())
        return;
    if (volume == 0) {
        m_muted = true;
    } else {
        m_muted = false;
        m_volume = volume;
    }
    qInfo() << "Volume: " << volume;
    snd_mixer_selem_set_playback_volume_all(m_hdmiElem, volume * m_max / 100);
    m_webView->onVolumeChanged(volume);
}

bool Volume::toggleMute()
{
    if (!available())
        return m_muted;
    int vol = m_muted ? m_volume : 0;
    set(vol);
    return m_muted;
}

void Volume::up()
{
    if (!available())
        return;
    set(std::min(get()+5, 100));
}

void Volume::down()
{
    if (!available())
        return;
    set(std::max(get()-5, 0));
}
