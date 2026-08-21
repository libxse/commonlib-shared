cmake_minimum_required(VERSION 3.20)

option(COMMONLIB_INI "enable REX::INI settings support" OFF)
option(COMMONLIB_JSON "enable REX::JSON settings support" OFF)
option(COMMONLIB_TOML "enable REX::TOML settings support" OFF)
option(COMMONLIB_XBYAK "enable xbyak support for Trampoline" OFF)

project(
    commonlib-shared
    LANGUAGES CXX
)

include(GNUInstallDirs)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

find_package(spdlog REQUIRED CONFIG)

if (COMMONLIB_INI)
    find_package(SimpleIni REQUIRED CONFIG)
endif()

if (COMMONLIB_JSON)
    find_package(glaze REQUIRED CONFIG)
endif()

if (COMMONLIB_TOML)
    find_package(toml11 REQUIRED CONFIG)
endif()

if (COMMONLIB_XBYAK)
    find_package(xbyak REQUIRED CONFIG)
endif()

include(res/cmake/sourcelist.cmake)

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES ${SOURCES}
)

add_library(
    "${PROJECT_NAME}"
    STATIC
    ${SOURCES}
)

add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_compile_definitions(
    "${PROJECT_NAME}"
    PUBLIC
        "$<$<BOOL:${COMMONLIB_INI}>:COMMONLIB_OPTION_INI=1>"
		"$<$<BOOL:${COMMONLIB_JSON}>:COMMONLIB_OPTION_JSON=1>"
		"$<$<BOOL:${COMMONLIB_TOML}>:COMMONLIB_OPTION_TOML=1>"
		"$<$<BOOL:${COMMONLIB_XBYAK}>:COMMONLIB_OPTION_XBYAK=1>"
)

target_compile_features(
	"${PROJECT_NAME}"
	PUBLIC
		cxx_std_23
)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(
        "${PROJECT_NAME}"
        PUBLIC
            # warnings -> errors
            /we4715 # not all control paths return a value

            # disable warnings
            /wd4200 # nonstandard extension used : zero-sized array in struct/union
            /wd4201 # nonstandard extension used : nameless struct/union
            /wd4324 # structure was padded due to alignment specifier
    )
endif()

target_include_directories(
	"${PROJECT_NAME}"
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(
    "${PROJECT_NAME}"
    PUBLIC
        spdlog::spdlog
        advapi32.lib
        bcrypt.lib
        d3d11.lib
        d3dcompiler.lib
        dbghelp.lib
        dxgi.lib
        ole32.lib
        shell32.lib
        user32.lib
        version.lib
        ws2_32.lib
        "$<$<BOOL:${COMMONLIB_INI}>:SimpleIni::SimpleIni>"
        "$<$<BOOL:${COMMONLIB_JSON}>:glaze::glaze>"
        "$<$<BOOL:${COMMONLIB_TOML}>:toml11::toml11>"
        "$<$<BOOL:${COMMONLIB_XBYAK}>:xbyak::xbyak>"
)

target_precompile_headers(
    "${PROJECT_NAME}"
    PRIVATE
        src/REX/PCH.h
)

install(
    TARGETS "${PROJECT_NAME}"
    EXPORT "${PROJECT_NAME}-targets"
)

install(
	EXPORT "${PROJECT_NAME}-targets"
	NAMESPACE "${PROJECT_NAME}::"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

configure_file(
	res/cmake/config.cmake.in
	"${PROJECT_NAME}Config.cmake"
	@ONLY
)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
	DIRECTORY
		"include/REL"
		"include/REX"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
