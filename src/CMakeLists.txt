include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(declarative)

set(FPFL_sources
    main.cpp
#    frame.cpp
#    hoverbutton.cpp
#    sidewidget.cpp
#    sidewidgetpagelabel.cpp
#    applet.cpp
#    page.cpp
)

set(FPFL_pages
#    pages/welcomepage.cpp
#    pages/languagepage.cpp
#    pages/regionpage.cpp
#    pages/colorthemepage.cpp
#    pages/userpage.cpp
#    pages/networkpage.cpp
)

set(FPFL_dialogs
#    dialogs/chfacedlg.cpp
)

#qt5_wrap_ui(FPFL_dialogs
#    dialogs/faceDlg.ui
#)

add_executable(fedora-plasma-first-login
                    ${FPFL_sources}
                    ${FPFL_pages}
                    ${FPFL_dialogs}
)

target_link_libraries(fedora-plasma-first-login
                      Qt5::Core
                      Qt5::Gui
                      Qt5::Widgets # FIXME: QApplication
                      Qt5::Quick
                      KF5::I18n
                      KF5::Plasma
                      KF5::Service
                      KF5::KDE4Support
                      #${PACKAGEKIT_QT2_LIBRARIES}      #FIXME: Needs Qt 5 version
)

install(TARGETS fedora-plasma-first-login ${INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY qml/ DESTINATION ${DATA_INSTALL_DIR}/fedora-plasma-first-login/qml)
install(FILES fedora-plasma-first-login.notifyrc DESTINATION ${DATA_INSTALL_DIR}/fedora-plasma-first-login)
