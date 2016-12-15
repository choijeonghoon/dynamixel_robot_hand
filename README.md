# dynamixel_robot_hand

# 1. workspace 만들기 (참조 :http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment)

# 2. "my_dynamixel_tutorial" 폴더를 src 안에 복사

# 3. src폴더에서 " $ git clone https://github.com/arebgun/dynamixel_motor.git " 를 하여 메타패키지 받기

# 4. 모터를 컴퓨터와 전원공급장치에 연결
#   (제공하는 패키지에서는 아래와 같은 조건을 가정으로 프로그래밍 되어있다.
#	 - motor model : RX_28
#	 - baud rate : 57142
#	 - a number of motors : 3 [id 1,2,3]
#   )

# 5." $ roslaunch my_dynamixel_tutorial controller_manager.launch " 를 하여 모터의 연결을 확인한다.

# 6. 새 터미널 창에 " $ roslaunch my_dynamixel_tutorial start_multipos_controller.launch " 를 실행한다.

# 7. " $ rosrun my_dynamixel_tutorial rolling " 을 실행시켜서 rolling.cpp 파일의 작동을 확인한다.
   
