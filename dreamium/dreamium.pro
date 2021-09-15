QT += webengine webenginewidgets

CONFIG += c++11 link_pkgconfig

PKGCONFIG += alsa

RESOURCES = dreamium.qrc

HEADERS += \
	webengineview.h \
	volume.h

SOURCES += \
	main.cpp \
	webengineview.cpp \
	volume.cpp

target.path = $$[QT_INSTALL_BINS]
INSTALLS += target
