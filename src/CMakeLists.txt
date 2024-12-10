# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>

# Target: static library
qt_add_qml_module(atychia_static
    STATIC
    URI org.filotimoproject.atychia
    VERSION 1.0
    QML_FILES
        contents/ui/Main.qml
        contents/ui/About.qml
)

target_sources(atychia_static PUBLIC
    Actions.cpp
)

target_link_libraries(atychia_static PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::DBus
    KF6::I18n
    KF6::CoreAddons
    KF6::ColorScheme
)
target_include_directories(atychia_static PUBLIC ${CMAKE_BINARY_DIR})

target_link_libraries(atychia_static PUBLIC Qt::Widgets)

# Target: main executable
add_executable(atychia main.cpp)
target_link_libraries(atychia PUBLIC atychia_static atychia_staticplugin)
install(TARGETS atychia ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
