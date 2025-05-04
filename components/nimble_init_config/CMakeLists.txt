file(GLOB_RECURSE srcs "src/*.c")


idf_component_register(SRCS "${srcs}"
	               INCLUDE_DIRS "./include"
		       REQUIRES "main" "bt" "nvs_flash")
