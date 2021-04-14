#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <opencv2/core/mat.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_load_img_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat original_img;
    QPixmap pixmap;
    QImage image;
};
#endif // MAINWINDOW_H
