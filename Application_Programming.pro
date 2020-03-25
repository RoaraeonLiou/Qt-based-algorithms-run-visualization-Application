#-------------------------------------------------
#
# Project created by QtCreator 2020-02-26T17:38:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Application_Programming
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += \
        ./Hanoi \
        ./Chessboard_lxy \
        ./Huffman \
        ./Minimum_Spanning_Tree \
        ./Chessboard_xyr \
        ./BubbleSort \

SOURCES += \
        # 汉诺塔题目文件
        BubbleSort/bubble_sort.cpp \
        BubbleSort/sort_window.cpp \
        Hanoi/hanoi_window.cpp \
        Hanoi/hanoi.cpp \
        Hanoi/displaywindow.cpp \
        Hanoi/pillar.cpp \
        Hanoi/plate.cpp \
        # 哈夫曼题目文件
        Huffman/huffman_window.cpp \
        Huffman/huffman.cpp \
        Huffman/node.cpp \
        # 棋盘覆盖题目文件
        Chessboard_lxy/chessboard_window.cpp \
        Chessboard_lxy/chessboard.cpp \
        Chessboard_lxy/block.cpp \
        Chessboard_xyr/algorithm.cpp \
        Chessboard_xyr/chessboard_window_2.cpp \
        # 主窗口文件
        imgbutton.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        # 汉诺塔题目文件
        BubbleSort/bubble_sort.h \
        BubbleSort/sort_window.h \
        Hanoi/hanoi_window.h \
        Hanoi/hanoi.h \
        Hanoi/displaywindow.h \
        Hanoi/pillar.h \
        Hanoi/plate.h \
        # 哈夫曼题目文件
        Huffman/huffman_window.h \
        Huffman/huffman.h \
        Huffman/node.h \
        # 棋盘覆盖题目文件
        Chessboard_lxy/chessboard_window.h \
        Chessboard_lxy/chessboard.h \
        Chessboard_lxy/block.h \
        Chessboard_xyr/algorithm.h \
        Chessboard_xyr/chessboard_window_2.h \
        # 主窗口文件
        imgbutton.h \
        mainwindow.h

FORMS += \
        BubbleSort/sort_window.ui \
        Chessboard_lxy/chessboard_window.ui \
        Chessboard_xyr/chessboard_window_2.ui \
        Hanoi/hanoi_window.ui \
        Huffman/huffman_window.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
