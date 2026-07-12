set_project("template")

set_languages("c11")

add_requires("libcurl")

add_requires("raylib")
add_requires("raygui")

add_requires("cjson")
add_requires("libxau")
add_requires("libxdmcp")

target("mfgdlui")
    set_kind("binary")
    set_targetdir("build")
    add_files("src/**.c")
    add_includedirs(".")

    if os.isdir("includes") then
        add_includedirs("includes")
    end

    add_packages("libcurl")

    add_packages("raylib")

    add_packages("cjson")

    add_packages("libxau")
    add_packages("libxdmcp")


    add_cflags("-Wall", "-Wextra")
    add_links("m")
