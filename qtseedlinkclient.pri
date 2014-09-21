SOURCES +=                                      \
    $$PWD/src/qtseedlinkclient.cpp              \
    $$PWD/src/qtseedlinkstation.cpp             \
    $$PWD/src/qtseedlinkinforequest.cpp         \
    $$PWD/src/qtseedlinkinforesponse.cpp        \
    $$PWD/src/qtseedlinkstreamxmlitemmodel.cpp  \
    $$PWD/src/qtseedlinkstreamxmlitem.cpp       \
    $$PWD/src/qtseedlinkerror.cpp               \

HEADERS +=                                      \
    $$PWD/src/qtseedlinkclient.h                \
    $$PWD/src/qtseedlinkstation.h               \
    $$PWD/src/qtseedlinkinforequest.h           \
    $$PWD/src/qtseedlinkinforesponse.h          \
    $$PWD/src/qtseedlinkstreamxmlitemmodel.h    \
    $$PWD/src/qtseedlinkstreamxmlitem.h         \
    $$PWD/src/qtseedlinkerror.h                 \

OTHER_FILES +=                                  \
    $$PWD/src/QtSeedLinkInfoResponse            \
    $$PWD/src/QtSeedLinkInfoRequest             \
    $$PWD/src/QtSeedLinkStation                 \
    $$PWD/src/QtSeedLinkClient                  \
    $$PWD/src/QtSeedLinkStreamXmlItem           \
    $$PWD/src/QtSeedLinkStreamXmlItemModel      \
    $$PWD/src/QtSeedLinkError                   \

RESOURCES +=                                    \
    $$PWD/rc/qtseedlinkclient.qrc               \

INCLUDEPATH +=                                  \
    $$PWD/src
