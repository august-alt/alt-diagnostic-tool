find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(HEADERS
    intropagewidget.h
    checkpagewidget.h
    repairpagewidget.h
    finishpagewidget.h
)

set(SOURCES
    intropagewidget.cpp
    checkpagewidget.cpp
    repairpagewidget.cpp
    finishpagewidget.cpp
    main.cpp
)

set(UI_FORMS
    intropagewidget.ui
    checkpagewidget.ui
    repairpagewidget.ui
    finishpagewidget.ui
)
set(RESOURCES
    resources.qrc
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})

qt5_add_resources (RCC_SOURCES ${RESOURCES})

add_executable(main ${SOURCES} ${HEADERS} ${UI_SOURCES} ${RCC_SOURCES})

target_link_libraries(main Qt5::Widgets Qt5::Core Qt5::Gui)
