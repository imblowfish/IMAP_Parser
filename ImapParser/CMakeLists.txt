#Устанавливаем минимальную версию CMake
cmake_minimum_required(VERSION 3.0)
#Название проекта
set(PROJECT TestParser)
project(${PROJECT})

#Флаги для компилятора, чтобы выводил предупреждения и использовал 11 стандарт C++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

#Вводим переменные, которые хранят пути
set(SOURCE_PATH "include/")
set(HTML_FOLDER "bin/html")
set(PCAP_FOLDER "bin/files")

#Добавляем include директорию в заголовками библиотеки
include_directories(${SOURCE_PATH})

#Устанавливает директории для хранения архивов, библиотек и исполняемых файлов
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Вводим переменную, которая хранит пути до заголовков
set(HEADERS ${SOURCE_PATH}Token.h ${SOURCE_PATH}TokenWorker.h ${SOURCE_PATH}MessageWorker.h ${SOURCE_PATH}MessageHeader.h ${SOURCE_PATH}Message.h ${SOURCE_PATH}ImapParser.h)

#Вводим переменную, которая хранит пути до .cpp файлов
set(SOURCES ${SOURCE_PATH}ImapParser.cpp ${SOURCE_PATH}TokenWorker.cpp)

#Добавляем цель для сборки исполняемого файла
add_executable(${PROJECT} main.cpp)

#Добавялем библиотеку SHARED(динамическую)
add_library(ImapParser SHARED ${HEADERS} ${SOURCES})

#Привязываем библиотеку к исполняемому файлу
target_link_libraries(${PROJECT} ImapParser)

#Создаем директории для сгенерированных html-документов и .pcap файлов
file(MAKE_DIRECTORY ${HTML_FOLDER})
file(MAKE_DIRECTORY ${PCAP_FOLDER})

#Копируем файл со стилем .css и несколько тестовых .pcap файлов
configure_file(styles/style2.css bin/html/style.css COPYONLY)
configure_file(pcapFiles/imap1.pcap bin/files/imap1.pcap COPYONLY)
configure_file(pcapFiles/imap2.pcap bin/files/imap2.pcap COPYONLY)