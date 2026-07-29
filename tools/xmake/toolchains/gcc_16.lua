-- gcc 16 toolchain.
--
-- GCC 16 ships C++26 reflection (P2996) behind -freflection. Unlike
-- clang-p2996 this is a stock compiler: no provisioning task, just a system
-- install (`brew install gcc` on macOS, distro package on Linux). The default
-- gcc-root covers those; override with `xmake f --gcc-root=...`.

toolchain("gcc-16")
set_kind("standalone")

on_load(function(tc)
	local gccroot = get_config("gcc-root")
	local bindir = path.join(gccroot, "bin")
	local suffix = is_host("windows") and ".exe" or ""

	-- Homebrew and distro installs suffix the binaries (g++-16); a
	-- from-source install does not. Take whichever is present.
	local function find_tool(...)
		for _, name in ipairs({ ... }) do
			local file = path.join(bindir, name .. suffix)
			if os.isfile(file) then
				return file
			end
		end
	end

	local cxx = find_tool("g++-16", "g++")
	if not cxx then
		raise("g++ 16 not found under %s\n"
			.. "install gcc 16 (e.g. `brew install gcc`) or pass --gcc-root=...", bindir)
	end

	local cc = find_tool("gcc-16", "gcc")
	local ar = find_tool("gcc-ar-16", "gcc-ar", "ar")

	tc:set("toolset", "cc", cc)
	tc:set("toolset", "cxx", cxx)
	tc:set("toolset", "ld", cxx)
	tc:set("toolset", "sh", cxx)
	tc:set("toolset", "as", cc)
	tc:set("toolset", "ar", ar)

	tc:add(
		"cxxflags",
		"-freflection",
		-- gcc's analog of clang's -fconstexpr-steps.
		"-fconstexpr-ops-limit=100000000",
		"-ftemplate-depth=2048",
		{ force = true }
	)
end)

option("gcc-root")
set_showmenu(true)
set_description("Root directory of gcc 16.")
set_default(os.host() == "macosx" and "/opt/homebrew" or "/usr")
