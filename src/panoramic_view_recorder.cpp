// Copyright 1996-2023 Cyberbotics Ltd.
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

#include <signal.h>
#include "ros/ros.h"
#include "tf/tf.h"

#include <webots_ros/get_bool.h>
#include <webots_ros/get_int.h>
#include <webots_ros/get_uint64.h>
#include <webots_ros/set_int.h>

#include <webots_ros/field_get_name.h>
#include <webots_ros/field_get_node.h>
#include <webots_ros/field_get_rotation.h>
#include <webots_ros/field_get_vec3f.h>
#include <webots_ros/field_set_rotation.h>
#include <webots_ros/field_set_vec3f.h>
#include <webots_ros/node_get_field.h>
#include <webots_ros/supervisor_movie_start_recording.h>
#include <webots_ros/supervisor_set_label.h>

#include <std_msgs/String.h>

#define TIME_STEP 32

ros::ServiceClient timeStepClient;
webots_ros::set_int timeStepSrv;

void quit(int sig) {
  timeStepSrv.request.value = 0;
  timeStepClient.call(timeStepSrv);
  ROS_INFO("User stopped the 'panoramic_view_recorder' node.");
  ros::shutdown();
  exit(0);
}

int main(int argc, char **argv) {
  std::string controllerName;

  // create a node named 'panoramic_view_recorder' on ROS network
  ros::init(argc, argv, "panoramic_view_recorder", ros::init_options::AnonymousName);
  ros::NodeHandle n;

  signal(SIGINT, quit);

  // Wait for the `ros` controller.
  ros::service::waitForService("/robot/time_step");
  ros::service::waitForService("/supervisor/field/set_rotation");
  ros::spinOnce();

  timeStepClient = n.serviceClient<webots_ros::set_int>("/robot/time_step");
  timeStepSrv.request.value = TIME_STEP;

  ROS_INFO("Welcome to the panoramic_view_recorder example.");
  ROS_INFO("This node will connect to a supervisor in Webots and access world to record a panoramic movie around a nao robot.");

  // get world node
  ros::ServiceClient getWorldClient = n.serviceClient<webots_ros::get_uint64>("/supervisor/get_root");
  webots_ros::get_uint64 getWorldSrv;
  getWorldClient.call(getWorldSrv);

  // get children list
  ros::ServiceClient getWorldChildrenClient = n.serviceClient<webots_ros::node_get_field>("/supervisor/node/get_field");
  webots_ros::node_get_field getWorldChildrenSrv;
  getWorldChildrenSrv.request.node = getWorldSrv.response.value;
  getWorldChildrenSrv.request.fieldName = "children";
  getWorldChildrenClient.call(getWorldChildrenSrv);

  // get POV(Point Of View) node
  ros::ServiceClient getPovClient = n.serviceClient<webots_ros::field_get_node>("/supervisor/field/get_node");
  webots_ros::field_get_node getPovSrv;
  getPovSrv.request.field = getWorldChildrenSrv.response.field;
  getPovSrv.request.index = 1;
  getPovClient.call(getPovSrv);

  // get POV position and orientation
  ros::ServiceClient getPovPositionFieldClient = n.serviceClient<webots_ros::node_get_field>("/supervisor/node/get_field");
  webots_ros::node_get_field getPovPositionFieldSrv;
  getPovPositionFieldSrv.request.node = getPovSrv.response.node;
  getPovPositionFieldSrv.request.fieldName = "position";
  getPovPositionFieldClient.call(getPovPositionFieldSrv);

  ros::ServiceClient getPositionClient = n.serviceClient<webots_ros::field_get_vec3f>("/supervisor/field/get_vec3f");
  webots_ros::field_get_vec3f getPovPositionSrv;
  getPovPositionSrv.request.field = getPovPositionFieldSrv.response.field;
  getPositionClient.call(getPovPositionSrv);
  geometry_msgs::Vector3 initialPovPosition = getPovPositionSrv.response.value;

  ros::ServiceClient getPovOrientationFieldClient = n.serviceClient<webots_ros::node_get_field>("/supervisor/node/get_field");
  webots_ros::node_get_field getPovOrientationFieldSrv;
  getPovOrientationFieldSrv.request.node = getPovSrv.response.node;
  getPovOrientationFieldSrv.request.fieldName = "orientation";
  getPovOrientationFieldClient.call(getPovOrientationFieldSrv);

  ros::ServiceClient getPovOrientationClient =
    n.serviceClient<webots_ros::field_get_rotation>("/supervisor/field/get_rotation");
  webots_ros::field_get_rotation getPovOrientationSrv;
  getPovOrientationSrv.request.field = getPovOrientationFieldSrv.response.field;
  getPovOrientationClient.call(getPovOrientationSrv);
  geometry_msgs::Quaternion povOrientation = getPovOrientationSrv.response.value;

  ROS_INFO("Initial position of point of view is %f %f %f and its orientation is %f %f %f %f.", initialPovPosition.x,
           initialPovPosition.y, initialPovPosition.z, povOrientation.x, povOrientation.y, povOrientation.z, povOrientation.w);

  timeStepClient.call(timeStepSrv);

  // initialize services to update POV position and orientation
  ros::ServiceClient setPositionClient = n.serviceClient<webots_ros::field_set_vec3f>("/supervisor/field/set_vec3f");
  webots_ros::field_set_vec3f setPovPositionSrv;
  setPovPositionSrv.request.field = getPovPositionFieldSrv.response.field;
  setPovPositionSrv.request.value = initialPovPosition;

  ros::ServiceClient setPovOrientationClient =
    n.serviceClient<webots_ros::field_set_rotation>("/supervisor/field/set_rotation");
  webots_ros::field_set_rotation setPovOrientationSrv;
  setPovOrientationSrv.request.field = getPovOrientationFieldSrv.response.field;
  setPovOrientationSrv.request.value = povOrientation;

  //  set label to 'Recording'
  ros::ServiceClient supervisorSetLabelClient;
  webots_ros::supervisor_set_label supervisorSetLabelSrv;
  supervisorSetLabelClient = n.serviceClient<webots_ros::supervisor_set_label>("/supervisor/set_label");

  supervisorSetLabelSrv.request.id = 1;
  supervisorSetLabelSrv.request.label = "Recording";
  supervisorSetLabelSrv.request.xpos = 0.02;
  supervisorSetLabelSrv.request.ypos = 0.1;
  supervisorSetLabelSrv.request.size = 0.2;
  supervisorSetLabelSrv.request.color = 0XFF0000;
  supervisorSetLabelSrv.request.transparency = 0;
  supervisorSetLabelSrv.request.font = "Tahoma";
  if (!supervisorSetLabelClient.call(supervisorSetLabelSrv) || supervisorSetLabelSrv.response.success != 1)
    ROS_ERROR("Failed to call service set_label.");

  // start_movie
  ros::ServiceClient supervisorMovieStartClient;
  webots_ros::supervisor_movie_start_recording supervisorMovieStartSrv;
  supervisorMovieStartClient =
    n.serviceClient<webots_ros::supervisor_movie_start_recording>("/supervisor/movie_start_recording");

  supervisorMovieStartSrv.request.filename = std::string(getenv("HOME")) + std::string("/supervisor_movie.mp4");
  supervisorMovieStartSrv.request.width = 480;
  supervisorMovieStartSrv.request.height = 360;
  supervisorMovieStartSrv.request.codec = 1337;
  supervisorMovieStartSrv.request.quality = 100;
  supervisorMovieStartSrv.request.acceleration = 1;
  supervisorMovieStartSrv.request.caption = false;
  if (supervisorMovieStartClient.call(supervisorMovieStartSrv) && supervisorMovieStartSrv.response.success == 1)
    ROS_INFO("Movie started recording.");
  else
    ROS_ERROR("Failed to call service movie_start_recording.");

  double angle = -M_PI;
  while (angle < M_PI) {  // we start at angle -pi and end at angle pi
    angle += 0.05;
    tf::Quaternion q;
    q.setEuler(0, 0, angle);
    setPovOrientationSrv.request.value.w = q.getW();
    setPovOrientationSrv.request.value.x = q.getX();
    setPovOrientationSrv.request.value.y = q.getY();
    setPovOrientationSrv.request.value.z = q.getZ();
    double cosAngle = cos(angle + M_PI);
    double sinAngle = sin(angle + M_PI);
    if (setPovOrientationClient.call(setPovOrientationSrv) && setPovOrientationSrv.response.success == 1) {
      setPovPositionSrv.request.value.x = initialPovPosition.x * cosAngle;
      setPovPositionSrv.request.value.y = initialPovPosition.x * sinAngle;
      setPovPositionSrv.request.value.z = 0.5;
      if (setPositionClient.call(setPovPositionSrv) && setPovPositionSrv.response.success == 1) {
        if (!timeStepClient.call(timeStepSrv) || !timeStepSrv.response.success)
          ROS_ERROR("Couldn't update robot step.");
      } else
        ROS_ERROR("Failed to send new point of view position.");
    } else
      ROS_ERROR("Failed to send new point of view orientation.");
  }

  // stop_movie
  ros::ServiceClient supervisorMovieStopClient;
  webots_ros::get_bool supervisorMovieStopSrv;
  supervisorMovieStopClient = n.serviceClient<webots_ros::get_bool>("/supervisor/movie_stop_recording");

  if (supervisorMovieStopClient.call(supervisorMovieStopSrv) && supervisorMovieStopSrv.response.value)
    ROS_INFO("Movie stopped recording.");
  else
    ROS_ERROR("Failed to call service movie_stop_recording.");

  supervisorSetLabelSrv.request.label = "";
  supervisorSetLabelClient.call(supervisorSetLabelSrv);

  timeStepSrv.request.value = 0;
  timeStepClient.call(timeStepSrv);
}
