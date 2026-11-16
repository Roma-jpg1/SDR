cmake_minimum_required(VERSION 3.16)
project(PlutoSDR C CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем библиотеку SoapySDR
find_package(SoapySDR REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui)
set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/implot)

set(MAIN_SOURCE_FILES
    src/main.cpp
    src/tx_rx.cpp
    src/modulate.cpp
    src/sdr.cpp
    src/gui.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

# Добавляем исполняемый файл
add_executable(main ${MAIN_SOURCE_FILES})


target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${IMGUI_DIR}
    ${IMPLOT_DIR}
    ${SoapySDR_INCLUDE_DIRS}
)

# Линкуем библиотеки к исполняемому файлу
target_link_libraries(main PRIVATE
    ${SoapySDR_LIBRARIES}
    SDL2::SDL2
    OpenGL::GL
    Threads::Threads
    ${CMAKE_DL_LIBS}
)
