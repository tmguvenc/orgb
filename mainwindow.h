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
    void on_vs_luma_valueChanged(int value);
    void on_vs_yb_valueChanged(int value);
    void on_vs_rg_valueChanged(int value);

private:
    void updateImage();

    Ui::MainWindow *ui;
    cv::Mat orgImage;
    QPixmap pixmap;
    QImage image;

    std::unique_ptr<ORGBConverter> orgbConverter;

    double m_lumaShift = 0.0;
    double m_ybShift = 0.0;
    double m_rgShift = 0.0;
};
#endif // MAINWINDOW_H
