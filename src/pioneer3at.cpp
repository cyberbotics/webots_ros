// Copyright 1996-2020 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * To run gmapping you should start gmapping:
 * rosrun gmapping slam_gmapping scan:=/pioneer3at/Sick_LMS_291/laser_scan/layer0 _xmax:=30 _xmin:=-30 _ymax:=30 _ymin:=-30
 * _delta:=0.2
 */

#include <sensor_msgs/Image.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/PointStamped.h>
#include <signal.h>
#include <std_msgs/String.h>
#include <tf/transform_broadcaster.h>
#include "ros/ros.h"

#include <webots_ros/set_float.h>
#include <webots_ros/set_int.h>

#define TIME_STEP 32

ros::NodeHandle *n;

ros::ServiceClient timeStepClient;
webots_ros::set_int timeStepSrv;

void nullFuncion(const std_msgs::String::ConstPtr &name){};

int main(int argc, char **argv)
{
  std::string controllerName;
  // create a node named 'pioneer3at' on ROS network
  ros::init(argc, argv, "pioneer3at", ros::init_options::AnonymousName);
  n = new ros::NodeHandle;
  ros::Subscriber nameSub = n->subscribe("model_name", 100, nullFuncion);
  while (nameSub.getNumPublishers() < 1)
    ros::spinOnce();
  timeStepClient = n->serviceClient<webots_ros::set_int>("pioneer3at/robot/time_step");
  timeStepSrv.request.value = TIME_STEP;
  ROS_INFO("You can now start the creation of the map using 'rosrun gmapping slam_gmapping "
           "scan:=/pioneer3at/Sick_LMS_291/laser_scan/layer0 _xmax:=30 _xmin:=-30 _ymax:=30 _ymin:=-30 _delta:=0.2'.");
  ROS_INFO("You can now visualize the sensors output in rqt using 'rqt'.");

  // main loop
  while (ros::ok())
  {
    if (!timeStepClient.call(timeStepSrv) || !timeStepSrv.response.success)
    {
      ROS_ERROR("Failed to call service time_step for next step.");
      break;
    }
    ros::spinOnce();
  }
  timeStepSrv.request.value = 0;
  timeStepClient.call(timeStepSrv);

  ros::shutdown();
  return 0;
}
