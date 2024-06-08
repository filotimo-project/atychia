# SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

project(atychia VERSION 0.1)

include(FeatureSummary)

set(QT6_MIN_VERSION 6.5.0)
set(KF6_MIN_VERSION 6.0.0)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMFindQmlModule)
include(KDEGitCommitHooks)
include(KDEClangFormat)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX ATYCHIA
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/version-atychia.h"
)

set(QT_EXTRA_COMPONENTS)
list(APPEND QT_EXTRA_COMPONENTS Widgets)

find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS Core Gui Qml QuickControls2 Svg DBus ${QT_EXTRA_COMPONENTS})
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS Kirigami2 CoreAddons I18n)

qt_policy(SET QTP0001 NEW)

ecm_find_qmlmodule(org.kde.kirigamiaddons.formcard 1.0)

add_subdirectory(src)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/directories.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/directories.h @ONLY
)

# Shell scripts
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/atychia-launch.in
    ${CMAKE_CURRENT_SOURCE_DIR}/atychia-launch @ONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/atychia-restart-plasmashell.in
    ${CMAKE_CURRENT_SOURCE_DIR}/atychia-restart-plasmashell @ONLY
)

install(FILES org.filotimo.atychia.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.filotimo.atychia-restart-plasmashell.desktop DESTINATION ${KDE_INSTALL_APPDIR})
# Register shortcuts with KGlobalAccel
file(CREATE_LINK ${KDE_INSTALL_FULL_APPDIR}/filotimo.atychia.desktop ${KDE_INSTALL_FULL_DATAROOTDIR}/kglobalaccel/filotimo.atychia.desktop SYMBOLIC)
file(CREATE_LINK ${KDE_INSTALL_FULL_APPDIR}/filotimo.atychia-restart-plasmashell.desktop ${KDE_INSTALL_FULL_DATAROOTDIR}/kglobalaccel/filotimo.atychia-restart-plasmashell.desktop SYMBOLIC)

install(FILES org.filotimo.atychia-launch.policy DESTINATION ${KDE_INSTALL_FULL_DATAROOTDIR}/polkit-1/actions)
install(FILES 99-filotimo-atychia.rules DESTINATION ${KDE_INSTALL_FULL_SYSCONFDIR}/polkit-1/rules.d)

install(PROGRAMS atychia-launch DESTINATION ${KDE_INSTALL_FULL_BINDIR})
install(PROGRAMS atychia-restart-plasmashell DESTINATION ${KDE_INSTALL_FULL_LIBEXECDIR})
install(PROGRAMS atychia-interactive-su DESTINATION ${KDE_INSTALL_FULL_LIBEXECDIR})

ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
