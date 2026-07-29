set_project("rutils")
set_version("0.1.0")
set_license("MIT")

set_warnings("all")

add_rules("mode.debug", "mode.release", "mode.releasedbg")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })

local function restrict_windows_to_mingw()
	if is_host("windows") then
		-- Disable the MSVC 'windows' target — clang-p2996 ships windows-gnu only.
		set_defaultplat("mingw")
		set_allowedplats("mingw")
	end
end

includes("tools/xmake/toolchains/*.lua")
includes("tools/xmake/tasks/*.lua")

-- Core library: header-only, dependency-free, no toolchain pinned. Including
-- the headers must not require the clang-p2996 compiler — only compiling
-- against them does, which happens in the tests and examples below.
target("rutils")
	set_kind("headeronly")
	set_languages("c++26", { public = true })
	add_includedirs("include", { public = true })
	add_headerfiles("include/(rutils/**.h)")

-- Toolchain for the tests and examples below: clang-p2996 by default,
-- stock GCC 16 (`-freflection`) via `xmake f --gcc=y`.
option("gcc")
	set_default(false)
	set_showmenu(true)
	set_description("Compile tests and examples with GCC 16 instead of clang-p2996")
option_end()

local cxx_toolchain = has_config("gcc") and "gcc-16" or "clang-p2996"

-- Compile-smoke under clang-p2996, run via `xmake test`.
--
-- Only *defined* when the toolchain is on disk: xmake loads a target's
-- toolchain even when default(false), so an unconditional definition would
-- trip the clang-p2996 guard on a fresh `xmake`. With this gate the default
-- build needs no compiler, and `xmake test` lights up automatically once
-- `xmake setup-clang-p2996` has run. For a custom --clang-root, force it with
-- `xmake f --tests=y`. The gate does not apply to --gcc builds: gcc is a
-- system install, its toolchain raises its own hint when missing.
option("tests")
	set_default(false)
	set_showmenu(true)
	set_description("Build the compile-smoke test (auto-on once clang-p2996 is provisioned)")
option_end()

local clang_ready = os.isfile(path.join(os.projectdir(), ".toolchains", "clang-p2996", "bin",
	is_host("windows") and "clang++.exe" or "clang++"))

if has_config("tests") or has_config("gcc") or clang_ready then
	target("rutils_tests")
		set_kind("binary")
		set_default(false)
		set_toolchains(cxx_toolchain)
		add_deps("rutils")
		add_files("tests/main.cpp")
		add_tests("compile_smoke")
		restrict_windows_to_mingw()
end
