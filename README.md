# to_zero
export ROS_NAMESPACE=camera

roslaunch zed_wrapper zed_camera.launch publish_tf:=false

rosrun tf static_transform_publisher 0 0 0 -1.5707963267948966 0 -1.5707963267948966 camera_link zed_center 100


roslaunch rtabmap_ros rtabmap.launch rtabmap_args:="--delete_db_on_start --Vis/CorFlowMaxLevel 5 --Stereo/MaxDisparity 200" right_image_topic:=/stereo_camera/right/image_rect_color stereo:=true


rostopic pub /rtabmap/setgoal geometry_msgs/PoseStamped '{header: {stamp: now, frame_id: "map"}, pose: {position: {x: 0.0, y: 0.0, z: 0.0}, orientation: {w: 1.0}}}'




http://wiki.ros.org/rtabmap_ros/Tutorials/StereoOutdoorMapping
http://wiki.ros.org/rtabmap_ros/Tutorials/StereoHandHeldMapping
https://answers.ros.org/question/47973/publishing-to-move_base_simplegoal/
