# g++ -std=c++11 -O3  -DDLIB_PNG_SUPPORT -DDLIB_JPEG_SUPPORT -I/opt/dlib-19.16 /opt/dlib-19.16/dlib/all/source.cpp -lpthread -lX11 -ljpeg -lpng $1
cd build
# rm -rf *
cmake ..
make -j8
        # time(&start);
        # for(int i=0; i<nframes; i++)
        # 	cap >> out;
        # time(&end);
        # double seconds = difftime(end, start);
        # std::cout << "Time taken: " << seconds << "secs\n";
        # std::cout << "FPS =" << (nframes/seconds) << "\n";
        # cap.release();
        # return 0;
        # cv::line(img, lvec[0], lvec[2], RED_COLOR);
	# cv::line(img, lvec[1], lvec[3], RED_COLOR);
	# cv::line(img, lvec[4], lvec[5], RED_COLOR);
	# cv::line(img, rvec[0], rvec[2], BLUE_COLOR);
	# cv::line(img, rvec[1], rvec[3], BLUE_COLOR);
	# cv::line(img, rvec[4], rvec[5], RED_COLOR);