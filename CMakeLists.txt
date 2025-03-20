cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(https_checks_disable)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

if (NOT ${RELEASE})
  add_definitions(-DENABLE_LOGGING)
endif()


if(NO_PRINTS)
  add_compile_definitions(NO_PRINTS=1)
endif()

add_executable(https_checks_disable
  https_checks_disable.c
)

target_link_libraries(https_checks_disable
  taihen_stub
  SceLibKernel_stub
  kubridge_stub
  SceSysmem_stub
  SceNetPs_stub
SceProcessmgr_stub
SceKernelThreadMgr_stub
SceLibc_stub
SceSysclibForDriver_stub
SceHttp_stub_weak 
SceSysmodule_stub
)

set_target_properties(https_checks_disable
  PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(https_checks_disable.suprx https_checks_disable
  CONFIG ${CMAKE_SOURCE_DIR}/https_checks_disable.yml
)
