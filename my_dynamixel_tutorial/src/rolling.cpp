//
#include "rosnamu.h"
#include "std_msgs/Float64.h"
#include "dynamixel_msgs/JointState.h"
#include "dynamixel_controllers/SetTorqueLimit.h"
#include "dynamixel_controllers/SetSpeed.h"
#include <sstream>
#include <cmath>
#include <stdio.h>

#define USE_MATH_DEFINES
#define _MAX_MOTORS 3
#define _MAX_TORQUE 0.7f
#define _MAX_SPEED 1.0f
#define _LOOP_RATE 50
#define _MIN_MOTOR_ID 1
#define _INIT_POS 2.6f
using namespace std;

class exampleRos : public rosNamu::Rosel {

	private :
		ros::ServiceClient motor_torque_controllers[_MAX_MOTORS];
		ros::ServiceClient motor_speed_controllers[_MAX_MOTORS];
		ros::Publisher motor_pubs[_MAX_MOTORS];
		virtual int begin() { 
			setSpeed(_MAX_SPEED);
			setTorque(_MAX_TORQUE);
			return 1;
		}

		virtual int end() {
	   		//setSpeed(0.0f);
			//setTorque(0.0f);
			return 1;
		}
		virtual int process() {

			ROS_INFO("Hello from ROS!");
			std_msgs::Float64 msg;
			for (int a =1; a<30; a++) {
			    for (int i =_MIN_MOTOR_ID;i<_MAX_MOTORS + _MIN_MOTOR_ID;i++) {
			        if (i%2==0)
			            msg.data = 3.0f;
			        else
    			        msg.data = 2.2f;
    				motor_pubs[i - _MIN_MOTOR_ID].publish(msg);
	    		}
	    	    ros::spinOnce();
       		    loop_rate->sleep();
	    	}
			ROS_INFO("Main STart!");
			return 0;
		}

		int setSpeed(double speed) {
			dynamixel_controllers::SetSpeed speed_srv;
    			speed_srv.request.speed = speed;
	    		for (int i =_MIN_MOTOR_ID;i<_MAX_MOTORS + _MIN_MOTOR_ID;i++) {
        			if(!motor_speed_controllers[i - _MIN_MOTOR_ID].call(speed_srv)) {
        		            ROS_ERROR("service call failed! : SPEED_LIMIT");
        		            return -1;
        			}
    			}
			return 1;
		}

		int setTorque(double torque) {
			dynamixel_controllers::SetTorqueLimit torque_srv;
		        torque_srv.request.torque_limit = torque;
   			for (int i =_MIN_MOTOR_ID;i<_MAX_MOTORS + _MIN_MOTOR_ID;i++) {
				//ROS_INFO("%d",i);
        			if(!motor_torque_controllers[i - _MIN_MOTOR_ID].call(torque_srv)) {
	            		ROS_ERROR("service call failed! : TORQUE_LIMIT");
        	    		return -1;
        			}
    			}
			return 1;
		}
	public :
		int init() {
			
			for (int i = _MIN_MOTOR_ID;i<_MAX_MOTORS + _MIN_MOTOR_ID;i++) {
				string str;
				char tmp[2];
				sprintf(tmp,"%d",i);
				str = "/joint";
				str += tmp;
				str += "_pos_controller/set_torque_limit";
				motor_torque_controllers[i - _MIN_MOTOR_ID] = n->serviceClient<dynamixel_controllers::SetTorqueLimit>(str);  
			}
			setTorque(_MAX_TORQUE);
			for (int i = _MIN_MOTOR_ID;i<_MAX_MOTORS + _MIN_MOTOR_ID;i++) {
        			string str;
	        		char tmp[2];
        			sprintf(tmp,"%d",i);
        			str = "/joint";
        			str += tmp;
        			str += "_pos_controller/set_speed";
        			motor_speed_controllers[i - _MIN_MOTOR_ID] = n->serviceClient<dynamixel_controllers::SetSpeed>(str);
			}
			setSpeed(_MAX_SPEED);
			for (int i = _MIN_MOTOR_ID; i <_MAX_MOTORS + _MIN_MOTOR_ID;i++) {
        			string str;
      			  	char tmp[2];
        			sprintf(tmp,"%d",i);
        			str = "/joint";
        			str += tmp;
        			str += "_pos_controller/command";
        			motor_pubs[i - _MIN_MOTOR_ID] = n->advertise<std_msgs::Float64>(str,100); // max 100 of queue.
			}
			return 1;
		}

		exampleRos(int argc, char **argv, std::string node_name) :
			rosNamu::Rosel(argc, argv, node_name) {
			}

		int motorAlign(int cnt) {
			setSpeed(_MAX_SPEED);
			setTorque(_MAX_TORQUE);
			ROS_INFO("Position Alignment...");
    
   		 	std_msgs::Float64 msg;
  			double curr_pos = M_PI, adder = 0.0025f, max_pos = 0.5f;
  		  	int test_cnt = 0;
   		 	while(ros::ok()) {
      			msg.data = _INIT_POS;
       			for (int i =_MIN_MOTOR_ID;i<_MAX_MOTORS + _MIN_MOTOR_ID;i++)
       		    	motor_pubs[i - _MIN_MOTOR_ID].publish(msg);
       	 		ros::spinOnce();
       			loop_rate->sleep();
                break;
			}
			return 0;
		}
};

int main(int argc, char **argv) {
	exampleRos *test;
	test = new exampleRos(argc, argv, "test");
	int select = 0;
	int (exampleRos::*func)(int);
	func = &exampleRos::motorAlign;

	test->set(_LOOP_RATE);
	test->init();

	//test->run(test->*func);

	while (1) {
		printf("선택 1 - Initial, 2 - Grip, 3 - Exit : ");
		scanf("%d",&select);
		if (select == 1) {
			test->motorAlign(0);
		}
		else if (select == 2)
			test->run();
		else if (select == 3){
			test->~exampleRos();
			test = NULL;
			break;
		}
		else printf("범위내의 숫자를 입력하세요.\n");
	}
	return 0;

}
