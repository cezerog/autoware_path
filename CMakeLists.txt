cmake_minimum_required(VERSION 3.5)
project(autoware_path)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)


add_executable(path src/autoware_path_finder_controller.cpp)
ament_target_dependencies(
  path 
  ament_cmake 
  rclcpp 
  std_msgs 
  nav_msgs 
  geometry_msgs
  tf2
  tf2_ros
  )

install(TARGETS
  path

  DESTINATION lib/${PROJECT_NAME}
)

ament_package()