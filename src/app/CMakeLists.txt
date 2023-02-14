macro(add_adt_executable name)
  # add executable
  add_executable(${name} ${ARGN})

  # add install target
  set_target_properties(${name} PROPERTIES OUTPUT_NAME ${name})
  install(TARGETS ${name} DESTINATION bin
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

  # copy binary
  add_custom_command(TARGET ${name} POST_BUILD COMMAND
      ${CMAKE_COMMAND} ARGS -E copy
        $<TARGET_FILE:${name}>
        ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:${name}>
      COMMENT "Copy ${name} to ${CMAKE_BINARY_DIR}/bin")

endmacro(add_adt_executable)

find_package(Qt5 COMPONENTS Widgets Core Gui DBus Xml REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(HEADERS
    introwizardpage.h
    checkwizardpage.h
    repairwizardpage.h
    finishwizardpage.h
    executablestatuswidget.h
    abstractexecutablepage.h
    adtwizardpage.h
    adtwizardpage.inl


    adtwizard.h
    slotconnector.h
)

set(SOURCES
    introwizardpage.cpp
    checkwizardpage.cpp
    repairwizardpage.cpp
    finishwizardpage.cpp
    executablestatuswidget.cpp
    abstractexecutablepage.cpp

    adtwizard.cpp
    slotconnector.cpp
    main.cpp
)

set(UI_FORMS

    introwizardpage.ui
    checkwizardpage.ui
    finishwizardpage.ui
    executablestatuswidget.ui
)
set(RESOURCES
    resources.qrc
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})

qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_adt_executable(alt-diagnostic-tool ${SOURCES} ${HEADERS} ${UI_SOURCES} ${RCC_SOURCES})

target_link_libraries(alt-diagnostic-tool Qt5::Widgets Qt5::Core Qt5::DBus Qt5::Xml Qt5::Gui adtcore)
