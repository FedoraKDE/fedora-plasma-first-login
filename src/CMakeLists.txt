include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(declarative)

set(FPFL_sources
    main.cpp
)

add_executable(fedora-plasma-first-login
    ${FPFL_sources}
)

target_link_libraries(fedora-plasma-first-login
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets # FIXME: QApplication
    Qt5::Quick
    KF5::I18n
    KF5::Plasma
    KF5::Service  # Plasma dependency
)

install(TARGETS fedora-plasma-first-login ${INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY qml/ DESTINATION ${DATA_INSTALL_DIR}/fedora-plasma-first-login/qml)
install(FILES fedora-plasma-first-login.notifyrc DESTINATION ${DATA_INSTALL_DIR}/fedora-plasma-first-login)
