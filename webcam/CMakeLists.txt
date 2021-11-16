project(WEBCAM)
cmake_minimum_required(VERSION 3.21)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTLIBS REQUIRED
        gobject-2.0
        glib-2.0
        gstreamer-webrtc-1.0
        gstreamer-sdp-1.0
        gstreamer-pbutils-1.0
        gstreamer-rtsp-server-1.0
        gstreamer-check-1.0)


#including GStreamer header files directory
include_directories(
        ${GSTREAMER_INCLUDE_DIRS}
        ${GSTLIBS_INCLUDE_DIRS}
        
)

#linking GStreamer library directory
link_directories(
        ${GSTREAMER_LIBRARY_DIRS}
        ${GSTLIBS_LIBRARY_DIRS})

add_library(webcam STATIC src/webcam.cpp)
target_include_directories(webcam PUBLIC include)
target_link_libraries(webcam ${GSTREAMER_LIBRARIES} ${GSTLIBS_LIBRARIES})


add_executable(main main.cpp)
target_link_libraries(main webcam)



