# AirSim Modification Notes

## Instruction of use

Follow the offical steps to catkin make it
https://microsoft.github.io/AirSim/airsim_ros_pkgs/

use this command to launch wrapper, which package all sensors' output from airsim

roslaunch airsim_ros_pkgs airsim_node.launch

### parameter in airsim_node.launch

- use_ros_time: ture is using ros simulation time, false is using airsim internal time.

- isENU_: true is output with East-North-UP frame, which is similiar with gazebo and other projects. The default frame in airsim is North-East-Down.

- world_frame_id: need to align with other navigation module.

- use_tf: false is not publish tf message. The default it true, but there are some issues when publishing tf.

- update_airsim_img_response_every_n_sec: control the topic frequency of camera ouput

- update_lidar_every_n_sec: control the topic frequency of lidar output

- update_airsim_control_every_n_sec: control all other topic frequency in this wrapper

## Summary of Changes

- Modified the NED to ENU coordinate conversion:
  - `pose` conversion is correct.
  - `orientation` conversion has been modified.
- Implemented a more complex transformation for LiDAR data during ENU conversion:
  - Two different cases are handled separately for sensor frame and world frame.
- Added a new variable to store the LiDAR settings.
- Added a `use_tf` variable to enable/disable publishing TF transforms.
- Updated the timestamp handling for LiDAR messages.
- Need to further verify whether the timestamp settings across the entire wrapper are consistent.
- Added a `use_ros_time` variable:
  - Allows choosing between ROS time and AirSim time for timestamps.
  - Currently, if AirSim time goes with natural speed, there is no issue.
  - If the AirSim simulation speed differs from real-time, potential problems may occur.
- **Note:** Time synchronization for TF-related data has **not** been modified yet.

## TODO
- need to check the correctness of the conversion for odometry, (twist)
- Complete ENU coordinate conversion for the TF part.
- Handle IMU coordinate transformation under ENU:
  - Currently, large angular velocity changes can cause unstable behavior.

