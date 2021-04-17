#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <opencv2/core/mat.hpp>
#include <memory>
#include <QLabel>

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

    void on_vs_sf_valueChanged(int value);

private:
    void updateImage(QLabel* pb, QImage& image, const cv::Vec3d& shiftingFactor);
    void updateImages();

    Ui::MainWindow *ui;
    cv::Mat m_orgImage;
    QPixmap m_pixmap;
    QImage m_image[3][3];

    std::unique_ptr<ORGBConverter> m_orgbConverter;

    double m_lumaShift = 0.0;
    double m_ybShift = 0.0;
    double m_rgShift = 0.0;

    QLabel *m_grid[3][3];
    double m_shiftFactor = 0.3;
};
#endif // MAINWINDOW_H
