#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <rclcpp/qos.hpp>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "nav_msgs/msg/path.hpp"
#include "std_msgs/msg/header.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "geometry_msgs/msg/pose_stamped.h"
#include <nav_msgs/msg/odometry.hpp>
#include "geometry_msgs/msg/pose_array.hpp"
#include "geometry_msgs/msg/quaternion.hpp"
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include "geometry_msgs/msg/vector3.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Vector3.h>
#include <tf2_ros/static_transform_broadcaster_node.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <shape_msgs/msg/mesh.hpp>



using namespace std::chrono_literals;
using namespace std;
using std::placeholders::_1;


class CarPublisher : public rclcpp::Node
{

 public: CarPublisher() : Node("car")
 {

    subscription_ = this->create_subscription<geometry_msgs::msg::PoseArray>("path_geo", 10, std::bind(&CarPublisher::publishPose, this, _1));
    publish_marker_ = this->create_publisher<visualization_msgs::msg::Marker>("path_rectangle_", 10);
    RCLCPP_INFO(this-> get_logger(), "Car Publishing Has Been Started!");
    
 }
 
 private:

   int i = 0;

    void publishPose(const geometry_msgs::msg::PoseArray::SharedPtr msg)
 {

    std::cout << "Inside publishPose" << std::endl;
    rclcpp::Time now;
    visualization_msgs::msg::Marker poses_rectangle_;
    geometry_msgs::msg::Pose poses_geo_;


    if( i < msg->poses.size()){ 


        poses_geo_= msg->poses[i];
        geometry_msgs::msg::Pose first_pose = msg->poses[i];
        geometry_msgs::msg::Pose second_pose = msg->poses[i+1];
        poses_rectangle_.pose.position.x = poses_geo_.position.x+2;
        poses_rectangle_.pose.position.y = poses_geo_.position.y;
        poses_rectangle_.pose.position.z = poses_geo_.position.z;
        poses_rectangle_.pose.orientation = poses_geo_.orientation;

         cout << poses_geo_.position.x+2 << "\n";
         cout << poses_geo_.position.y << "\n";

        poses_rectangle_.type = visualization_msgs::msg::Marker::CUBE;
        poses_rectangle_.action = visualization_msgs::msg::Marker::ADD;

        poses_rectangle_.header.frame_id = "map";
        poses_rectangle_.header.stamp = rclcpp::Node::now();
          
         
        poses_rectangle_.color.a = 1;
        poses_rectangle_.color.g = 1;
        poses_rectangle_.color.r = poses_rectangle_.color.b = 0;
        poses_rectangle_.scale.x = 8.5;
        poses_rectangle_.scale.y = 2.5;
        poses_rectangle_.scale.z = 3;

        publish_marker_->publish(poses_rectangle_);      
        i++;
    }
    

    } 


rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr publish_marker_;
rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr subscription_;
     
 };
 
 
 int main(int argc, char *argv[])
{
 rclcpp::NodeOptions options;
 options.allow_undeclared_parameters(true);
 options.automatically_declare_parameters_from_overrides(true);
 rclcpp::init(argc, argv);
 rclcpp::spin(std::make_shared<CarPublisher>());
 rclcpp::shutdown();
 return 0;
}