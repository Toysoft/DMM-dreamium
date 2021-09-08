QT += webengine webenginewidgets

CONFIG += c++11 link_pkgconfig

PKGCONFIG += alsa

HEADERS += \
	webengineview.h \
	volume.h

SOURCES += \
	main.cpp \
	webengineview.cpp \
	volume.cpp
