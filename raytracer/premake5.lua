local raytracer = {}
raytracer.name = "raytracer"
raytracer.path = "./raytracer"
raytracer.kind = "StaticLib"
raytracer.language = "C++"
raytracer.cppdialect = "C++latest"

raytracer.files = function()
  files {
    "./src/**.cpp" ,
    "./src/**.hpp"
  }
end

raytracer.include_dirs = function()
  includedirs { "./src/" }
end

raytracer.windows_configurations = function()
  files {}
  includedirs {}
  systemversion "latest"
  -- entrypoint "WinMainCRTStartup"
end

AddProject(raytracer)
