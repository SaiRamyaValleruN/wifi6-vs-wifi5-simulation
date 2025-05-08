# ##############################################################################
# Required CMake version
# ##############################################################################
cmake_minimum_required(VERSION 3.13..3.13)

# ##############################################################################
# Project name
# ##############################################################################
project(NS3 CXX C)

file(STRINGS VERSION NS3_VER)

# minimum compiler versions
set(AppleClang_MinVersion 13.1.6)
set(Clang_MinVersion 11.0.0)
set(GNU_MinVersion 10.1.0)

# Common build options
option(NS3_ASSERT "Enable assert on failure" OFF)
option(NS3_DES_METRICS "Enable DES Metrics event collection" OFF)
option(NS3_EXAMPLES "Enable examples to be built" OFF)
option(NS3_LOG "Enable logging to be built" OFF)
option(NS3_TESTS "Enable tests to be built" OFF)
option(NS3_EMU "Build with emulation support" ON)
option(NS3_TAP "Build with Tap support" ON)
option(NS3_DPDK "Enable fd-net-device DPDK features" OFF)
option(NS3_CLANG_FORMAT "Enforce code style with clang-format" OFF)
option(NS3_CLANG_TIDY "Use clang-tidy static analysis" OFF)
option(NS3_COLORED_OUTPUT "Colorize CMake messages" OFF)
option(NS3_COVERAGE "Enable code coverage measurements and report generation" OFF)
option(NS3_COVERAGE_ZERO_COUNTERS "Zero lcov counters before running. Requires NS3_COVERAGE=ON" OFF)
option(NS3_INCLUDE_WHAT_YOU_USE "Use IWYU to determine unnecessary headers included" OFF)
option(NS3_LINK_WHAT_YOU_USE "Use LWYU to determine unnecessary linked libraries" OFF)
option(NS3_SANITIZE "Build with address, leak and undefined sanitizers" OFF)
option(NS3_SANITIZE_MEMORY "Build with memory sanitizer" OFF)
option(NS3_NETANIM "Build netanim" OFF)
option(NS3_ENABLE_BUILD_VERSION "Embed version info into libraries" OFF)
option(NS3_CCACHE "Use Ccache to speed up recompilation" ON)
option(NS3_CPM "Enable the CPM C++ library manager support" OFF)
option(NS3_FAST_LINKERS "Use Mold or LLD to speed up linking if available" ON)
option(NS3_FETCH_OPTIONAL_COMPONENTS "Fetch Brite, Click and Openflow dependencies" OFF)
option(NS3_FORCE_LOCAL_DEPENDENCIES "Only search applications, headers, libraries on explicitly set directories" OFF)
option(NS3_GSL "Build with GSL support" ON)
option(NS3_GTK3 "Build with GTK3 support" ON)
option(NS3_LINK_TIME_OPTIMIZATION "Build with link-time optimization" OFF)
option(NS3_MONOLIB "Build a single shared ns-3 library and link it against executables" OFF)
option(NS3_MPI "Build with MPI support" OFF)
option(NS3_NATIVE_OPTIMIZATIONS "Build with -march=native -mtune=native" OFF)
option(NS3_NINJA_TRACING "Use ninjatracing to convert Ninja's build log to the about://tracing format" OFF)
set(NS3_OUTPUT_DIRECTORY "" CACHE STRING "Directory to store built artifacts")
option(NS3_PRECOMPILE_HEADERS "Precompile module headers to speed up compilation" ON)
option(NS3_PYTHON_BINDINGS "Build ns-3 python bindings" OFF)
option(NS3_SQLITE "Build with SQLite support" ON)
option(NS3_EIGEN "Build with Eigen support" ON)
option(NS3_STATIC "Build a static ns-3 library and link it against executables" OFF)
option(NS3_VCPKG "Enable the Vcpkg C++ library manager support" OFF)
option(NS3_VERBOSE "Print additional build system messages" OFF)
option(NS3_VISUALIZER "Build visualizer module" ON)
option(NS3_WARNINGS "Enable compiler warnings" ON)
option(NS3_WARNINGS_AS_ERRORS "Treat warnings as errors. Requires NS3_WARNINGS=ON" ON)

# Module filtering
set(NS3_ENABLED_MODULES "" CACHE STRING "List of modules to enable (e.g. core;network;internet)")
set(NS3_DISABLED_MODULES "" CACHE STRING "List of modules to disable (e.g. lte;wimax)")
set(NS3_FILTER_MODULE_EXAMPLES_AND_TESTS "" CACHE STRING "List of modules that should have their examples and tests built")

# Include core macros
include(build-support/macros-and-definitions.cmake)

# Discover internal and external modules
subdirlist(libs_to_build ${CMAKE_CURRENT_SOURCE_DIR}/src)
subdirlist(contrib_libs_to_build ${CMAKE_CURRENT_SOURCE_DIR}/contrib)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../ns-3-external-contrib)
  subdirlist(external_contrib_libs_to_build ${CMAKE_CURRENT_SOURCE_DIR}/../ns-3-external-contrib)
  set(contrib_libs_to_build ${contrib_libs_to_build} ${external_contrib_libs_to_build})
endif()

# Load .ns3rc settings
parse_ns3rc(ns3rc_enabled_modules ns3rc_disabled_modules ns3rc_examples_enabled ns3rc_tests_enabled)

# Apply module filters
filter_enabled_and_disabled_modules(libs_to_build contrib_libs_to_build NS3_ENABLED_MODULES NS3_DISABLED_MODULES ns3rc_enabled_modules ns3rc_disabled_modules)

# Use ccache if enabled
if(${NS3_CCACHE})
  find_program(CCACHE ccache)
  if(NOT ("${CCACHE}" STREQUAL "CCACHE-NOTFOUND"))
    message(STATUS "CCache is enabled.")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
    execute_process(
      COMMAND ${CCACHE} --set-config sloppiness=pch_defines,time_macros,include_file_mtime,include_file_ctime
    )
  endif()
endif()

# Process build options
process_options()

# Add build directories
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(scratch)
add_subdirectory(utils)

# Export configs
write_lock()
write_configtable()
ns3_cmake_package()

# ##############################################################################
# Optional: auto-build scratch files (custom support)
# ##############################################################################
set(target_prefix scratch_)

function(create_scratch source_files)
  list(LENGTH source_files number_sources)
  if(number_sources EQUAL 0)
    return()
  endif()

  set(scratch_src)
  foreach(source_file ${source_files})
    file(READ ${source_file} source_file_contents)
    string(REGEX MATCHALL "main[(| (]" main_position "${source_file_contents}")
    if(CMAKE_MATCH_0)
      list(APPEND scratch_src ${source_file})
    endif()
  endforeach()

  list(LENGTH scratch_src scratch_src_len)
  if(${scratch_src_len} EQUAL 0)
    message(FATAL_ERROR "No main function found in: ${source_files}")
  endif()

  if(${scratch_src_len} GREATER 1)
    message(FATAL_ERROR "Multiple main() found: ${scratch_src}")
  endif()

  get_filename_component(scratch_dirname ${scratch_src} DIRECTORY)
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" scratch_dirname "${scratch_dirname}")
  string(REPLACE "/" "_" scratch_dirname "${scratch_dirname}")
  get_filename_component(scratch_name ${scratch_src} NAME_WLE)

  if(scratch_dirname)
    set(target_prefix scratch${scratch_dirname}_)
  endif()

  get_filename_component(scratch_src ${scratch_src} ABSOLUTE)
  get_filename_component(scratch_absolute_directory ${scratch_src} DIRECTORY)
  string(REPLACE "${PROJECT_SOURCE_DIR}" "${CMAKE_OUTPUT_DIRECTORY}" scratch_directory ${scratch_absolute_directory})

  build_exec(
    EXECNAME ${scratch_name}
    EXECNAME_PREFIX ${target_prefix}
    SOURCE_FILES "${source_files}"
    LIBRARIES_TO_LINK "${ns3-libs}" "${ns3-contrib-libs}"
    EXECUTABLE_DIRECTORY_PATH ${scratch_directory}/
  )
endfunction()

file(GLOB single_source_file_scratches CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/[^.]*.cc)
foreach(scratch_src ${single_source_file_scratches})
  create_scratch(${scratch_src})
endforeach()

file(GLOB scratch_subdirectories CONFIGURE_DEPENDS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/**)
foreach(entry ${scratch_subdirectories})
  if(NOT (IS_DIRECTORY ${entry}))
    list(REMOVE_ITEM scratch_subdirectories ${entry})
  endif()
endforeach()

foreach(subdir ${scratch_subdirectories})
  if(EXISTS ${subdir}/CMakeLists.txt)
    add_subdirectory(${subdir})
  else()
    file(GLOB scratch_sources CONFIGURE_DEPENDS ${subdir}/[^.]*.cc)
    create_scratch("${scratch_sources}")
  endif()
endforeach()
