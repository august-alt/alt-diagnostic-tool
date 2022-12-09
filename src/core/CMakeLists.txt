find_package(Qt5 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS
    adtexecutable.h
    adtcheck.h
    adtresolver.h

    adtbaseexecutor.h
)

set (SOURCES
    adtexecutable.cpp
    adtcheck.cpp
    adtresolver.cpp
)

ADD_LIBRARY(adtcore STATIC ${SOURCES} ${HEADERS})
target_link_libraries(adtcore PRIVATE Qt5::Core)
