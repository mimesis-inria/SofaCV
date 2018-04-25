cmake_minimum_required(VERSION 2.8.12)
project(SofaCV)

set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION})

find_package(SofaFramework REQUIRED)
find_package(SofaGui REQUIRED)
find_package(SofaQtQuickGUI QUIET)

include_directories( src/ )

set(HEADER_FILES
  src/initplugin.h
  src/cvKeypoint.h
  src/cvDMatch.h
  src/cvMat.h
  src/cvMatUtils.h
  src/ImplicitDataEngine.h
  src/DependencyGraphVisitor.h
  src/DependencyGraphAnimationLoop.h
  )

set(SOURCE_FILES
  src/initplugin.cpp
  src/cvKeypoint.cpp
  src/cvDMatch.cpp
  src/cvMat.cpp
  src/cvMatUtils.cpp
  src/ImplicitDataEngine.cpp
  src/DependencyGraphVisitor.cpp
  src/DependencyGraphAnimationLoop.cpp
  )

find_package(SofaPython QUIET)
if( SofaPython_FOUND )
  set(PYTHON_FILES
    src/python/SofaOR/__init__.py
    )

  list(APPEND SOURCE_FILES python/Binding_cvMatData.cpp)
  list(APPEND HEADER_FILES
    src/python/Binding_cvMatData.h)
  sofa_set_python_directory(${PROJECT_NAME} "python")
endif()


set(${PROJECT_NAME}_FILES
  ${HEADER_FILES}
  ${SOURCE_FILES}
  )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(${PROJECT_NAME}_LIBRARIES
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

  add_definitions("-D${PROJECT_NAME}_NEW_GUI_ENABLED")
  add_definitions("-DSOFA_BUILD_${PROJECT_NAME}_PLUGIN")

  set(QML_FILES
    src/gui/qml/SofaDataTypes/SofaDataType_cvMat.qml
    src/gui/qml/SofaDataTypes/SofaDataType_cvDMatch.qml
    src/gui/qml/SofaDataTypes/SofaDataType_cvKeypoint.qml
    )

  set(QRC_FILES
    src/gui/qml/SofaCV_qml.qrc
    )

  list (APPEND ${PROJECT_NAME}_LIBRARIES
    SofaQtQuickGUI
    )
  # do not use "set(CMAKE_AUTOMOC ON)" since all the mocced files
  # will be compiled by a single compilation unit leading to a
  # "out of heap space" issue on MSVC
  qt5_wrap_cpp(MOC_FILES ${MOC_HEADER_FILES})
  qt5_add_resources(RESOURCE_FILES ${QRC_FILES})

  list (APPEND ${PROJECT_NAME}_FILES
    ${MOC_FILES}
    ${RESOURCE_FILES}
    ${QML_FILES}
    ${QRC_FILES}
    )
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQT_NO_DEBUG)
  endif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DQT_PLUGIN)
endif (SofaQtQuickGUI_FOUND)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_FILES} ${PYTHON_FILES})

target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

if ( SofaPython_FOUND )
  target_link_libraries(${PROJECT_NAME} SofaPython)
  add_definitions("-DSOFA_HAVE_SOFAPYTHON")
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "${${PROJECT_NAME}_COMPILER_FLAGS}")

sofa_create_package(${PROJECT_NAME} ${${PROJECT_NAME}_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION "include/${PROJECT_NAME}"
  FILES_MATCHING PATTERN "*.h" PATTERN "*.inl")
