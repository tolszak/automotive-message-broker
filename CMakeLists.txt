project(automotive-message-broker)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE, Debug)

include(FindPkgConfig)

set(PROJECT_NAME "automotive-message-broker")
set(PROJECT_VERSION "0.11.808")
set(PROJECT_CODENAME "veyron")
set(PROJECT_QUALITY "alpha")

add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DPROJECT_CODENAME="${PROJECT_CODENAME}")
add_definitions(-DPROJECT_QUALITY="${PROJECT_QUALITY}")

set (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set (LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/${CMAKE_LIBRARY_ARCHITECTURE}" CACHE STRING "Directory where lib will install")
set (INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "The directory the headers are installed in")
set (DOC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/doc/packages/${PROJECT_NAME}" CACHE PATH "The directory the docs will be installed to")

option(qtmainloop "Use QCoreApplication mainloop " OFF)
option(websocket_plugin "websocket source and sink plugins" OFF)
option(websocketold_plugin "old websocket sink plugin" OFF)
option(tpms_plugin "TPMS plugin " OFF)
option(obd2_plugin "OBD-II plugin" OFF)
option(database_plugin "Database plugins" OFF)
option(qt_bindings "AMB Qt DBus bindings" OFF)
option(opencvlux_plugin "OpenCV Lux plugin" OFF)
option(gpsd_plugin "gpsd location plugin" OFF)
option(murphy_plugin "murphy policy framework plugin" OFF)
option(test_plugin "Test Plugin" ON)
option(bluemonkey_plugin "bluemonkey irc plugin" OFF)
option(gpsnmea_plugin "gps NMEA location plugin" OFF)
option(openxc_plugin "OpenXC plugin" OFF)
option(obdlinkcan_plugin "ObdLink mx CAN plugin" OFF)
option(cansim_plugin "Can simulator plugin" OFF)
option(cangen_plugin "Can simulator plugin" OFF)
option(enable_icecc "Enable icecc checking, for distributed compilation" ON)
option(enable_docs "enable Doxygen doc generation" OFF)

if(opencvlux_plugin)
   message(STATUS "OpenCV Lux plugin enabled")

endif(opencvlux_plugin)

include (CMakeForceCompiler)

if (enable_icecc)
   find_program(ICECC /usr/lib/icecc/bin/g++)
    if (ICECC)
        message(STATUS "icecc will be used for distributed compiling")
        cmake_force_cxx_compiler(${ICECC} icecc)
    else(ICECC)
        message(STATUS "Not using icecc")
    endif(ICECC)
endif(enable_icecc)

# add a target to generate API documentation with Doxygen
if(enable_docs)
message(STATUS "doxygen doc generation enabled")
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(all ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)
endif(enable_docs)

find_library(libtool_LIBRARY ltdl DOC "Libtool libraries")
find_path(libtool_INCLUDE_DIR ltdl.h DOC "Libtool headers")

if(libtool_LIBRARY)
     message(STATUS "libltdl / libtool found")
else(libtool_LIBRARY)
     message(FATAL_ERROR "libltdl / libtool missing.  please install libltdl / libtool")
endif(libtool_LIBRARY)

find_package(Boost REQUIRED)


pkg_check_modules(glib REQUIRED glib-2.0 gobject-2.0)
pkg_check_modules(json REQUIRED json)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
add_definitions(-DDBusServiceName="org.automotive.message.broker")

set(include_dirs ${libtool_INCLUDE_DIR} ${glib_INCLUDE_DIRS} ${gio_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${json_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(link_libraries -lamb ${libtool_LIBRARY} ${glib_LIBRARIES} ${json_LIBRARIES} -L${CMAKE_CURRENT_BINARY_DIR}/lib)

if(qtmainloop)
	message(STATUS "using Qt mainloop")

	find_package(Qt5Core)
	if(Qt5Core_FOUND)
		message(STATUS "using Qt5")
		set(QT_INCLUDE_DIRS ${Qt5Core_INCLUDE_DIRS})
		set(QT_LIBRARIES ${Qt5Core_LIBRARIES})
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
		if(CMAKE_SIZEOF_VOID_P MATCHES "8")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcmodel=large")
		endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
		add_definitions(${Qt5Core_DEFINITIONS})
	else(Qt5Core_FOUND)
		message(STATUS "using Qt4")
		find_package( Qt4 REQUIRED )
		include(${QT_USE_FILE})

	endif(Qt5Core_FOUND)

	set(include_dirs ${include_dirs} ${QT_INCLUDE_DIRS})
	set(link_libraries ${link_libraries} ${QT_LIBRARIES})

	add_definitions(-DQT_NO_KEYWORDS)
	add_definitions(-DUSE_QT_CORE)
endif(qtmainloop)

set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
add_custom_target(dist COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# packaging stuff:
# Common things to every type of package
SET(CPACK_PACKAGE_DESCRIPTION "daemon to access vehicle data")
SET(CPACK_PACKAGE_NAME "automotive-message-broker")
SET(CPACK_PACKAGE_EXECUTABLES "ambd")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
SET(CPACK_STRIP_FILES true)
SET(CPACK_PACKAGE_CONTACT "tripzero.kev@gmail.com")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")


# Always build .tar.gz and .sh files
SET(CPACK_GENERATOR "STGZ;TGZ")

SET(CPACK_GENERATOR "${CPACK_GENERATOR};DEB")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "gpsd (>=2.0), libopencv, libjson0, libltdl7")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})

INCLUDE(CPack)

add_subdirectory(lib)
add_subdirectory(ambd)
add_subdirectory(plugins)
add_subdirectory(docs)
add_subdirectory(tests)
add_subdirectory(examples)
