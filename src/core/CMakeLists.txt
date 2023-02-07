find_package(Qt5 REQUIRED COMPONENTS Core DBus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS
    adtexecutable.h
    adtcheck.h
    adtresolver.h

    adtexecutablerunner.h
    adtexecutablerunnerprivate.h

    adtjsonconverter.h
    alteratorexecutordbusinterface.h
    adtjsonloader.h
)

set (SOURCES
    adtexecutable.cpp
    adtcheck.cpp
    adtresolver.cpp

    adtexecutablerunner.cpp
    adtexecutablerunnerprivate.cpp

    adtjsonconverter.cpp
    adtjsonloader.cpp
)

ADD_LIBRARY(adtcore STATIC ${SOURCES} ${HEADERS})
target_link_libraries(adtcore PRIVATE Qt5::Core Qt5::DBus)
