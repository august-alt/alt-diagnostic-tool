find_package(Qt5 REQUIRED COMPONENTS Core DBus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS
    adtexecutable.h
    adtcheck.h
    adtresolver.h

    adtjsonconverter.h

    diagnostictool.h
    diagnostictoolprivate.h
    alteratorexecutordbusinterface.h
)

set (SOURCES
    adtexecutable.cpp
    adtcheck.cpp
    adtresolver.cpp

    adtjsonconverter.cpp
    diagnostictool.cpp
    diagnostictoolprivate.cpp
)

ADD_LIBRARY(adtcore STATIC ${SOURCES} ${HEADERS})
target_link_libraries(adtcore PRIVATE Qt5::Core Qt5::DBus)
