include "../RealEngine/vendor/coral/Coral.Managed"

project "RealEngine"
	filter { "not action:vs*", "not system:windows" }
        kind "StaticLib"
    filter { "action:vs* or system:windows" }
	language "C#"
	dotnetframework "net9.0"
    kind "SharedLib"
    clr "Unsafe"

	targetdir ("../Build/%{cfg.buildcfg}")
	objdir ("../Intermediates/%{cfg.buildcfg}")

	links { "Coral.Managed" }

	vsprops {
		AppendTargetFrameworkToOutputPath = "false",
		Nullable = "enable",
		CopyLocalLockFileAssemblies = "true",
		EnableDynamicLoading = "true",
	}

	files {
		"Source/RealEngine/**.cs"
	}
