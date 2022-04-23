set_project("GPPCC11")

add_repositories("PucklaMotzer09::JohnnyEngine https://github.com/PucklaMotzer09/xmake-repo.git johnnyengine")
add_repositories("PucklaMotzer09::cAudio https://github.com/PucklaMotzer09/xmake-repo.git caudio")
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