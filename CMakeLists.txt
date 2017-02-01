cmake_minimum_required(VERSION 2.8.12)
project(SofaORCommon)

set(SOFAORCOMMON_MAJOR_VERSION 0)
set(SOFAORCOMMON_MINOR_VERSION 1)
set(SOFAORCOMMON_VERSION ${SOFAORCOMMON_MAJOR_VERSION}.${SOFAORCOMMON_MINOR_VERSION})


set(HEADER_FILES
  initplugin.h
  cvKeypoint.h
  cvDMatch.h
  cvMat.h
  cvMatUtils.h
  FrameViewer.h
  #StereoViewer.h
  ImplicitDataEngine.h)

set(SOURCE_FILES
  initplugin.cpp
  cvKeypoint.cpp
  cvDMatch.cpp
  cvMat.cpp
  cvMatUtils.cpp
  FrameViewer.cpp
  #StereoViewer.cpp
  ImplicitDataEngine.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(SofaFramework REQUIRED)
find_package(SofaGui REQUIRED)

if (SOFAOR_ENABLE_IGTLINK)
  find_package(OpenIGTLink REQUIRED)
  include_directories(${OpenIGTLink_INCLUDE_DIRS})
endif ()


add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC SofaCore SofaSimulationCommon SofaGuiQt SofaGuiCommon ${OpenIGTLink_LIBRARIES} ${OpenCV_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${SOFAORCOMMON_VERSION})

#set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

add_definitions("-DSOFA_BUILD_SOFAORCOMMON")

## Install rules for the library; CMake package configurations files
sofa_create_package(${PROJECT_NAME} ${SOFAORCOMMON_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" DESTINATION "include/SofaORCommon"
  FILES_MATCHING PATTERN "*.h" PATTERN "*.inl")
