cmake_minimum_required(VERSION 2.8.12)
project(SofaORCommon)

set(SOFAORCOMMON_MAJOR_VERSION 0)
set(SOFAORCOMMON_MINOR_VERSION 1)
set(SOFAORCOMMON_VERSION ${SOFAORCOMMON_MAJOR_VERSION}.${SOFAORCOMMON_MINOR_VERSION})

find_package(SofaFramework REQUIRED)
find_package(SofaGui REQUIRED)
find_package(SofaQtQuickGUI QUIET)

include_directories( . )

option(SOFAOR_COMMUNICATIONCONTROLLER "Enables Communication component, requires to install ZMQ library" OFF)

set(HEADER_FILES
  initplugin.h
  cvKeypoint.h
  cvDMatch.h
  cvMat.h
  cvMatUtils.h
  ImplicitDataEngine.h
  )

set(SOURCE_FILES
  initplugin.cpp
  cvKeypoint.cpp
  cvDMatch.cpp
  cvMat.cpp
  cvMatUtils.cpp
  ImplicitDataEngine.cpp
)

#COMMUNICATION CONTROLLER
if(SOFAOR_COMMUNICATIONCONTROLLER)

    list(APPEND HEADER_FILES
        controller/CommunicationController.h
        controller/CommunicationController.inl
            )

    list(APPEND SOURCE_FILES
        controller/CommunicationController.cpp
            )

endif()

set(SofaORCommon_FILES
    ${HEADER_FILES}
    ${SOURCE_FILES}
    )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SofaORCommon_LIBRARIES
    SofaCore
    SofaSimulationCommon
    SofaGuiQt
    SofaGuiCommon
    ${OpenCV_LIBRARIES}
    )

if(SofaQtQuickGUI_FOUND)
    find_package(Qt5QuickControls2 REQUIRED)
    find_package(Qt5Quick REQUIRED)
    find_package(Qt5Qml REQUIRED)

    add_definitions("-DSOFAOR_NEW_GUI_ENABLED")
    add_definitions("-DSOFA_BUILD_SOFAORCOMMON_PLUGIN")

    set(QML_FILES
        gui/qml/SofaDataTypes/SofaDataType_cvMat.qml
        gui/qml/SofaDataTypes/SofaDataType_cvDMatch.qml
        gui/qml/SofaDataTypes/SofaDataType_cvKeypoint.qml
        )

    set(QRC_FILES
        gui/qml/SofaORCommon_qml.qrc
        )

    list (APPEND SofaORCommon_LIBRARIES
        SofaQtQuickGUI
        )
    # do not use "set(CMAKE_AUTOMOC ON)" since all the mocced files
    # will be compiled by a single compilation unit leading to a
    # "out of heap space" issue on MSVC
    qt5_wrap_cpp(MOC_FILES ${MOC_HEADER_FILES})
    qt5_add_resources(RESOURCE_FILES ${QRC_FILES})

    list (APPEND SofaORCommon_FILES
        ${MOC_FILES}
        ${RESOURCE_FILES}
        ${QML_FILES}
        ${QRC_FILES}
        )
    # to fix a bug when one is compiling a debug version of the code
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-DQT_NO_DEBUG)
    endif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQT_PLUGIN)
endif (SofaQtQuickGUI_FOUND)

add_library(${PROJECT_NAME} SHARED ${SofaORCommon_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SofaORCommon_LIBRARIES})

#COMMUNICATION CONTROLLER
if(SOFAOR_COMMUNICATIONCONTROLLER)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${SofaORCommon_LIBRARIES} "-lzmq")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${SOFAORCOMMON_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

sofa_create_package(${PROJECT_NAME} ${SOFAORCOMMON_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION "include/SofaORCommon"
    FILES_MATCHING PATTERN "*.h" PATTERN "*.inl")
