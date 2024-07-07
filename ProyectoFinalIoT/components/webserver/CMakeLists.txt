idf_component_register(SRCS "webserver.c"
                    INCLUDE_DIRS "include"
                    EMBED_FILES "./index.html"
                    EMBED_FILES "./script.js"
                    EMBED_FILES "./style.css"
                    REQUIRES esp_http_server APSTAmodule nvs_flash)
