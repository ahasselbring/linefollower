cmake_minimum_required(VERSION 3.0)
project(line)

set(SOURCES
  main.c
  module/behavior_control.c
  module/line_perception.c
  module/modeling.c
  module/motor_control.c
)

set(HEADERS
  framework.h
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
  sim/simulator.c
)

set(SIM_HEADERS
  sim/simulator.h
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=c11")
if(SIM)
  add_definitions(-DTARGET_SIMULATOR)
elseif(ROBOT)
  add_definitions(-DTARGET_ROBOT)
else()
  message(FATAL_ERROR "You need to define a target.")
endif()

add_executable(line ${SOURCES} ${HEADERS} ${SIM_SOURCES} ${SIM_HEADERS})
target_include_directories(line PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(line m)
