set(declarative_SRCS
    firstloginplugin.cpp
)

add_library(firstloginplugin SHARED ${declarative_SRCS})
target_link_libraries(firstloginplugin
        Qt5::Qml
)

install(TARGETS firstloginplugin DESTINATION ${QML_INSTALL_DIR}/org/fedoraproject/kde/FirstLogin)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/fedoraproject/kde/FirstLogin)
