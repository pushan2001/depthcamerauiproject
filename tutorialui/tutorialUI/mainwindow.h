#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class FrameType {
        DEPTH,
        COLOR,
        INFRARED
        // Add other frame types as needed
    };

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
    FrameType currentFrameType = FrameType::DEPTH; // Variable to hold the current frame type

    bool isPipeStarted = false; // New variable to keep track of pipeline status

    void startPipeline(); // New function to start the pipeline

private slots:
    void updateFrameType(int index);
};

#endif // MAINWINDOW_H
