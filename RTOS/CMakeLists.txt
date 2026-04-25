cmake_minimum_required(VERSION 4.3)
project(RTOS VERSION 1.0)   # ← язык определяется автоматически

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# Указываем директорию с исходниками (если не в корне)
# ============================================
# Если файлы в папке src:
# set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# Если файлы там же, где CMakeLists.txt:
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# ============================================
# Исходные файлы (проверьте имена!)
# ============================================
set(RTOS_SOURCES
    ${SOURCE_DIR}/os.cpp
    ${SOURCE_DIR}/task.cpp
    ${SOURCE_DIR}/resource.cpp
    ${SOURCE_DIR}/event.cpp      # если есть
    ${SOURCE_DIR}/global.cpp
    ${SOURCE_DIR}/switch_context.asm 
)

set(RTOS_HEADERS
    ${SOURCE_DIR}/rtos_api.h
    ${SOURCE_DIR}/sys.h        # если есть
)

# ============================================
# Проверка: существуют ли файлы?
# ============================================
foreach(file ${RTOS_SOURCES})
    if(NOT EXISTS ${file})
        message(WARNING "File not found: ${file}")
    endif()
endforeach()

# ============================================
# Статическая библиотека
# ============================================
add_library(rtos STATIC ${RTOS_SOURCES} ${RTOS_HEADERS})
target_include_directories(rtos PUBLIC ${SOURCE_DIR})

# ============================================
# Тестовый исполняемый файл
# ============================================
add_executable(rtos_test ${SOURCE_DIR}/test.cpp)
target_link_libraries(rtos_test rtos)
