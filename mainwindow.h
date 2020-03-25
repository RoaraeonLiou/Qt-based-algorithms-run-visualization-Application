#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hanoi_window.h"
#include "huffman_window.h"
#include "Chessboard_lxy/chessboard_window.h"
#include "Chessboard_xyr/chessboard_window_2.h"
#include "sort_window.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 各题目窗口对象
    Hanoi_Window * hanoi_window = nullptr;
    Huffman_Window * huffman_window = nullptr;
    Chessboard_Window * chessboard_window = nullptr;
    Chessboard_Window_2 * chessboard_window_2 = nullptr;
    Sort_Window * sort_window = nullptr;

private:
    Ui::MainWindow *ui;
    void generate_connect();
};

#endif // MAINWINDOW_H
