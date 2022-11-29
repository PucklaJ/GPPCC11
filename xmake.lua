set_project("GPPCC11")

add_requires("johnnyengine", "caudio")

add_rules("mode.debug", "mode.release")
target("battle_of_yggdrasil")
  set_kind("binary")
  if is_plat("windows") then
    add_cxxflags("/EHsc")
  end
  add_packages("johnnyengine", "caudio")

  add_files("src/*.cpp")
  add_includedirs("include")
  add_headerfiles("include/*.h")
