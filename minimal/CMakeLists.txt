set(SRC_FILES minimal.cpp CMakeLists.txt)

if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES ../sample.rc)
elseif(APPLE)
    # Add an icon for the apple .app file
    list(APPEND SRC_FILES ../../src/osx/carbon/wxmac.icns)
endif()

list(APPEND CMAKE_MODULE_PATH "$ENV{SPAM_ROOT_DIR}/cmake")
find_package(wxGTK3 REQUIRED COMPONENTS mono)
find_package(wxSVG REQUIRED COMPONENTS wxSVG)
list(POP_BACK CMAKE_MODULE_PATH)

# Define the build target for the executable
target_sources(minimal PRIVATE ${SRC_FILES})
target_link_libraries(minimal PRIVATE VCPKG::gtk3)
target_link_libraries(minimal PRIVATE VCPKG::gdk3)
target_link_libraries(minimal PRIVATE VCPKG::cairo)
target_link_libraries(minimal PRIVATE VCPKG::gio)
target_link_libraries(minimal PRIVATE VCPKG::pangocairo)
target_link_libraries(minimal PRIVATE VCPKG::gdk_pixbuf)
target_link_libraries(minimal PRIVATE VCPKG::pango)
target_link_libraries(minimal PRIVATE VCPKG::gobject)
target_link_libraries(minimal PRIVATE VCPKG::gthread)
target_link_libraries(minimal PRIVATE VCPKG::glib)
target_link_libraries(minimal PRIVATE VCPKG::gmodule)
target_link_libraries(minimal PRIVATE wxSVG::wxSVG)
target_link_libraries(minimal PRIVATE wxGTK3::mono)
