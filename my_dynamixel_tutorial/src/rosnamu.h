#include "ros/ros.h"

namespace rosNamu {
	class Rosel {

		typedef int (Rosel::*FuncPtr)(int);
		private:
			
			virtual int begin() { }
			virtual int end() { }
			virtual int process() { }

		public:

			ros::Rate *loop_rate;
			ros::NodeHandle *n;
			Rosel(int argc, char **argv, std::string node_name) {
				ros::init(argc, argv, node_name);
				n = new ros::NodeHandle(node_name);
			}
			~Rosel() {
				loop_rate->~Rate();
				loop_rate = NULL;
				n->~NodeHandle();
				n = NULL;
			}
			int set(int looprate) {
				if (looprate > 0) {
					loop_rate = new ros::Rate(looprate);
					return 1;
				} else
					return 0;
			}
			void runOnce() {
				ros::ok();
				process();
				ros::spinOnce();
				loop_rate->sleep();

			}
			void run() {
				if (!begin()) return;
				while(process() && ros::ok()) {
					ros::spinOnce();
					loop_rate->sleep();
				}
				if (!end()) return;
			}
		protected:

	};
};

