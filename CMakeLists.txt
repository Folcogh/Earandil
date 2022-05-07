cmake_minimum_required(VERSION 3.5)

project(Earandil VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        Dialog/AddDocumentation.hpp
        Dialog/AddDocumentation.cpp
        Dialog/AddDocumentation.ui
        Dialog/EditDocPath.hpp
        Dialog/EditDocPath.cpp
        Dialog/EditDocPath.ui
        Dialog/EditDocProperty.hpp
        Dialog/EditDocProperty.cpp
        Dialog/EditDocProperty.ui
        Dialog/SetDBFilename.hpp
        Dialog/SetDBFilename.cpp
        Dialog/SetDBFilename.ui
        DocProperty.hpp
        DocProperty.cpp
        Languages.hpp
        Languages.cpp
        main.cpp
        MainWindow.hpp
        MainWindow.cpp
        MainWindow.ui
        Settings.hpp
        Settings.cpp
        Validator/ValidatorSN_full.hpp
        Validator/ValidatorSN_full.cpp
        Validator/ValidatorSN_suffix.hpp
        Validator/ValidatorSN_suffix.cpp
        Validator/ValidatorDevStep.hpp
        Validator/ValidatorDevStep.cpp
        WorkingDB.hpp
        WorkingDB.cpp
        ADBformat.txt
        BUGS.txt
        SPECS.txt
        TODO.txt
        Global.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Earandil
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Earandil APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Earandil SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Earandil
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Earandil PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Earandil PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Earandil)
endif()
