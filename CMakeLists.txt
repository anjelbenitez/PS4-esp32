set(COMPONENT_SRCDIRS "src")

set(IDF_TARGET "esp32")
set(IDF_COMPONENT_MANAGER 0)

set(EXTRA_COMPONENT_DIRS "/tools/esp-idf/components")

set(COMPONENT_REQUIRES
        "bt"
        "esp32"
        )

register_component()
