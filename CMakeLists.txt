cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(LoaderCompanionLoader)

include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Loader Companion Loader")

set(VITA_TITLEID "LOADCOMP0")

set(VITA_VERSION "01.00")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -g -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-psabi -Wunused-variable -Wwrite-strings -fpermissive -std=c++17 -Wimplicit-fallthrough")
set(VITA_MAKE_FSELF_FLAGS "${VITA_MAKE_FSELF_FLAGS} -a 0x2808000000000000")

include_directories()

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  /usr/local/vitasdk/arm-vita-eabi/include/common/
)


add_executable(${PROJECT_NAME}
  src/main.cpp
  /usr/local/vitasdk/arm-vita-eabi/include/common/debugScreen.c
)


target_link_libraries(${PROJECT_NAME}
  SceLibKernel_stub 
  SceDisplay_stub
  taihen_stub
  SceVshBridge_stub
  SceShellSvc_stub 
  SceAppMgr_stub
  taihenForKernel_stub
  SceIofilemgr_stub 
  SceSysmodule_stub
  
  LCKernelExports_stub
  SceCtrl_stub
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})

vita_create_vpk("${PROJECT_NAME}.vpk" "${VITA_TITLEID}" "${PROJECT_NAME}.self"
  VERSION "${VITA_VERSION}"
  NAME "${VITA_APP_NAME}"
  FILE sce_sys sce_sys
  FILE sce_module sce_module
)
