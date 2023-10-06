cmake_minimum_required(VERSION 3.16)

project(aus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Network NetworkAuth)

qt_standard_project_setup()

#qt_add_resources(RESOURCES
#    resources.qrc
#)

qt_add_executable(appaus
    main.cpp
    spotifyapi.h
    spotifyapi.cpp
    imageprocessor.h
    imageprocessor.cpp
    #    ${RESOURCES}
)

qt_add_qml_module(appaus
    URI aus
    VERSION 1.0
    QML_FILES Main.qml
)

# Platform-specific settings
if(APPLE)
    set_target_properties(appaus PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings can go here
endif()

target_link_libraries(appaus
    PRIVATE Qt6::Quick Qt6::Network Qt6::NetworkAuth
)

install(TARGETS appaus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

