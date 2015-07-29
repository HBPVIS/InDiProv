#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "zmq.hpp"
#include <json/json.h>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void EReciever(zmq::context_t* context, int portNum);
private:
    Ui::MainWindow *ui;
    zmq::context_t* context;
};

#endif // MAINWINDOW_H
