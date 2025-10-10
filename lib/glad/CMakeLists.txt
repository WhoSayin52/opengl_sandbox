enable_language(C) 

add_library(glad STATIC src/glad.c)

target_include_directories(glad SYSTEM PUBLIC include)

set_target_properties(glad PROPERTIES LINKER_LANGUAGE C) 
