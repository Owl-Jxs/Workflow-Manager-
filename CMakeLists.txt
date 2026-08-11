cmake_minimum_required(VERSION 3.15)
project(WorkflowManager LANGUAGES CXX)

# --- Estándar de C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Recolecta automáticamente todos los .cpp dentro de src/ ---
# CONFIGURE_DEPENDS hace que CMake vuelva a escanear la carpeta
# cada vez que se reconfigura el proyecto (por ejemplo al abrir
# la carpeta en Visual Studio), asi que si alguien agrega un
# archivo nuevo en src/, no hay que tocar este archivo.
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# --- Definición del ejecutable ---
add_executable(WorkflowManager ${PROJECT_SOURCES})

# --- Permite usar #include "structures/ListaDoble.h" desde cualquier
#     archivo dentro de src/, sin importar en qué subcarpeta esté ---
target_include_directories(WorkflowManager PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)

# --- El .exe compilado va a parar a una carpeta bin/ en la raíz,
#     en vez de quedar enterrado dentro de build/ ---
set_target_properties(WorkflowManager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# --- Copia la carpeta data/ (los CSV) al lado del ejecutable,
#     para que el programa los encuentre al correr desde bin/ ---
add_custom_command(TARGET WorkflowManager POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:WorkflowManager>/data"
)

# --- Warnings útiles para atrapar errores de punteros/memoria ---
if(MSVC)
    target_compile_options(WorkflowManager PRIVATE /W4)
else()
    target_compile_options(WorkflowManager PRIVATE -Wall -Wextra)
endif()
