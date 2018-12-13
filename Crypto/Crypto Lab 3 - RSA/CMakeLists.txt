cmake_minimum_required(VERSION 3.12)
project(Crypto_Lab_3___RSA)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

add_executable(Crypto_Lab_3___RSA main.cpp UI/mainwindow.cpp UI/mainwindow.h UI/ui_mainwindow.h RSA.h RSAText.h)

# Qt5 -- BEGIN SECTION --
set(QT_DIR /Users/so/Qt5.12.0/5.12/clang_64/lib/cmake/Qt5Core/)
set(Qt5Widgets_DIR /Users/so/Qt5.12.0/5.12/clang_64/lib/cmake/Qt5Widgets/)
set(Qt5OpenGL_DIR /Users/so/Qt5.12.0/5.12/clang_64/lib/cmake/Qt5OpenGL/)
set(Qt5Multimedia_DIR /Users/so/Qt5.12.0/5.12/clang_64/lib/cmake/Qt5Multimedia/)
set(Qt5Charts_DIR /Users/so/Qt5.12.0/5.12/clang_64/lib/cmake/Qt5Charts/)

set (CMAKE_INSTALL_PREFIX /Users/so/Qt5.12.0/5.12/clang_64/lib/cmake/Qt5Core)
set(CMAKE_PREFIX_PATH /Users/so/Qt5.12.0/5.12/clang_64/lib/cmake/Qt5Core)

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package( Qt5OpenGL REQUIRED )
find_package( Qt5Multimedia REQUIRED )
find_package( Qt5Charts REQUIRED )

target_link_libraries(Crypto_Lab_3___RSA Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Charts)

# Qt5 -- END SECTION --