-- set minimum xmake version
set_xmakever("2.8.2")

-- set project
set_project("commonlib-shared")
set_languages("c++23")
set_warnings("allextra")
set_encodings("utf-8")

-- add rules
add_rules("mode.debug", "mode.releasedbg")

-- add options
option("rex_ini", function()
    set_default(false)
    set_description("enable REX::INI settings support")
    add_defines("REX_OPTION_INI=1")
end)

option("rex_json", function()
    set_default(false)
    set_description("enable REX::JSON settings support")
    add_defines("REX_OPTION_JSON=1")
end)

option("rex_toml", function()
    set_default(false)
    set_description("enable REX::TOML settings support")
    add_defines("REX_OPTION_TOML=1")
end)

option("xse_mmio", function()
    set_default(false)
    set_description("enable mmio based Address Library support")
    add_defines("XSE_MMIO_ADDRESSLIB=1")
end)

option("xse_xbyak", function()
    set_default(false)
    set_description("enable xbyak support for Trampoline")
    add_defines("XSE_SUPPORT_XBYAK=1")
end)

-- add packages
add_requires("spdlog", { configs = { header_only = false, wchar = true, std_format = true } })

-- add config packages
if has_config("rex_ini") then add_requires("simpleini") end
if has_config("rex_json") then add_requires("nlohmann_json") end
if has_config("rex_toml") then add_requires("toml11") end
if has_config("xse_mmio") then add_requires("rsm-mmio") end
if has_config("xse_xbyak") then add_requires("xbyak") end

target("commonlib-shared", function()
    -- set target kind
    set_kind("static")

    -- add packages
    add_packages("spdlog", { public = true })

    -- add config packages
    if has_config("rex_ini") then add_packages("simpleini", { public = true }) end
    if has_config("rex_json") then add_packages("nlohmann_json", { public = true }) end
    if has_config("rex_toml") then add_packages("toml11", { public = true }) end
    if has_config("xse_mmio") then add_packages("rsm-mmio", { public = true }) end
    if has_config("xse_xbyak") then add_packages("xbyak", { public = true }) end

    -- add options
    add_options("rex_ini", "rex_json", "rex_toml", "xse_mmio", "xse_xbyak", { public = true })

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
    set_pcxxheader("include/REX/Impl/PCH.h")

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
end)
