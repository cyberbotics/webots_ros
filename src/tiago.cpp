#include <tf/tf.h>
#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <string>
#include <vector>
#include <map>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "plan_arm_torso_ik");

    if (argc < 7)
    {
        ROS_INFO(" ");
        ROS_INFO("\tUsage:");
        ROS_INFO(" ");
        ROS_INFO("\trosrun tiago_moveit_tutorial plan_arm_torso_ik  x y z  r p y");
        ROS_INFO(" ");
        ROS_INFO("\twhere the list of arguments specify the target pose of /arm_tool_link expressed in /base_footprint");
        ROS_INFO(" ");
        return EXIT_FAILURE;
    }

    geometry_msgs::PoseStamped goal_pose;
    goal_pose.header.frame_id = "base_link";
    goal_pose.pose.position.x = atof(argv[1]);
    goal_pose.pose.position.y = atof(argv[2]);
    goal_pose.pose.position.z = atof(argv[3]);
    goal_pose.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(atof(argv[4]), atof(argv[5]), atof(argv[6]));

    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(1);
    spinner.start();

    std::vector<std::string> torso_arm_joint_names;
    //select group of joints
    moveit::planning_interface::MoveGroupInterface group_arm_torso("arm");
    //choose your preferred planner
    group_arm_torso.setPlannerId("SBLkConfigDefault");
    group_arm_torso.setPoseReferenceFrame("base_link");
    group_arm_torso.setPoseTarget(goal_pose);

    ROS_INFO_STREAM("Planning to move " << group_arm_torso.getEndEffectorLink() << " to a target pose expressed in " << group_arm_torso.getPlanningFrame());

    group_arm_torso.setStartStateToCurrentState();
    group_arm_torso.setMaxVelocityScalingFactor(1.0);

    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    //set maximum time to find a plan
    group_arm_torso.setPlanningTime(5.0);

    moveit::planning_interface::MoveItErrorCode success = group_arm_torso.plan(my_plan);

    if (!success)
        throw std::runtime_error("No plan found");

    ROS_INFO_STREAM("Plan found in " << my_plan.planning_time_ << " seconds");

    // Execute the plan
    ros::Time start = ros::Time::now();

    group_arm_torso.move();

    ROS_INFO_STREAM("Motion duration: " << (ros::Time::now() - start).toSec());

    spinner.stop();

    return EXIT_SUCCESS;
}
