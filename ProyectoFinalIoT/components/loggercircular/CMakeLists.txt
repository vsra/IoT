idf_component_register(SRCS "loggercircular.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "nvs_flash" "NTP" "CustomDelay" "QueueComponent")
