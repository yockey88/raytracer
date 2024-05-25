local raytracing_ex = {}

raytracing_ex.name = "raytracing_ex"
raytracing_ex.path = "./examples/raytracing"
raytracing_ex.kind = "ConsoleApp"
raytracing_ex.language = "C++"
raytracing_ex.dialect = "C++latest"

raytracing_ex.files = function()
  files { "./raytracing/**.cpp" }
end

raytracing_ex.include_dirs = function()
  includedirs{ "./raytracing" }
end

raytracing_ex.components = {}
raytracing_ex.components["raytracer"] = { "%{wks.location}/raytracer/src" }

local other_ex = {}

other_ex.name = "other_ex"
other_ex.path = "./examples/other"
other_ex.kind = "ConsoleApp"
other_ex.language = "C++"
other_ex.dialect = "C++latest"

other_ex.files = function()
  files { "./other/**.cpp" }
end

other_ex.include_dirs = function()
  includedirs{ "./other" }
end

other_ex.components = {}
other_ex.components["raytracer"] = { "%{wks.location}/raytracer/src" }

AddProject(raytracing_ex)
AddProject(other_ex)
