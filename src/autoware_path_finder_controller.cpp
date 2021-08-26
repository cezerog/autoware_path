#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "nav_msgs/msg/path.hpp"
#include "std_msgs/msg/header.hpp"
#include <cmath>
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/point.hpp"
#include <sstream>
#include "geometry_msgs/msg/pose_stamped.h"
#include <rclcpp/qos.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include "geometry_msgs/msg/pose_array.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

#include <vector>



using namespace std::chrono_literals;
using namespace std;
using std::placeholders::_1;

int myint;
vector<int> myvector;
 
class PathNode : public rclcpp::Node
{
 public:
 PathNode() : Node("path")
 {
    publisher_ = this->create_publisher<nav_msgs::msg::Path>("path_ang", 10);
    publish_ = this->create_publisher<geometry_msgs::msg::PoseArray>("path_geo", 10);
    publishspace_ = this ->create_publisher<geometry_msgs::msg::Quaternion>("path_quaternion", 10);

    timer_ = this->create_wall_timer(100ms, std::bind(&PathNode::publishPose, this));
    RCLCPP_INFO(this-> get_logger(), "Path Angle Has Been Started!");
 }
 
 private:
    void publishPose()
 {
     
     geometry_msgs::msg::PoseArray path_geo1_;
     nav_msgs::msg::Path path_;
     geometry_msgs::msg::Quaternion path_quaternion1;
     for(int theta=0;theta<180;theta++)
     {
         geometry_msgs::msg::PoseStamped poses_stamped;
         poses_stamped.header.stamp = rclcpp::Node::now();
         poses_stamped.header.frame_id = "map";
         poses_stamped.pose.position.x = theta;
         poses_stamped.pose.position.y = 5*sin(0.2*theta);
         poses_stamped.pose.position.z = 0;
         path_.poses.push_back(poses_stamped);

         geometry_msgs::msg::Pose poses;
         path_geo1_.poses.push_back(poses); 
         poses.position.x = theta;
         poses.position.y = 5*sin(0.2*theta);
         poses.position.z = 0;
         path_geo1_.poses.push_back(poses);


         poses.orientation. = theta;


         
        
     }
     
     
     path_.header.stamp = rclcpp::Node::now();

     path_.header.frame_id = "map";
     publisher_->publish(path_);
     
     path_geo1_.header.frame_id = "map";
     path_geo1_.header.stamp = rclcpp::Node::now();
     publish_->publish(path_geo1_);
     
 }
 
 rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr publish_;
 rclcpp::TimerBase::SharedPtr timer_;
 rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publisher_;
 rclcpp::Publisher<geometry_msgs::msg::Quaternion>::SharedPtr publishspace_;
};

int main(int argc, char *argv[])
{
 rclcpp::init(argc, argv);
 rclcpp::spin(std::make_shared<PathNode>());
 rclcpp::shutdown();
 return 0;
}