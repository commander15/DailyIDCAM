cmake_minimum_required(VERSION 3.30)

project(DailyIDCAM VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core OPTIONAL_COMPONENTS Network)
qt_standard_project_setup()

# QXlsx
include(FetchContent)
FetchContent_Declare(QXlsx
    GIT_REPOSITORY https://github.com/QtExcel/QXlsx.git
    GIT_TAG v1.5.0
    SOURCE_SUBDIR QXlsx
)
FetchContent_MakeAvailable(QXlsx)

qt_add_executable(Tool
    main.cpp
    activitysheet.h activitysheet.cpp
    res.qrc
    activitysheetwriter.h activitysheetwriter.cpp
    activitysheetreader.h activitysheetreader.cpp
    utils.h utils.cpp
)

set_target_properties(Tool PROPERTIES OUTPUT_NAME idcam-day)

target_link_libraries(Tool PRIVATE QXlsx)

if (Qt6Network_FOUND)
    target_sources(Tool PRIVATE clockify.h clockify.cpp)
    target_link_libraries(Tool PRIVATE Qt::Network)
endif()

target_compile_definitions(Tool PRIVATE TOOL_VERSION="${PROJECT_VERSION}")

install(TARGETS Tool RUNTIME DESTINATION bin)

qt_generate_deploy_app_script(TARGET Tool OUTPUT_SCRIPT INSTALL_SCRIPT NO_TRANSLATIONS)
install(SCRIPT ${INSTALL_SCRIPT})

set(CPACK_GENERATOR ZIP TGZ)
include(CPack)

# Just make testing pass
enable_testing()
