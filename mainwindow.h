#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void repaint();

public slots:
    void timer1();
    void stream();
    void send_done();

private:
    bool flag;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
