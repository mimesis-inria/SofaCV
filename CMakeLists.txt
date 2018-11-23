cmake_minimum_required(VERSION 2.8.12)
project(SofaCV)

set(${PROJECT_NAME}_MAJOR_VERSION 18)
set(${PROJECT_NAME}_MINOR_VERSION 12)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION})

find_package(OpenCV 3.2 REQUIRED)
find_package(SofaFramework REQUIRED)
find_package(SofaGui REQUIRED)
find_package(SofaQtQuickGUI QUIET)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

add_definitions("-std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(HEADER_FILES
  SofaCV.h
  src/SofaCVPlugin.h
  src/datatypes/cvKeypoint.h
  src/datatypes/cvDMatch.h
  src/datatypes/cvMat.h
  src/datatypes/cvMatUtils.h
  src/utils/FrameViewer.h
  src/utils/FrameExporter.h
  src/utils/PCViewer.h
  src/acquisition/BaseGrabber.h
  src/acquisition/BaseFrameGrabber.h
  src/acquisition/VideoGrabber.h
  src/ImplicitDataEngine.h
  src/ImageFilter.h
  )

set(SOURCE_FILES
  src/SofaCVPlugin.cpp
  src/datatypes/cvKeypoint.cpp
  src/datatypes/cvDMatch.cpp
  src/datatypes/cvMat.cpp
  src/datatypes/cvMatUtils.cpp
  src/utils/FrameViewer.cpp
  src/utils/FrameExporter.cpp
  src/utils/PCViewer.cpp
  src/acquisition/BaseGrabber.cpp
  src/acquisition/BaseFrameGrabber.cpp
  src/acquisition/VideoGrabber.cpp
  src/ImplicitDataEngine.cpp
  )

find_package(SofaPython QUIET)
if( SofaPython_FOUND )
  set(PYTHON_FILES
    src/python/SofaCV/__init__.py
    )

  list(APPEND SOURCE_FILES src/python/Binding_cvMatData.cpp)
  list(APPEND HEADER_FILES
    src/python/Binding_cvMatData.h)
  sofa_set_python_directory(${PROJECT_NAME} "src/python")
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

set (SOFACV_NEW_GUI_ENABLED OFF CACHE BOOL "Enables ImageFilter Widgets for runSofa2")
message (SofaQtQuickGUI_FOUND)
message(${SOFACV_NEW_GUI_ENABLED})
if(SofaQtQuickGUI_FOUND AND ${SOFACV_NEW_GUI_ENABLED})
    message ("WE FOUND THE NEW GUI! COOL!")
  find_package(Qt5QuickControls2 REQUIRED)
  find_package(Qt5Quick REQUIRED)
  find_package(Qt5Qml REQUIRED)
  find_package(GLEW REQUIRED)

  add_definitions("-DSOFACV_NEW_GUI_ENABLED")
  add_definitions("-DSOFA_BUILD_${PROJECT_NAME}_PLUGIN")
  add_definitions("-DHAS_GLEW")

  list(APPEND SOURCE_FILES
    src/gui/ImageFilterDisplay.cpp
    src/gui/ImageFilterModel.cpp
    src/ImageFilter_newGUI.cpp
    )

  list(APPEND HEADER_FILES
    src/gui/ImageFilterDisplay.h
    src/gui/ImageFilterModel.h
    src/ImageFilter_newGUI.h
    )

    set(MOC_HEADER_FILES
    src/gui/ImageFilterDisplay.h
    src/gui/ImageFilterModel.h
    )

  set(QML_FILES
    src/gui/qml/SofaDataTypes/SofaDataType_cvMat.qml
    src/gui/qml/SofaDataTypes/SofaDataType_cvDMatch.qml
    src/gui/qml/SofaDataTypes/SofaDataType_cvKeypoint.qml
    src/gui/qml/SofaWidgets/ImageFilterWidget.qml
    )

  set(QRC_FILES
    src/gui/qml/SofaCV_qml.qrc
    )

  list (APPEND ${PROJECT_NAME}_LIBRARIES
    SofaQtQuickGUI
    ${GLEW_LIBRARIES}
    )
  # do not use "set(CMAKE_AUTOMOC ON)" since all the mocced files
  # will be compiled by a single compilation unit leading to a
  # "out of heap space" issue on MSVC
  qt5_wrap_cpp(MOC_FILES ${MOC_HEADER_FILES})
  qt5_add_resources(RESOURCE_FILES ${QRC_FILES})

  list (APPEND ${PROJECT_NAME}_FILES
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${MOC_FILES}
    ${RESOURCE_FILES}
    ${QML_FILES}
    ${QRC_FILES}
    )
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQT_NO_DEBUG)
  endif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DQT_PLUGIN)
else ()
  list(APPEND HEADER_FILES
    src/DataSliderMgr.h
    src/DataSliderMgr.inl
    src/ImageFilter_oldGUI.h
    )

  list(APPEND SOURCE_FILES
    src/ImageFilter_oldGUI.cpp
    )
  set(${PROJECT_NAME}_FILES
    ${SOURCE_FILES}
    ${HEADER_FILES}
    )
endif (SofaQtQuickGUI_FOUND AND ${SOFACV_NEW_GUI_ENABLED})

set(${PROJECT_NAME}_RESOURCES_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/resources\")
configure_file("${PROJECT_NAME}.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.h")
configure_file("${PROJECT_NAME}.h.in" "${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}.h")
set(${PROJECT_NAME}_RESOURCES_PATH  \"${CMAKE_INSTALL_PREFIX}/share/sofa/plugins/${PROJECT_NAME}\")
configure_file("${PROJECT_NAME}.h.in" "${CMAKE_BINARY_DIR}/include/installed${PROJECT_NAME}.h")
install(FILES "${CMAKE_BINARY_DIR}/include/installed${PROJECT_NAME}.h" DESTINATION "include/${PROJECT_NAME}/" RENAME "${PROJECT_NAME}.h")

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_FILES} ${PYTHON_FILES})

target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBRARIES})

target_compile_options(${PROJECT_NAME} PUBLIC -D${PROJECT_NAME}_VERSION=\"${${PROJECT_NAME}_VERSION}\")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${${PROJECT_NAME}_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

if ( SofaPython_FOUND )
  target_link_libraries(${PROJECT_NAME} SofaPython)
  add_definitions("-DSOFA_HAVE_SOFAPYTHON")
endif( SofaPython_FOUND )

sofa_create_package(${PROJECT_NAME} ${${PROJECT_NAME}_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})

install(DIRECTORY resources/ DESTINATION share/sofa/plugins/${PROJECT_NAME})

if(SOFA_BUILD_TESTS)
    find_package(SofaTest QUIET)
    if(SofaTest_FOUND)
        add_subdirectory(SofaCV_test)
    endif(SofaTest_FOUND)
endif(SOFA_BUILD_TESTS)
