-- set minimum xmake version
set_xmakever("3.0.0")

-- set project constants
set_project("commonlib-shared")
set_arch("x64")
set_languages("c++23")
set_warnings("allextra")
set_encodings("utf-8")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- add options
option("commonlib_ini", function()
    set_default(false)
    set_description("enable REX::INI settings support")
end)

option("commonlib_json", function()
    set_default(false)
    set_description("enable REX::JSON settings support")
end)

option("commonlib_toml", function()
    set_default(false)
    set_description("enable REX::TOML settings support")
end)

option("commonlib_xbyak", function()
    set_default(false)
    set_description("enable xbyak support for Trampoline")
end)

-- add packages
add_requires("spdlog v1.16.0", { configs = { header_only = false, wchar = true, std_format = true } })

-- add config packages
if has_config("commonlib_ini") then add_requires("simpleini v4.25") end
if has_config("commonlib_json") then add_requires("glaze v7.0.0") end
if has_config("commonlib_toml") then add_requires("toml11 v4.4.0") end
if has_config("commonlib_xbyak") then add_requires("xbyak v7.06") end

target("commonlib-shared", function()
    -- set target kind
    set_kind("static")

    -- set build by default
    set_default(os.scriptdir() == os.projectdir())

    -- add packages
    add_packages("spdlog", { public = true })

    -- add config packages
    if has_config("commonlib_ini") then
        add_packages("simpleini", { public = true })
        add_defines("COMMONLIB_OPTION_INI=1", { public = true })
    end

    if has_config("commonlib_json") then
        add_packages("glaze", { public = true })
        add_defines("COMMONLIB_OPTION_JSON=1", { public = true })
    end

    if has_config("commonlib_toml") then
        add_packages("toml11", { public = true })
        add_defines("COMMONLIB_OPTION_TOML=1", { public = true })
    end

    if has_config("commonlib_xbyak") then
        add_packages("xbyak", { public = true })
        add_defines("COMMONLIB_OPTION_XBYAK=1", { public = true })
    end

    -- add options
    add_options("commonlib_ini", "commonlib_json", "commonlib_toml", "commonlib_xbyak", { public = true })

    -- add system links
    add_syslinks("advapi32", "bcrypt", "d3d11", "d3dcompiler", "dbghelp", "dxgi", "ole32", "shell32", "user32", "version", "ws2_32")

    -- add source files
    add_files("src/**.cpp")

    -- add header files
    add_includedirs("include", { public = true })
    add_headerfiles(
        "include/(REL/**.h)",
        "include/(REX/**.h)"
    )

    -- set precompiled header
    set_pcxxheader("src/REX/PCH.h")

    -- add flags
    add_cxxflags("/EHsc", "/permissive-", { public = true })

    -- add flags (cl)
    add_cxxflags(
        "cl::/bigobj",
        "cl::/cgthreads8",
        "cl::/diagnostics:caret",
        "cl::/external:W0",
        "cl::/fp:contract",
        "cl::/fp:except-",
        "cl::/guard:cf-",
        "cl::/Zc:preprocessor",
        "cl::/Zc:templateScope"
    )

    -- add flags (cl: disable warnings)
    add_cxxflags(
        "cl::/wd4200", -- nonstandard extension used : zero-sized array in struct/union
        "cl::/wd4201", -- nonstandard extension used : nameless struct/union
        "cl::/wd4324", -- structure was padded due to alignment specifier
        { public = true }
    )

    -- add flags (cl: warnings -> errors)
    add_cxxflags(
        "cl::/we4715", -- not all control paths return a value
        { public = true }
    )

    -- add flags (clang-cl)
    add_cxxflags(
        "clang_cl::-fms-compatibility",
        "clang_cl::-fms-extensions",
        { public = true }
    )

    -- add flags (clang-cl: disable warnings)
    add_cxxflags(
        "clang_cl::-Wno-delete-non-abstract-non-virtual-dtor",
        "clang_cl::-Wno-deprecated-volatile",
        "clang_cl::-Wno-ignored-qualifiers",
        "clang_cl::-Wno-inconsistent-missing-override",
        "clang_cl::-Wno-invalid-offsetof",
        "clang_cl::-Wno-microsoft-include",
        "clang_cl::-Wno-overloaded-virtual",
        "clang_cl::-Wno-pragma-system-header-outside-header",
        "clang_cl::-Wno-reinterpret-base-class",
        "clang_cl::-Wno-switch",
        "clang_cl::-Wno-unused-private-field",
        { public = true }
    )
end)
