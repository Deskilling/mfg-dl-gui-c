set_project("template")

set_languages("c11")

add_requires("libglfw3", { system = true })
add_requires("cglm", { system = true })
add_requires("libcurl")

target("leif")
    set_kind("static")
    set_languages("c11")

    add_files("external/leif/leif.c")
    add_files("external/leif/vendor/glad/src/glad.c")

    add_defines("LF_GLFW", { public = true })

    add_includedirs(
        "external/leif/vendor/glad/include",
        "external/leif/vendor/stb_image",
        "external/leif/vendor/stb_image_resize",
        "external/leif/vendor/stb_truetype",

        "external/leif/include",
        "external/leif",
        { public = true}
    )

    add_packages("libglfw3", "cglm", { public = true})
    add_syslinks("GL", "m", "clipboard")

target("mfgdlui")
    set_kind("binary")

    add_files("src/**.c")

    add_includedirs(".")

    if os.isdir("includes") then
        add_includedirs("includes")
    end

    add_deps("leif")

    add_packages("libcurl")

    add_cflags("-Wall", "-Wextra")

    add_links("m")
