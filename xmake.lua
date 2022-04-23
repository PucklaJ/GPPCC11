set_project("GPPCC11")

add_repositories("PucklaMotzer09 https://github.com/PucklaMotzer09/xmake-repo.git johnnyengine")
add_requires("johnnyengine")

add_rules("mode.debug", "mode.release")
target("battle_of_yggdrasil")
  set_kind("binary")
  if is_plat("windows") then
    add_cxxflags("/EHsc")
  end
  add_packages("johnnyengine")

  add_files("src/*.cpp")
  add_includedirs("include")
  add_headerfiles("include/*.h")