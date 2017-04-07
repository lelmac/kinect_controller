#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Empty.h>
#include <nav_msgs/Odometry.h>
#include <ardrone_autonomy/Navdata.h>
#include <geometry_msgs/Twist.h>
#include <cstdint>

class Drone_controller {
private:
	bool landed;
	int drone_state;
	ros::NodeHandle nodeHandle;
	ros::Subscriber navdataSubscriber;
	ros::Publisher commandPublisher;
	geometry_msgs::Twist command;
public:
	Drone_controller(ros::NodeHandle);
	void takeoff();
	void land();
	void reset();
	void navdataCallback(const ardrone_autonomy::Navdata& msg_in);
	void setCommand(float roll, float pitch, float yaw, float z_velocity);
	void sendCommand();
};

void Drone_controller::takeoff() {
	if (drone_state == 2) { //2 == Landed
		system("rostopic pub -1 /ardrone/takeoff std_msgs/Empty");
	}
}

void Drone_controller::land() {
	system("rostopic pub -1 /ardrone/land std_msgs/Empty");
}

void Drone_controller::reset() {
	system("rostopic pub -1 /ardrone/reset std_msgs/Empty");
}

void Drone_controller::navdataCallback(
		const ardrone_autonomy::Navdata& msg_in) {
	drone_state = msg_in.state;
}
void Drone_controller::setCommand(float roll, float pitch, float yaw,
		float z_velocity) {
	command.linear.x = pitch;
	command.linear.y = roll;
	command.linear.z = z_velocity;
	command.angular.z = yaw;
}
void Drone_controller::sendCommand() {
	if (drone_state == 3 || drone_state == 4 || drone_state == 7) {
		commandPublisher.publish(command);
	}
}

Drone_controller::Drone_controller(ros::NodeHandle nh) {
	nodeHandle = nh;
	navdataSubscriber = nodeHandle.subscribe("/ardrone/navdata", 1,
			&Drone_controller::navdataCallback, this);
	commandPublisher = nodeHandle.advertise<geometry_msgs::Twist>("/cmd_vel",1);
}

int main(int argc, char **argv) {
	std_msgs::Empty emp_msg;
	ROS_INFO("Flying ARdrone");

	ros::init(argc, argv, "ARDrone_test");
	ros::NodeHandle node;

	ros::Publisher pub_empty;
	Drone_controller dc(node);

}
