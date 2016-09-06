cmake_minimum_required(VERSION 3.1)
project(line)

set(SOURCES
  controller.c
  module/behavior_control.c
  module/line_perception.c
  module/modeling.c
  module/motor_control.c
)

set(HEADERS
  framework.h
  controller.h
  data/control.h
  data/light.h
  data/line.h
  data/request.h
  data/state.h
  module/behavior_control.h
  module/line_perception.h
  module/modeling.h
  module/motor_control.h
)

set(SIM_SOURCES
  sim/DebugView.cpp
  sim/main.cpp
  sim/MainWindow.cpp
  sim/PropertyView.cpp
  sim/SceneView.cpp
  sim/SimulationThread.cpp
  sim/Simulator.cpp
)

set(SIM_HEADERS
  sim/DebugView.hpp
  sim/MainWindow.hpp
  sim/PropertyView.hpp
  sim/SceneView.hpp
  sim/SimulationThread.hpp
  sim/Simulator.hpp
  sim/SimulatorCycleBundle.hpp
  sim/SimulatorLoadBundle.hpp
  sim/Types.hpp
)

set(SIM_RESOURCES
  sim/Resources.qrc
)

set(ROBOT_SOURCES
  robot/asuro.c
  robot/main.c
)

set(ROBOT_HEADERS
  robot/asuro.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(SIM)
  add_definitions(-DTARGET_SIMULATOR)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  find_package(Qt5Widgets REQUIRED)

  add_executable(line ${SOURCES} ${HEADERS} ${SIM_SOURCES} ${SIM_HEADERS} ${SIM_RESOURCES})
  target_link_libraries(line m)
  target_link_libraries(line Qt5::Widgets)
elseif(ROBOT)
  add_definitions(-DTARGET_ROBOT)
  add_executable(line ${SOURCES} ${HEADERS} ${ROBOT_SOURCES} ${ROBOT_HEADERS})
else()
  message(FATAL_ERROR "You need to define a target.")
endif()
