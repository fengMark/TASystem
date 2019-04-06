INCLUDEPATH += $$PWD $$PWD/../src
DEPENDPATH  += $$PWD $$PWD/../src

HEADERS += $$PWD/eventdispatcher.h

CONFIG  *= link_prl
LIBS    += -L$$OUT_PWD/$$DESTDIR/../lib -leventdispatcher_epoll

equals(QMAKE_PREFIX_STATICLIB, ""): QMAKE_PREFIX_STATICLIB = lib
equals(QMAKE_EXTENSION_STATICLIB, ""): QMAKE_EXTENSION_STATICLIB = a

PRE_TARGETDEPS *= $$OUT_PWD/$$DESTDIR/../lib/$${QMAKE_PREFIX_STATICLIB}eventdispatcher_epoll$${LIB_SUFFIX}.$${QMAKE_EXTENSION_STATICLIB}
