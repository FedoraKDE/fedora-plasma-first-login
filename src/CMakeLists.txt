include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(FPFL_sources
    frame.cpp
    main.cpp
    hoverbutton.cpp
    sidewidget.cpp
    sidewidgetpagelabel.cpp
    wizard.cpp
    applet.cpp
    page.cpp
    view.cpp
)

set(FPFL_pages
    pages/welcomepage.cpp
    pages/languagepage.cpp
    pages/regionpage.cpp
    pages/colorthemepage.cpp
    pages/userpage.cpp
)

set(FPFL_dialogs
    dialogs/chfacedlg.cpp
)

kde4_add_ui_files(FPFL_dialogs
    dialogs/faceDlg.ui
)

kde4_add_executable(fedora-plasma-first-login
                    ${FPFL_sources}
                    ${FPFL_pages}
                    ${FPFL_dialogs}
)

target_link_libraries(fedora-plasma-first-login
                    ${QT_QTCORE_LIBRARY}
                    ${QT_QTGUI_LIBRARY}
                    ${KDE4_KDECORE_LIBRARY}
                    ${KDE4_KIO_LIBRARY}
                    ${KDE4_PLASMA_LIBRARY}
                    ${PACKAGEKIT_QT2_LIBRARIES}
)

install(TARGETS fedora-plasma-first-login ${INSTALL_TARGETS_DEFAULT_ARGS})
