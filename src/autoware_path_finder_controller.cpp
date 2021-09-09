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



using namespace std::chrono_literals;
using namespace std;
using std::placeholders::_1;

 
class PathNode : public rclcpp::Node
{


 public: PathNode() : Node("path")
 {
    publish_ang_ = this->create_publisher<nav_msgs::msg::Path>("path_ang", 10);
    publish_geo_ = this->create_publisher<geometry_msgs::msg::PoseArray>("path_geo", 10);

    timer_ = this->create_wall_timer(100ms, std::bind(&PathNode::publishPose, this));

    RCLCPP_INFO(this-> get_logger(), "Path Angle Has Been Started!");
 }
 
 private:
    void publishPose()
 {

     std::cout << "Inside publishPose" << std::endl;
     rclcpp::Time now;
     geometry_msgs::msg::PoseArray path_geo_;
     nav_msgs::msg::Path path_ang_;


     for(int theta=0;theta<180;theta++)
     {
         geometry_msgs::msg::PoseStamped poses_ang;
         poses_ang.header.stamp = rclcpp::Node::now();
         poses_ang.header.frame_id = "map";
         poses_ang.pose.position.x = theta;
         poses_ang.pose.position.y = 5*sin(0.2*theta);
         poses_ang.pose.position.z = 0;
         poses_ang.pose.orientation.w = 0;
         poses_ang.pose.orientation.x = 0;
         poses_ang.pose.orientation.y = 0;
         poses_ang.pose.orientation.z = 0;
         path_ang_.poses.push_back(poses_ang);

         geometry_msgs::msg::Pose poses_geo_;
         poses_geo_.position.x = theta;
         poses_geo_.position.y = 5*sin(0.2*theta);
         poses_geo_.position.z = 0;
         path_geo_.poses.push_back(poses_geo_);

     }
      std::cout << "First loop" << std::endl;

      for (int i = 0; i < path_geo_.poses.size(); i++) { 

        geometry_msgs::msg::Pose first_pose = path_geo_.poses[i];
        geometry_msgs::msg::Pose second_pose = path_geo_.poses[i+1];

        float delta_x = second_pose.position.x - first_pose.position.x;
        float delta_y = second_pose.position.y - first_pose.position.y;

        
        double yaw = std::atan2(delta_y,delta_x);     
      

         tf2::Quaternion q;
         q.setRPY(0,0,yaw);

         geometry_msgs::msg::Quaternion q_geo;

         tf2::convert(q,q_geo);

         first_pose.orientation = q_geo; 
         // don't forget to update relevant path_geo_ pose  
         path_geo_.poses[i].orientation = q_geo;
     }
     std::cout << "Second loop" << std::endl;

     path_ang_.header.frame_id = "map";
     path_ang_.header.stamp = rclcpp::Node::now();
     publish_ang_->publish(path_ang_);
     
     path_geo_.header.frame_id = "map";
     path_geo_.header.stamp = rclcpp::Node::now();
     publish_geo_->publish(path_geo_);


    } 
     
 rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr publish_geo_;
 rclcpp::TimerBase::SharedPtr timer_;
 rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publish_ang_;
 rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr publish_orientation;  
     
 };
 
 
 int main(int argc, char *argv[])
{
 rclcpp::NodeOptions options;
 options.allow_undeclared_parameters(true);
 options.automatically_declare_parameters_from_overrides(true);
 rclcpp::init(argc, argv);
 rclcpp::spin(std::make_shared<PathNode>());
 rclcpp::shutdown();
 return 0;
}