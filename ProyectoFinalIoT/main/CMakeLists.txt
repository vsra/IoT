<<<<<<< Updated upstream
idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES APSTAmodule nvs_flash)
=======
#set(COMPONENT_SRCS "main.c" )

#set(COMPONENT_ADD_INCLUDEDIRS "include")

#register_component()

# Create a SPIFFS image from the contents of the 'spiffs' directory
# that fits the partition named 'storage'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'idf.py -p PORT flash'.



idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES webserver NTP esp_wifi MQTTComponent mqtt QueueComponent)

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)

target_compile_options(${COMPONENT_LIB} PRIVATE
                                        -Wno-deprecated-declarations)

>>>>>>> Stashed changes
