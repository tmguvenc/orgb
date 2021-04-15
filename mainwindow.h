#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <opencv2/core/mat.hpp>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ORGBConverter;

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
    cv::Mat orgImage;
    QPixmap pixmap;
    QImage image;

    std::unique_ptr<ORGBConverter> orgbConverter;
};
#endif // MAINWINDOW_H
