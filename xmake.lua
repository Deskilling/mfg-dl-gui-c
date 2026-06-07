set_project("template")

set_languages("c11")

add_requires("libcurl")
add_requires("raylib")

target("mfgdlui")
    set_kind("binary")
    add_files("src/**.c")
    add_includedirs(".")

    if os.isdir("includes") then
        add_includedirs("includes")
    end

    add_packages("raylib")
    add_packages("libcurl")
    add_cflags("-Wall", "-Wextra")
    add_links("m")
