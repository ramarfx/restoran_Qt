cmake_minimum_required(VERSION 3.16)

project(kasir_kafe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)


# PERUBAHAN 1: Memuat modul Core, Gui, dan Widgets (bukan Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup(REQUIRES 6.10)


# Pastikan file main.cpp masuk ke dalam executable
qt_add_executable(appkasir_kafe
    main.cpp
    restaurantapp.cpp
    restaurantapp.h
    models.h

    appkasir_kafe.qrc
    styles.cpp
    algorithms.cpp
    algorithms.h
    pages/page_home.cpp
    pages/page_consumer.cpp
    pages/page_cart.cpp
    pages/page_login.cpp
    pages/page_cashier.cpp
    pages/page_kitchen.cpp
    pages/page_waiter.cpp

)

qt_add_resources(appkasir_kafe appkasir_kafe.qrc)

# (Bagian qt_add_qml_module dihapus karena kita tidak memakai Main.qml)

# Konfigurasi standar untuk build window/mac
set_target_properties(appkasir_kafe PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# PERUBAHAN 2: Menghubungkan (link) library aplikasi ke modul Widgets
target_link_libraries(appkasir_kafe
    PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets
)

target_include_directories(appkasir_kafe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS appkasir_kafe
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)