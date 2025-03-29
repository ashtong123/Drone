# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

# OPTIONAL PROJECT VARIABLES CAN GO HERE (below cmake_min... and above include...):
# https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html#optional-project-variables
set(COMPONENTS "main" "esp_adc" "log" "freertos")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(main)

# OPTIONAL OVERRIDE PROJECT BUILD SPECS GO HERE (below project...)
# https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html#overriding-default-build-specifications
