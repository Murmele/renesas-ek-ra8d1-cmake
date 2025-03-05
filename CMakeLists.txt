cmake_minimum_required(VERSION 3.15)
project(ra8d1_slint C CXX ASM)

# Configure Slint
set(SLINT_FEATURE_RENDERER_SOFTWARE ON)
set(SLINT_FEATURE_SDF_FONTS ON)
set(SLINT_FEATURE_FREESTANDING ON)
set(DEFAULT_SLINT_EMBED_RESOURCES "embed-for-software-renderer" CACHE STRING "")
set(Rust_CARGO_TARGET thumbv8m.main-none-eabihf)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(BUILD_SHARED_LIBS OFF)
include(FetchContent)
FetchContent_Declare(
    Slint
    GIT_REPOSITORY https://github.com/slint-ui/slint.git
    GIT_TAG master
)
FetchContent_MakeAvailable(Slint)


# Set compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Compiler flags
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/script/fsp.lld)
set(CPU_FLAGS "-mcpu=cortex-m85 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16")
set(COMMON_FLAGS "-fdata-sections -ffunction-sections -fno-common ${CPU_FLAGS}")
set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -T${LINKER_SCRIPT} -L${CMAKE_CURRENT_SOURCE_DIR} -L${CMAKE_CURRENT_SOURCE_DIR}/script ${CPU_FLAGS}")
#set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_BUILD_TYPE RelWithDebInfo)


# Include FSP directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/inc/api
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/inc/instances
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/src/rm_freertos_port
    ${CMAKE_CURRENT_SOURCE_DIR}/ra_cfg/fsp_cfg/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/tes/dave2d/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/arm/CMSIS_6/CMSIS/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/aws/FreeRTOS/FreeRTOS/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ra_cfg/aws
    ${CMAKE_CURRENT_SOURCE_DIR}/ra_cfg/fsp_cfg
    ${CMAKE_CURRENT_SOURCE_DIR}/ra_gen

    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add FSP source files
file(GLOB FSP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/src/*/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/src/bsp/mcu/all/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ra_gen/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ra/aws/FreeRTOS/FreeRTOS/Source/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/ra/board/ra8d1_ek/*.c"

    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Create executable
add_executable(${PROJECT_NAME}.elf
    ${FSP_SOURCES}

    slint-ra-fsp.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME}.elf PRIVATE Slint::Slint)
slint_target_sources(${PROJECT_NAME}.elf app-window.slint )
set_target_properties(${PROJECT_NAME}.elf PROPERTIES SLINT_EMBED_RESOURCES "embed-for-software-renderer-with-sdf")
