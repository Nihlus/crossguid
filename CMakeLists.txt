cmake_minimum_required(VERSION 3.5.1)
project(crossguid
	LANGUAGES C CXX
	VERSION 0.2.3
)

# Configure CMake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
option(CROSSGUID_TESTS "Build test runner" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

include(GenerateExportHeader)

# Configure language settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Configure target
add_library(${PROJECT_NAME})
add_library(crossguid::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PRIVATE
		src/guid.cpp
		src/platform/android.cpp
		src/platform/cfuuid.cpp
		src/platform/libuuid.cpp
		src/platform/windows.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
        ${PROJECT_BINARY_DIR}/include
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

# Configure dependencies
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUID_WINDOWS)
elseif(APPLE)
    find_library(CFLIB CoreFoundation)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${CFLIB}
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUID_CFUUID)
elseif(ANDROID)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUID_ANDROID)
else()
    find_package(LibUUID REQUIRED)
    if (NOT LIBUUID_FOUND)
        message(FATAL_ERROR
            "You might need to run 'sudo apt-get install uuid-dev' or similar")
    endif()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            LibUUID::UUID
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUID_LIBUUID)
endif()

# Configure versioning information
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Configure generated code
generate_export_header(${PROJECT_NAME}
	EXPORT_FILE_NAME include/crossguid/${PROJECT_NAME}-export.h
)

if (CROSSGUID_TESTS)
    add_subdirectory(test)
endif()
