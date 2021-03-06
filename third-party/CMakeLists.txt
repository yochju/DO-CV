message(STATUS "")
do_step_message("Appending third-party libraries:")

set(THIRDPARTY_FOLDER_NAME "DO Third-Party")

if (DO_BUILD_UNIT_TESTS)
  message(STATUS "  - Google Mock")
  add_subdirectory(gmock)
  set_property(TARGET gmock PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})
  set_property(TARGET gmock_main PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

  message(STATUS "  - Google Test")
  set_property(TARGET gtest PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})
  set_property(TARGET gtest_main PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})
endif ()

if (DO_BUILD_IMAGE_IO)
  message(STATUS "  - antigrain")
  add_subdirectory(antigrain)
  set_property(TARGET antigrain PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

  message(STATUS "  - zlib")
  add_subdirectory(zlib)
  set_property(TARGET zlib PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

  message(STATUS "  - libjpeg")
  add_subdirectory(libjpeg)
  set_property(TARGET jpeg PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

  message(STATUS "  - libpng")
  add_subdirectory(libpng)
  set_property(TARGET png PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

  message(STATUS "  - libtiff")
  add_subdirectory(libtiff)
  set_property(TARGET tiff PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})
  
  message(STATUS "  - easyexif")
  add_subdirectory(easyexif)
  set_property(TARGET easyexif PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})
endif ()

message(STATUS "  - FLANN")
add_subdirectory(flann)
set_property(TARGET flann_cpp_s PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

message(STATUS "  - stlplus")
add_subdirectory(stlplus)
set_property(TARGET stlplus PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

# message(STATUS "  - DAISY")
# add_subdirectory(libdaisy)