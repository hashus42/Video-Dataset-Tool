cmake_minimum_required(VERSION 3.16.0)

project(VideoDatasetTool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Create executable
add_executable(VideoDatasetTool
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Link Qt libraries
target_link_libraries(VideoDatasetTool
    Qt6::Core
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# Include directories
target_include_directories(VideoDatasetTool PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

# Qt MOC handling
set_target_properties(VideoDatasetTool PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Windows specific settings
if(WIN32)
    set_target_properties(VideoDatasetTool PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

# macOS specific settings
if(APPLE)
    set_target_properties(VideoDatasetTool PROPERTIES
        MACOSX_BUNDLE ON
    )
endif()
