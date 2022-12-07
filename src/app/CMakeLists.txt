set(SOURCES
    main.cpp
)

add_executable(main ${SOURCES})

target_link_libraries(main Qt5::Widgets)
