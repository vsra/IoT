
idf_component_register(SRCS "audio.c"
INCLUDE_DIRS "include"
REQUIRES led_strip es8311 board spiffs touch helix)
                       

target_compile_options(${COMPONENT_LIB} PRIVATE
                       -Wno-deprecated-declarations)