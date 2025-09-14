# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2025 Thomas Duckworth <tduck@filotimoproject.org>

add_executable(atychiad main.cpp)

target_include_directories(atychiad PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(atychiad
    PRIVATE
    Qt6::Core
    Qt6::DBus
)

install(TARGETS atychiad DESTINATION ${KDE_INSTALL_BINDIR})

configure_file(atychiad.service.in atychiad.service @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/atychiad.service DESTINATION ${KDE_INSTALL_SYSTEMDUSERUNITDIR})
