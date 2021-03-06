#������������� ����������� ������ CMake
cmake_minimum_required(VERSION 3.0)
#�������� �������
set(PROJECT TestParser)
project(${PROJECT})

#����� ��� �����������, ����� ������� �������������� � ����������� 11 �������� C++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

#������ ����������, ������� ������ ����
set(SOURCE_PATH "include/")
set(HTML_FOLDER "bin/html")
set(PCAP_FOLDER "bin/files")

#��������� include ���������� � ����������� ����������
include_directories(${SOURCE_PATH})

#������������� ���������� ��� �������� �������, ��������� � ����������� ������
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#������ ����������, ������� ������ ���� �� ����������
set(HEADERS ${SOURCE_PATH}Token.h ${SOURCE_PATH}TokenWorker.h ${SOURCE_PATH}MessageWorker.h ${SOURCE_PATH}MessageHeader.h ${SOURCE_PATH}Message.h ${SOURCE_PATH}ImapParser.h)

#������ ����������, ������� ������ ���� �� .cpp ������
set(SOURCES ${SOURCE_PATH}ImapParser.cpp ${SOURCE_PATH}TokenWorker.cpp)

#��������� ���� ��� ������ ������������ �����
add_executable(${PROJECT} main.cpp)

#��������� ���������� SHARED(������������)
add_library(ImapParser SHARED ${HEADERS} ${SOURCES})

#����������� ���������� � ������������ �����
target_link_libraries(${PROJECT} ImapParser)

#������� ���������� ��� ��������������� html-���������� � .pcap ������
file(MAKE_DIRECTORY ${HTML_FOLDER})
file(MAKE_DIRECTORY ${PCAP_FOLDER})

#�������� ���� �� ������ .css � ��������� �������� .pcap ������
configure_file(styles/style2.css bin/html/style.css COPYONLY)
configure_file(pcapFiles/imap1.pcap bin/files/imap1.pcap COPYONLY)
configure_file(pcapFiles/imap2.pcap bin/files/imap2.pcap COPYONLY)