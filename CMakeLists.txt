cmake_minimum_required(VERSION 3.16)
project(PlutoSDR C CXX)

set(CMAKE_BUILD_TYPE Debug)

# Ищем библиотеку SoapySDR
find_package(SoapySDR REQUIRED)

set(MAIN_SOURCE_FILES
    src/main.cpp
    src/tx_rx.cpp
    src/modulate.cpp
    src/sdr.cpp
)

# Добавляем исполняемый файл
add_executable(main ${MAIN_SOURCE_FILES})


target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Линкуем библиотеки к исполняемому файлу
target_link_libraries(main ${SoapySDR_LIBRARIES})
