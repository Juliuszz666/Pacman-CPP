cmake_minimum_required(VERSION 3.5)

project(Pacman VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
        Pages/mainwindow.cpp
        Pages/mainwindow.h
        Pages/mainwindow.ui

        resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Pacman
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        shared.h                    shared.cpp
        Entities/entity.h           Entities/enitity.cpp
        Entities/ghost.h            Entities/ghost.cpp
        Entities/pacman.h           Entities/pacman.cpp
        Entities/blinky.h           Entities/blinky.cpp
        Entities/clyde.h            Entities/clyde.cpp
        Entities/inky.h             Entities/inky.cpp
        Entities/pinky.h            Entities/pinky.cpp
        Pages/gameoverpage.h        Pages/gameoverpage.cpp          Pages/gameoverpage.ui
        Pages/gamepage.h            Pages/gamepage.cpp              Pages/gamepage.ui
        Pages/mainwindow.h          Pages/mainwindow.cpp            Pages/mainwindow.ui
        Pages/settingspage.h        Pages/settingspage.cpp          Pages/settingspage.ui
        MapElements/collectable.h   MapElements/collectable.cpp
        MapElements/tile.h          MapElements/tile.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Pacman APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Pacman SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Pacman
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Pacman PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Pacman PRIVATE Qt6::Multimedia)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Pacman)
endif()
set_target_properties(Pacman PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Pacman
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Pacman)
endif()
