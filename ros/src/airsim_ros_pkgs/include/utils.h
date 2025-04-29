#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <ros/ros.h>

namespace utils
{
double get_yaw_from_quat_msg(const geometry_msgs::Quaternion& quat_msg)
{
    tf2::Quaternion quat_tf;
    double roll, pitch, yaw;
    tf2::fromMsg(quat_msg, quat_tf);
    tf2::Matrix3x3(quat_tf).getRPY(roll, pitch, yaw);
    return yaw;
}

// OdometryEuler get_eigen_odom_from_rosmsg(const nav::msgs &odom_msg);

/**
 * @brief Transform a pose from NED (North-East-Down) to ENU (East-North-Up) coordinate system
 * @param pose_ned Pose in NED frame
 * @return Transformed pose in ENU frame
 */
geometry_msgs::Pose transform_pose_ned_to_enu(const geometry_msgs::Pose& pose_ned)
{
    geometry_msgs::Pose pose_enu;
    
    // Transform position
    pose_enu.position.x = pose_ned.position.y;    // NED.y -> ENU.x
    pose_enu.position.y = pose_ned.position.x;    // NED.x -> ENU.y
    pose_enu.position.z = -pose_ned.position.z;   // NED.z -> -ENU.z

    // Transform orientation
    tf2::Quaternion q_ned;
    tf2::fromMsg(pose_ned.orientation, q_ned);

    // Convert quaternion to Euler angles
    double roll, pitch, yaw;
    tf2::Matrix3x3(q_ned).getRPY(roll, pitch, yaw);
    
    // Adjust Euler angles for ENU:
    // - roll remains unchanged
    // - pitch is negated
    // - yaw becomes pi/2 - yaw
    double roll_enu = roll;
    double pitch_enu = -pitch;
    double yaw_enu = M_PI_2 - yaw;
    
    // Convert back to quaternion
    tf2::Quaternion q_enu;
    q_enu.setRPY(roll_enu, pitch_enu, yaw_enu);

    pose_enu.orientation = tf2::toMsg(q_enu);

    return pose_enu;
}

/**
 * @brief Transform orientation quaternion from NED to ENU coordinate system
 * @param orientation_ned Orientation in NED frame
 * @return Transformed orientation in ENU frame
 */
geometry_msgs::Quaternion transform_orientation_ned_to_enu(const geometry_msgs::Quaternion& orientation_ned)
{
    tf2::Quaternion q_ned;
    tf2::fromMsg(orientation_ned, q_ned);
    
    // Convert quaternion to Euler angles
    double roll, pitch, yaw;
    tf2::Matrix3x3(q_ned).getRPY(roll, pitch, yaw);
    
    // Adjust Euler angles for ENU:
    // - roll remains unchanged
    // - pitch is negated
    // - yaw becomes pi/2 - yaw
    double roll_enu = roll;
    double pitch_enu = -pitch;
    double yaw_enu = M_PI_2 - yaw;
    
    // Convert back to quaternion
    tf2::Quaternion q_enu;
    q_enu.setRPY(roll_enu, pitch_enu, yaw_enu);
    
    // // Log the conversion results (similar to Python rospy.loginfo)
    // ROS_DEBUG_STREAM("q_enu: w=" << q_enu.w() << ", x=" << q_enu.x() 
    //               << ", y=" << q_enu.y() << ", z=" << q_enu.z());
    // ROS_DEBUG_STREAM("q_ned: w=" << orientation_ned.w << ", x=" << orientation_ned.x 
    //               << ", y=" << orientation_ned.y << ", z=" << orientation_ned.z);
    
    return tf2::toMsg(q_enu);
}

/**
 * @brief Transform twist from NED (North-East-Down) to ENU (East-North-Up) coordinate system
 * @param twist_ned Twist in NED frame
 * @return Transformed twist in ENU frame
 */
geometry_msgs::Twist transform_twist_ned_to_enu(const geometry_msgs::Twist& twist_ned)
{
    geometry_msgs::Twist twist_enu;
    
    // Transform linear velocity
    twist_enu.linear.x = twist_ned.linear.y;      // NED.y -> ENU.x
    twist_enu.linear.y = twist_ned.linear.x;      // NED.x -> ENU.y
    twist_enu.linear.z = -twist_ned.linear.z;     // NED.z -> -ENU.z
    
    // Transform angular velocity
    twist_enu.angular.x = twist_ned.angular.x;   
    twist_enu.angular.y = -twist_ned.angular.y;   
    twist_enu.angular.z = -twist_ned.angular.z;    
    
    return twist_enu;
}
}