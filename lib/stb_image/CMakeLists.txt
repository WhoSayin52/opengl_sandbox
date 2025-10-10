enable_language(C) 

add_library(stb_image STATIC src/stb_image.c)

target_include_directories(stb_image SYSTEM PUBLIC include)

set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C) 
