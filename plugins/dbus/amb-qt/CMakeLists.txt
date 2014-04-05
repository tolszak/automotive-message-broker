if(qt_bindings)

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5DBus REQUIRED)

#if(Qt5Core_FOUND)
#	message(STATUS "using Qt5")
#
#	set(QT_INCLUDE_DIRS ${Qt5Core_INCLUDE_DIRS} ${Qt5Quick_INCLUDE_DIRS} ${Qt5DBus_INCLUDE_DIRS})
#	set(QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Quick_LIBRARIES} ${Qt5DBus_LIBRARIES})
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
#	message(STATUS "size of void_p: ${CMAKE_SIZEOF_VOID_P}")
#	if(CMAKE_SIZEOF_VOID_P MATCHES "8")
#		message(STATUS "can has 64bits")
#		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcmodel=large")
#	endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
#	add_definitions(${Qt5Core_DEFINITIONS})
#	set(CMAKE_AUTOMOC ON)
#endif(Qt5Core_FOUND)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(ambqt_headers ambqt.h amb-qt_global.h)
set(ambqt_sources ambqt.cpp)
set(ambqtquick_sources ambqtquick.cpp)
set(ambqtquick_headers ambqtquick.h)

#include_directories(${CMAKE_SOURCE_DIR}/lib ${include_dirs} ${QT_INCLUDE_DIRS})

add_library(amb-qt SHARED ${ambqt_sources})
add_library(ambqtquick MODULE ${ambqtquick_sources})

qt5_use_modules(amb-qt Core DBus)
qt5_use_modules(ambqtquick Core DBus Qml)

target_link_libraries(amb-qt amb -L${CMAKE_CURRENT_BINARY_DIR}/lib ${link_libraries} ${gio_LIBRARIES})
target_link_libraries(ambqtquick amb-qt -L${CMAKE_CURRENT_BINARY_DIR}/lib ${link_libraries} ${gio_LIBRARIES})

add_executable(ambtestclient test.cpp)
#qt5_use_modules(ambtestclient Core DBus)
target_link_libraries(ambtestclient amb amb-qt -L${CMAKE_CURRENT_BINARY_DIR}/lib ${link_libraries} ${gio_LIBRARIES})

install (TARGETS amb-qt LIBRARY DESTINATION lib${LIB_SUFFIX})

install (FILES ${ambqt_headers} DESTINATION ${INCLUDE_INSTALL_DIR}/ COMPONENT Devel)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir DESTINATION ${LIB_INSTALL_DIR}/qt5/qml/amb COMPONENT Devel)
install (TARGETS ambqtquick DESTINATION ${LIB_INSTALL_DIR}/qt5/qml/amb)
install (TARGETS ambtestclient RUNTIME DESTINATION bin)
endif(qt_bindings)
