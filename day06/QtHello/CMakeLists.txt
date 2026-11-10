cmake_minimum_required(VERSION 3.19)
project(QtHello LANGUAGES CXX)

# Export the actual include paths and compiler flags for VS Code IntelliSense.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

qt_standard_project_setup()

qt_add_executable(QtHello
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    DeviceReading.h
    mainwindow.ui
    MockCollector.h
    MockCollector.cpp
    mock_tcp_server.py
    mock_tcp_test.py
    SlowWorker.h
    SlowWorker.cpp
    AlarmManager.h
    AlarmManager.cpp
    alarm_tcp_test.py
    AppConfig.h
    AppConfig.cpp
    DeviceStore.h
    DeviceStore.cpp

)

target_link_libraries(QtHello
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

# On macOS, prefer real framework headers over Qt's include_next forwarding
# headers so both Clang and VS Code IntelliSense resolve the same declarations.
if(APPLE)
    target_include_directories(QtHello BEFORE PRIVATE
        "$<TARGET_FILE_DIR:Qt6::Core>/Headers"
        "$<TARGET_FILE_DIR:Qt6::Gui>/Headers"
        "$<TARGET_FILE_DIR:Qt6::Widgets>/Headers"
    )
endif()

install(TARGETS QtHello
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QtHello
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
