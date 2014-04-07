set(declarative_SRCS
    firstloginplugin.cpp
    toolinvocation.cpp
    locales.cpp
)

add_library(firstloginplugin SHARED ${declarative_SRCS})
target_link_libraries(firstloginplugin
        Qt5::Qml
        KF5::Service # KToolInvocation
        KF5::KDE4Support # KLocale
        KF5::ConfigCore # Locales
)

install(TARGETS firstloginplugin DESTINATION ${QML_INSTALL_DIR}/org/fedoraproject/kde/FirstLogin)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/fedoraproject/kde/FirstLogin)
