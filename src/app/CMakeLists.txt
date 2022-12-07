find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(SOURCES
    main.cpp
)

add_executable(main ${SOURCES})

target_link_libraries(main Qt5::Core Qt5::Widgets)
