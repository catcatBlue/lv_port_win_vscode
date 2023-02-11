cmake_minimum_required(VERSION 3.0.0)

project(lvgl_simulator VERSION 0.1.0)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 11)

# include(CTest)
# enable_testing()

# Includes and libraries
set(SDL2_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2/SDL2-2.0.10)

find_package(SDL2 REQUIRED)
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/lib/
    ${PROJECT_SOURCE_DIR}/lib/config/
    ${PROJECT_SOURCE_DIR}/lib/lv_drivers/
    ${PROJECT_SOURCE_DIR}/lib/lvgl/
    ${PROJECT_SOURCE_DIR}/lib/ui/
)

# subdirectories
add_subdirectory(lib/lv_drivers)
add_subdirectory(lib/lvgl)

FILE(GLOB_RECURSE LV_DRIVERS_Sources CONFIGURE_DEPENDS lib/lv_drivers/*.c)
FILE(GLOB_RECURSE LVGL_Sources CONFIGURE_DEPENDS lib/lvgl/*.c)
FILE(GLOB_RECURSE UI_Sources CONFIGURE_DEPENDS lib/ui/*.c)
FILE(GLOB_RECURSE MAIN_Sources CONFIGURE_DEPENDS main/src/*.c)

add_executable(
    ${PROJECT_NAME} 
    ${MAIN_Sources} 
    ${UI_Sources} 
    ${LVGL_Sources} 
    ${LV_DRIVERS_Sources} 
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC lvgl_demos lvgl_examples lv_drivers lvgl
    PRIVATE ${SDL2_LIBRARIES}
) #注意链接顺序，前面依赖后面

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
