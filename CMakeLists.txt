cmake_minimum_required(VERSION 3.5.1)
project(crossguid
	LANGUAGES CXX
	VERSION 0.2.3
)

# Configure CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
option(CROSSGUID_TESTS "Build test runner" ON)

# Configure language settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure target
add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/guid.cpp
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

set_property(TARGET ${PROJECT_NAME}
	PROPERTY
		POSITION_INDEPENDENT_CODE ON
)

# Configure dependencies
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUID_WINDOWS)
elseif(APPLE)
    find_library(CFLIB CoreFoundation)
    target_link_libraries(${PROJECT_NAME} ${CFLIB})
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUID_CFUUID)
elseif(ANDROID)
    # GUID_ANDROID is used in the headers, so make PUBLIC
    target_compile_definitions(${PROJECT_NAME} PUBLIC GUID_ANDROID)
else()
    find_package(LibUUID REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
            "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIBUUID_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${LIBUUID_LIBRARY})
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUID_LIBUUID)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(WARNINGS "-Werror" "-Wall")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(WARNINGS "-Werror" "-Wall")
elseif(MSVC)
    set(WARNINGS "/WX" "/W4")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNINGS})

set_target_properties(${PROJECT_NAME}
					  PROPERTIES
					  VERSION ${PROJECT_VERSION}
					  SOVERSION ${PROJECT_VERSION_MAJOR}
					  DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	include(GNUInstallDirs)

	set(CROSSGUID_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
	set(CROSSGUID_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
	set(CROSSGUID_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
	set(CROSSGUID_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
	set(CROSSGUID_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")

	set(CROSSGUID_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/crossguid/cmake")
	set(CROSSGUID_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/crossguid")

	# Install target
	install(TARGETS ${PROJECT_NAME} EXPORT crossguidTargets
		RUNTIME       DESTINATION ${CROSSGUID_RUNTIME_INSTALL_DIR}
		LIBRARY       DESTINATION ${CROSSGUID_LIBRARY_INSTALL_DIR}
		ARCHIVE       DESTINATION ${CROSSGUID_ARCHIVE_INSTALL_DIR}
		FRAMEWORK     DESTINATION ${CROSSGUID_FRAMEWORK_INSTALL_DIR})

	# Install headers
	install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
		DESTINATION ${CROSSGUID_INC_INSTALL_DIR})

	# Make cmake config files for all targets
	install(EXPORT  crossguidTargets
		DESTINATION ${CROSSGUID_CMAKE_CONFIG_INSTALL_DIR}
		FILE crossguid-config.cmake)

	# Install readme and license
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
		DESTINATION ${CROSSGUID_ADDITIONAL_FILES_INSTALL_DIR})

	configure_file(crossguid.pc.in ${PROJECT_BINARY_DIR}/crossguid.pc @ONLY)
	install(FILES ${PROJECT_BINARY_DIR}/crossguid.pc DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig)
endif()

if (CROSSGUID_TESTS)
    add_executable(crossguid-test test/TestMain.cpp test/Test.cpp)
    target_link_libraries(crossguid-test crossguid)
endif()
