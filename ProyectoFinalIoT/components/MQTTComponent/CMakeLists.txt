idf_component_register(SRCS "MQTTComponent.c"
                    INCLUDE_DIRS "include"
                    REQUIRES mqtt QueueComponent loggercircular)
