#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include <fstream>
#include <vector>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>


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

    //slot functions are called in response to a particular signal, specially for qt's widgets
public slots:
    void updateFrame();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;
    rs2::pipeline pipe; // Pipeline for RealSense
    rs2::frameset cached_data;
    QTimer *timer; // Timer for updating frames
    FrameType currentFrameType = FrameType::DEPTH; // Variable to hold the current frame type
    std::vector<cv::Mat> frameBuffer;
    bool isRecording = false; //Variable to keep track of recording status

    bool isPipeStarted = false; // New variable to keep track of pipeline status

    int frameIndex;

    bool isLoading;

    void startPipeline(); // New function to start the pipeline

    std::vector<cv::Mat> frames; // Variable to store captured frames

    void captureFrame(); // Function to capture a frame

    bool saveFrames(const std::string& filename); // Function to save frames to a file

    void loadFrames(const std::string& filename); //Funcion to read saved frame files.

    void handleMouseMoveOnImageLabel(const QPoint &pos);

    const int BUFFER_SIZE = 50; // Adjust based on your system's capabilities


private slots:
    void updateFrameType(int index);
    void on_recordButton_toggled(bool checked);
    void on_loadButton_toggled(bool checked);
};


#endif // MAINWINDOW_H
