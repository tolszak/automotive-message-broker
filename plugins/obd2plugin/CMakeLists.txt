if(obd2_plugin)

include(CheckIncludeFiles)

pkg_check_modules(gio REQUIRED gio-2.0)
pkg_check_modules(gio-unix REQUIRED gio-unix-2.0)

include_directories(${CMAKE_SOURCE_DIR}/lib ${include_dirs} ${gio_INCLUDE_DIRS} ${gio-unix_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/plugins/common)

set(obd2sourceplugin_headers obd2source.h obdlib.h obdpid.h asyncqueuewatcher.h)
set(obd2sourceplugin_sources obd2source.cpp obdlib.cpp obdpid.cpp asyncqueuewatcher.cpp)
add_library(obd2sourceplugin MODULE ${obd2sourceplugin_sources})
set_target_properties(obd2sourceplugin PROPERTIES PREFIX "")
target_link_libraries(obd2sourceplugin amb -L${CMAKE_CURRENT_BINARY_DIR}/lib amb-plugins-common -L${CMAKE_CURRENT_BINARY_DIR}/plugins/common ${link_libraries} ${gio_LIBRARIES} ${gio-unix_LIBRARIES} )

install(TARGETS obd2sourceplugin LIBRARY DESTINATION lib${LIB_SUFFIX}/automotive-message-broker)

endif(obd2_plugin)
