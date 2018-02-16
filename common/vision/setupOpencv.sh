sudo apt-get update

sudo apt-get -y install build-essential cmake cmake-curses-gui pkg-config
sudo apt-get -y install libjpeg-dev libtiff5-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libeigen3-dev libxvidcore-dev libx264-dev libgtk2.0-dev
sudo apt-get -y install libv4l-dev v4l-utils
sudo modprobe bcm2835-v4l2
sudo apt-get -y install libatlas-base-dev gfortran

wget -nc https://github.com/opencv/opencv/archive/3.2.0.zip -O opencv_source.zip
rm -rf opencv-3.2.0
unzip opencv_source.zip
cd opencv-3.2.0
mkdir build
cd build

cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D BUILD_WITH_DEBUG_INFO=OFF \
    -D BUILD_DOCS=OFF \
    -D BUILD_EXAMPLES=OFF \
    -D BUILD_TESTS=OFF \
    -D BUILD_opencv_ts=OFF \
    -D BUILD_PERF_TESTS=OFF \
    -D INSTALL_C_EXAMPLES=OFF \
    -D INSTALL_PYTHON_EXAMPLES=OFF \
    -D ENABLE_NEON=ON \
    -D WITH_LIBV4L=ON \
        ../

make -j4
sudo make install
sudo ldconfig
