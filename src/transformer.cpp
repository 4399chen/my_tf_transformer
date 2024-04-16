#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_transformer");
  ros::NodeHandle nh;

  tf::TransformBroadcaster br;
  tf::TransformListener listener;
  tf::StampedTransform transform;

  ros::Rate rate(10.0);
  while (nh.ok()){
    // 等待转换变得可用
    if (listener.waitForTransform("tag_1", "tag_1", ros::Time(0), ros::Duration(3.0)))
    {
        try{
        listener.lookupTransform("tag_1", "tag_1", ros::Time(0), transform);
        }
        catch (tf::TransformException &ex) {
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
        continue;
        }

        // 四元数描述的坐标变换
        tf::Quaternion change_orientation(0.5, -0.5, -0.5, -0.5);

        // 原始转换的四元数
        tf::Quaternion original_orientation = transform.getRotation();

        // 应用坐标轴变换
        tf::Quaternion new_orientation = change_orientation * original_orientation;

        // 设置新的转换
        tf::Transform new_transform;
        new_transform.setOrigin(transform.getOrigin());
        // new_transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));  // 设置位置为0
        new_transform.setRotation(new_orientation);

        // 广播新的变换
        br.sendTransform(tf::StampedTransform(new_transform, ros::Time::now(), "tag_1", "new_tag_1"));
    }
    else
    {
        ROS_ERROR("Timeout waiting for tf between 'camera_color_frame' and 'tag_1'");
    }
    // rate.sleep();
  }
  return 0;
}
