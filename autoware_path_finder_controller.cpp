#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "nav_msgs/msg/path.hpp"
#include "std_msgs/msg/header.hpp"
#include "lgsvl_msgs/msg/vehicle_control_data.hpp"
#include <cmath>
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/point.hpp"
#include <sstream>
#include "geometry_msgs/msg/pose_stamped.h"
#include <rclcpp/qos.hpp>
#include <nav_msgs/msg/odometry.hpp>

#include <vector>



using namespace std::chrono_literals;
using namespace std;
using std::placeholders::_1;

 
class PathNode : public rclcpp::Node
{
 public:
 PathNode() : Node("path")
 {
    publisher_ = this->create_publisher<nav_msgs::msg::Path>("path_ang", 10);
 }
 
 private:
    void publishPose()
 {
     for(int i=0;i<180;i++)
     {
         poses->header.stamp.sec = 0;
         poses->header.stamp.nanosec = 0;
         poses->header.frame_id = "map";
         poses->pose.position.x = 15*sin(theta);
         poses->pose.position.y = 7.5*sin(theta++);
         poses->pose.position.z = 0;
         path_->poses.push_back(*poses);
     }
     path_->header.frame_id = "map";
     path_->header.stamp.sec = 0;
     path_->header.stamp.nanosec = 0;
     publisher_->publish(*path_);
 }
 rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publisher_;
 geometry_msgs::msg::PoseStamped::SharedPtr poses;
 nav_msgs::msg::Path::SharedPtr path_;
 int8_t theta;
};

int main(int argc, char *argv[])
{
 rclcpp::init(argc, argv);
 rclcpp::spin(std::make_shared<PathNode>());
 rclcpp::shutdown();
 return 0;
}