#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer> // Include QTimer for updating frames
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Function to display the image
    void displayImage(const cv::Mat &img);

public slots:
    void updateFrame();

private:
    Ui::MainWindow *ui;
    rs2::pipeline pipe; // Pipeline for RealSense
    QTimer *timer; // Timer for updating frames



};

#endif // MAINWINDOW_H
