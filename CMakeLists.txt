cmake_minimum_required(VERSION 3.5)

project(wifi_basic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets )

file(GLOB_RECURSE PROJECT_HEADERS "include/*.h")
file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")

set(PROJECT_FILES
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    main.cpp
)

include_directories(
    include
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(wifi_basic
        MANUAL_FINALIZATION
        ${PROJECT_FILES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET wifi_basic APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(wifi_basic SHARED
            ${PROJECT_FILES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(wifi_basic
            ${PROJECT_FILES}
            img/sys_status.png img/sys_status.svg img/wifi.png img/wifi.svg
        )
    endif()
endif()

target_link_libraries(wifi_basic PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.wifi_basic)
endif()
set_target_properties(wifi_basic PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS wifi_basic
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(wifi_basic)
endif()
