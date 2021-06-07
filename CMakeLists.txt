cmake_minimum_required(VERSION 3.15)
project(vim)

set(CMAKE_CXX_STANDARD 14)


if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    add_compile_definitions("_GLIBCXX_DEBUG")
    if (UNIX)
        add_compile_options(-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wunused)
        add_compile_options(-Wno-sign-compare)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -ggdb -fsanitize=address -fno-sanitize-recover")
    endif()
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)

if (UNIX)
    set(CMAKE_CXX_CPPCHECK cppcheck --error-exitcode=0 --enable=all --suppress=missingIncludeSystem)
endif(UNIX)

if (UNIX)
    set(cursesDir ${CMAKE_CURRENT_SOURCE_DIR}/PDCurses/x11)
    message(STATUS "using curses: ${cursesDir}")
    add_custom_command(OUTPUT ${cursesDir}/libXCurses.a ${cursesDir}/libXCurses.so
                       COMMAND ./configure && make
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/PDCurses/x11)

    add_custom_target(
                curses
                 DEPENDS ${cursesDir}/libXCurses.a ${cursesDir}/libXCurses.so
                 )
    set(CURSES_LIBRARIES "${cursesDir}/libXCurses.so")
    set(CURSES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/PDCurses")
endif (UNIX)

if(WIN32)
    set(cursesDir "${CMAKE_CURRENT_SOURCE_DIR}\\PDCurses\\wincon")
    message(STATUS "WIN32: using curses: ${cursesDir}")
    add_custom_command(OUTPUT "${cursesDir}\\pdcurses.lib"
            COMMAND nmake -f Makefile.vc
            WORKING_DIRECTORY "${cursesDir}")

    add_custom_target(curses
            DEPENDS "${cursesDir}\\pdcurses.lib")

    set(CURSES_LIBRARIES "${cursesDir}\\pdcurses.lib")
    set(CURSES_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}\\PDCurses")
endif(WIN32)

#find_package(Curses REQUIRED)
include_directories("${CURSES_INCLUDE_DIR}")
add_executable(vim
        main.cpp
        view/Window.cpp
        view/TextField.cpp
        view/TextView.cpp
        model/Model.cpp
        controller/command_controller.cpp
        controller/normal_controller.cpp
        controller/input_controller.cpp
        controller/help_controller.cpp
        controller/search_mode.cpp 
	    MyString/MyString.cpp
	    MyString/MyString.h "controller/Controller.cpp")
add_dependencies(vim curses)
target_link_libraries(vim "${CURSES_LIBRARIES}")
