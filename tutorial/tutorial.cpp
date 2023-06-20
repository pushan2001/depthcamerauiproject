#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
/*This is a tutorial to get an output from the camera*/

#include "cv-helper.hpp"

//Setting the dimensions of the window
const size_t windowwidth = 480;
const size_t windowheight = 270;


using namespace rs2;
using namespace cv;

int main(){
    
    //pipeline is just an interface for the video module. 
    //For the config, you pass in the parameters for adjusting resolution, etc,
    //Then you pass it into a pipeline, and the pipeline is what allows you extract
    // access to those frames and other things that you need
    rs2::config cfg;
    pipeline pipe; 

    //which frame, width, height, what format, framerate
    //you can get the info from realsense viewer
    cfg.enable_stream(RS2_STREAM_INFRARED, 848, 480, RS2_FORMAT_Y8, 30 );
    auto config = pipe.start(cfg);
    rs2::align align_to(RS2_STREAM_DEPTH);

    //opeing a opencv window
    namedWindow("Tutorial Hello", WINDOW_AUTOSIZE);

   //while loop so that we get continuous input
    while(true){
        
        auto data = pipe.wait_for_frames();
        //we are calling the align_to and callling the process method on that
        data = align_to.process(data);

        auto ir_frame = data.get_infrared_frame();

        //opencv matrix
        Mat ir_mat;
        ir_mat = frame_to_mat(ir_frame);

        //show the image
        imshow("Tutorial Hello", ir_mat);

        if(waitKey(1) >= 0){
            break;
        }

    }
}
