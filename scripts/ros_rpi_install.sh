#!/bin/sh
#Script to install ROS Melodic on Raspbian Buster.
#This might not work for other ROS or Raspbian version.
ROS_VER="melodic"
ROS_DESKTOP="Y"

#Size of the swap in megabytes to set to prevent compilation issues
SWAP=500

sudo sed -i -e "s/\(CONF_SWAPSIZE=\).*/\1$SWAP/" /etc/dphys-swapfile
sudo /etc/init.d/dphys-swapfile restart

sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654

sudo apt-get update
sudo apt-get -y upgrade
sudo apt-get -y install dirmngr python-rosdep python-rosinstall-generator python-wstool python-rosinstall build-essential cmake

sudo rosdep init
rosdep update

mkdir -p ~/ros_catkin_ws
cd ~/ros_catkin_ws

if [ $ROS_DESKTOP = "Y" ]; then
  #Desktop install
  sudo apt-get -y install pyqt5-dev #Dependencies that are not automaticaly installed
  rosinstall_generator desktop --rosdistro ${ROS_VER} --deps --wet-only --tar \
    > "${ROS_VER}-desktop-wet.rosinstall"
  wstool init src ${ROS_VER}-desktop-wet.rosinstall
else
  #ROS-Comm no GUI install
  rosinstall_generator ros_comm --rosdistro ${ROS_VER} --deps --wet-only --tar > "${ROS_VER}-ros_comm-wet.rosinstall"
  wstool init src "${ROS_VER}-ros_comm-wet.rosinstall"
fi


cd ~/ros_catkin_ws
rosdep install -y --from-paths src --ignore-src --rosdistro ${ROS_VER} -r --os=debian:buster

#Compile using only 2 threads in order to prevent compilation errors caused by memory exhaustion
sudo ./src/catkin/bin/catkin_make_isolated \
  --install -DCMAKE_BUILD_TYPE=Release --install-space /opt/ros/${ROS_VER} -j2

echo "source /opt/ros/${ROS_VER}/setup.bash" >> ~/.bashrc
