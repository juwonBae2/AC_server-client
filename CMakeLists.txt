# CMake버전 3.26.4
cmake_minimum_required(VERSION 3.26.4)

# 프로젝트 이름과 몇가지 유용한 설정
project(server-client VERSION 1.0.0
                   LANGUAGES C CXX)

# 빌드 속도 향상
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  message(STATUS "CCache Found")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
else()
  message(STATUS "CCache Not Found")
endif()

# 별도 build 폴더가 아닌 경우 cmake 할 수 없도록 방지
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
  message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory.")
endif()

message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 1.0.0)
  set(CMAKE_CXX_COMPILER_VERSION "5.3.1")
  message(STATUS "Detected alternative compiler. Fixing version to ${CMAKE_CXX_COMPILER_VERSION}, which is minimum requirement")
endif()

# 컴파일 옵션 등, 의존성에도 적용될 수 있는 설정만 할 것
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # 이곳에서 메인 프로젝트일때 한정 옵션 변수 설정
  set(CMAKE_CXX_STANDARD 14 CACHE STRING "The C++ standard to use")
  set(CMAKE_C_STANDARD 11 CACHE STRING "The C standard to use")
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF) # -std=c++xx를 사용 (-std=g++xx 대신)

  # 기본 빌드 타입 설정 (설정되어 있지 않은 경우 Debug로 빌드)
  set(default_build_type "Debug")
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
      "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
  endif()

  if(${CMAKE_BUILD_TYPE} MATCHES "[Dd]ebug")
    ADD_DEFINITIONS(-DDEBUG)
  endif()

  message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
  message(STATUS "CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")

  # IDE에서 folder 지원
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()


# clang으로 분석 (TODO)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # clang-tidy를 통해 정적 분석 수행
  option(CLANG_TIDY_FIX "Perform fixes for Clang-Tidy" OFF)
  find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
  )
  find_program( # clang이 사용되는 것은 아니나, clang-tidy의 올바른 동작을 위해 필요함
    CLANG_EXE
    NAMES "clang"
    DOC "Path to clang executable"
  )

  # 정적 분석 준비
  if((NOT CLANG_TIDY_EXE) OR (NOT CLANG_EXE))
    message(FATAL_ERROR "clang-tidy tool not found")
  endif()
endif()

# 의존성 가져오기 #
include(FetchContent)

# SFML
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        2.5.1
)
FetchContent_GetProperties(sfml)

if(NOT sfml_POPULATED)
  FetchContent_Populate(sfml)
  add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.8.5
)
FetchContent_GetProperties(spdlog)

if(NOT spdlog_POPULATED)
  FetchContent_Populate(spdlog)
  set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Enable spdlog tests" FORCE)
  set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "Enable spdlog bench" FORCE)
  set(SPDLOG_BUILD_SHARED ON CACHE BOOL "Force spdlog to shared" FORCE)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# gtest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  set(gtest_build_tests OFF CACHE BOOL "Enable gtest tests" FORCE)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# json
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.10.4)
FetchContent_GetProperties(json)

if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# 의존성 가져오기 끝 #

# 개별 디렉토리가 아닌 최상위 디렉토리에 빌드 결과물을 모은다
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


function(add_subdirectory_with_static_analysis TARGET_NAME)
  add_subdirectory(${TARGET_NAME})
endfunction()

# add_subdirectorys
add_subdirectory_with_static_analysis(consoleStyle)
add_subdirectory_with_static_analysis(client)
add_subdirectory_with_static_analysis(server)
add_subdirectory_with_static_analysis(chatHandler)