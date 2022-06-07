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
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Configure target
add_library(${PROJECT_NAME} SHARED)
add_library(crossguid::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/guid.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
        ${PROJECT_BINARY_DIR}/include
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
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
    target_link_libraries(${PROJECT_NAME} LibUUID::UUID)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GUID_LIBUUID)
endif()

# Configure versioning information
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

if (CROSSGUID_TESTS)
    add_executable(crossguid-test test/TestMain.cpp test/Test.cpp)
    target_link_libraries(crossguid-test crossguid)
endif()
