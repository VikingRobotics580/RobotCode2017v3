//2017 RoboCode v3.0.2 Source Code
#include <WPILib.h> // Worchester Polytechnic Institute (WPI) Library for Robot classes,
#include <stdio.h> //We use some of this for logging purposes.
#include <logging.h> //This is where we have our logging code
/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 * The Robot Load Order, after Calling START_ROBOT_CLASS, is as follows:       *
 *                                                                             *
 * Constructor -> RobotInit -> DisabledInit -> DisabledPeriodic                *
 * Once a mode changes, the function execution order looks like the following: *
 * PreviousModePeriodic -> NextModeInit -> NextModePeriodic                    *
 *                                                                             *
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */
class Robot : public IterativeRobot {
    public:
    Robot():
            IterativeRobot(), //constructor
            //These constructions are given the number of the PWM (or DIO?) port on the roboRio
            //Variables for objects are defined at the bottom, in the PRIVATE modifier.
            frontLeft(0),   //port 0
            backLeft(1),    //port 1
            frontRight(2),  //port 2
            backRight(3),   //port 3
            shooter(4),     //port 4
            gear1(5),       //port 5
            gear2(6),       //port 6
            ballReleaser(7),//port 7
        //    agitator(8),    //(ex)port 8

            lights(9),		//port 9
            clock(),
            shooterClock(),
            inputA(0), //HOPEFULLY DIO port 0
            inputB(1), //HOPEFULLY DIO port 1
            inputC(2) //HOPEFULLY DIO port 2
        {
            // You should only initialize value here. Try not to have any other
            //  executable code here.
            joy = new Joystick(0);  //Drive Joystick
            joy2 = new Joystick(1); //Shooter Joystick
            drive = NULL;
            climber = NULL;
        }
    void middle(){
    	// void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
    	if(clock.Get() <= 4.0f){
    		drive->MecanumDrive_Cartesian(0.0f, -0.50f, 0.0f, 0.0f); //range of motion is inverted from -1.0-1.0
    	} else if(clock.Get() == 14.9){
    		clock.Reset();
    	    clock.Stop();
    	} else {
    		drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //0.0 = not moving
    	}
    }

    void right(){ //field position right, toggle 001
        if (clock.Get() <= 2.6){
        drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //drive forward 50%
        } else if (clock.Get() > 2.6 && clock.Get() < 3.3){
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, -1.0f, 0.0f); //rotate 100%
        } else if (clock.Get() >= 3.3 && clock.Get() < 4.0){
            drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //drive forward 50%
        } else if (clock.Get() >= 4.0 && clock.Get() < 6.0){
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //do nothing momentarily
        } else if (clock.Get() >= 6.0 && clock.Get() < 6.5){
            drive->MecanumDrive_Cartesian(0.0f, 0.5f, 0.0f, 0.0f); //reverse 50%
        } else if (clock.Get() > 6.5 && clock.Get() < 8.0){
            drive->MecanumDrive_Cartesian(0.5f, -0.5f, 0.0f, 0.0f); //drive forward 50%
        } else if (clock.Get() >= 14.8){
        	clock.Stop(); //stop clock, signaling end of autonomous mode
        } else {
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); // do nothing
        }
    }

    void right2() { //rightmost field position and shooting at right fuel station, toggle 110

        // void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
        //to-do list - FIX TIMING AND DISTANCE INPUTS
        //ESTABLISH CLOCK TIMES

        //SETUP 2ND "RIGHT" SECNARIO STARTING FROM LEFTMOST POSITION, using hypothetical times
        if(clock.Get() >= 4.0) {
        	drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //drive forward 50% for 4 secs
        } else if(clock.Get() >= 4.1 && clock.Get() < 8.2) {
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, -5.0f, 0.0f); //turn left
        } else if(clock.Get() >= 8.3 && clock.Get() < 11.4) {
        	drive->MecanumDrive_Cartesian(-0.5f, 0.0f, 0.0f, 0.0f); //drive left 50% for 4 secs
        } else if(clock.Get() >= 11.5 && clock.Get() < 13.6) {
        	drive->MecanumDrive_Cartesian(-0.5f, -0.5f, 0.0f, 0.0f); //NEED MEASURED diagonal angle for precise shot position
        } else if(clock.Get() >= 13.7 && clock.Get() < 14.8) {
        	shooter.Set(((((joy2->GetThrottle())*-1)+1)/4)+0.25); //This puts power between 50% and 100%
            //SHOOT BALLS!!! NEED MEASURED
        } else {
        	drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);
        }

    }

    void left(){ //field position left, toggle 100
        if (clock.Get() <= 2.6){
        drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f);
        } else if (clock.Get() > 2.6 && clock.Get() < 3.3){
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, -0.5f, 0.0f);
        } else if (clock.Get() >= 3.4 && clock.Get() < 4.0){
            drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f);
        } else if (clock.Get() >= 4.1 && clock.Get() < 6.0){
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);
        } else if (clock.Get() >= 6.1 && clock.Get() < 6.5){
            drive->MecanumDrive_Cartesian(0.0f, 0.5f, 0.0f, 0.0f);
        } else if (clock.Get() >= 6.6 && clock.Get() < 8.0){
            drive->MecanumDrive_Cartesian(-0.5f, -0.5f, 0.0f, 0.0f);
        } else if (clock.Get() >= 14.8){
        	clock.Stop();
        } else {
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    void left2() { //leftmost field position and shooting at right fuel station, toggle 011

    	// void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
    	//to-do list - FIX TIMING AND DISTANCE INPUTS
    	//ESTABLISH CLOCK TIMES

    	//SETUP 2ND "LEFT" SECNARIO STARTING FROM LEFTMOST POSITION, using hypothetical times
    	if(clock.Get() >= 4.0) {
    		drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //drive forward 50% for 4 secs
    	} else if(clock.Get() >= 4.1 && clock.Get() < 8.2) {
    		drive->MecanumDrive_Cartesian(0.0f, 0.0f, -5.0f, 0.0f); //turn left by rotating
    	} else if(clock.Get() >= 8.3 && clock.Get() < 11.4) {
    		drive->MecanumDrive_Cartesian(-0.5f, 0.0f, 0.0f, 0.0f); //drive left 50% for 4 secs
    	} else if(clock.Get() >= 11.5 && clock.Get() < 13.6) {
    		drive->MecanumDrive_Cartesian(-0.5f, -0.5f, 0.0f, 0.0f); //NEED MEASURED diagonal angle for precise shot position
    	} else if(clock.Get() >= 13.7 && clock.Get() < 14.8) {
    		shooter.Set(((((joy2->GetThrottle())*-1)+1)/4)+0.25); //This puts power between 50% and 100%
    		//SHOOT BALLS!!! NEED MEASURED
    	} else {
    		drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);
    	}
    }

    void RobotInit() {
            // Initialize the Robot and all functions that need to be called.
            log_info("Initializing Robot.\n");
            drive = new RobotDrive(frontLeft, backLeft, frontRight, backRight);
            drive->SetSafetyEnabled(false); // This is dangerous, but our robot
                                            //  has some problems and needs
                                            //  this.
        }
        void AutonomousInit() {
            // Here is where you would initialize all of your autonomous code
            log_info("Initializing Autonomous mode.");
            clock.Reset(); //Resets time before starting from 0s
            clock.Start(); //Starts the timer from 0s
        }
        void AutonomousPeriodic() {
            // Here is where you would put all of your autonomous code.
            // It is important to note that all code here will run as though
            //  autonomous mode is endless, so you have to carefully time how
            //  long your code will run for.
            /*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *\
             *  APPROXIMATE VELOCITY OF ROBOT @50% POWER: 76.67 in/s OR 6.389 ft/s         *
             *  a(x) = TBD                                                                 *
             *                                                                             *
            \*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */
          if (inputB.Get()){ //010
              middle();
          } else if (inputA.Get()){ //100
              left();
          } else if(inputC.Get()){ //001
              right();
          } else if (inputA.Get() && inputC.Get()){//101
              middle();
          } else if(inputA.Get() && inputB.Get()){//110, rightmost field position
              right2();
          } else if (inputB.Get() && inputC.Get()){ //011, leftmost field position
              left2();
          } else if (inputA.Get() && inputB.Get() && inputC.Get()){ //111
              //Open channel
          } else if (clock.Get() <= 14f){
              drive->MecanumDrive_Cartesian(0.0f, -10f, 0.0f, 0.0f); //move forward 100%
          } else if(clock.Get() == 149){
              clock.Reset();
              clock.Stop();
          } else {
              drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //do nothing
          }

        }
        void TeleopInit() {
            log_info("Initializing Teleop mode.\n");
            clock.Reset(); //This is so we can reset Autonomous mode through TeleOp for testing
        }
        void TeleopPeriodic() {
            //Drive Train
            // During TeleOp we want to pass Mecanum Drive the joystick axes.
            // void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
            drive->MecanumDrive_Cartesian(joy->GetX(), joy->GetY(),
                                          joy->GetTwist(), 0.0f);

            //climber
            if(joy2->GetRawButton(3)){
            	climber->MecanumDrive_Cartesian(0.0f, joy2->GetY(), 0.0f, 0.0f);
            } else {
            	climber->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);
            }
            //Gears 1 and 2
            if(joy2->GetRawButton(10)) { // button 10 on the joystick will initiate the gear stopper
                gear1.Set(0.425); //previous value: 1
                gear2.Set(0.575); //previous value: 0
            } else {
                gear1.Set(0.95); //previous value: .45
                gear2.Set(0.05); //previous value: .55
            }
            //Trigger
            if(joy2 ->GetRawButton(1)){ // trigger on the joystick will release the balls into the motor.
                ballReleaser.Set(0);
            } else {
                ballReleaser.Set(0.5);
            }
            //Shooter
            if(joy2->GetRawButton(2)) { // starts up the motor when button 12 is pressed
                shooter.Set(((((joy2->GetThrottle())*-1)+1)/4)+0.25); //This puts power between 50% and 100%
            } else {
                shooter.Set(0.0f);
            }
            //Agitator
          /*  if(joy2->GetRawButton(1)){
                agitator.Set(-0.75);
            } else {
                agitator.Set(0.0);
            } */
           //TEST/DEBUG for switches
            if(inputA.Get()){
                if(inputB.Get()){
                    lights.Set(1);
                } else {
                    lights.Set(0);
                }
            } else if (inputB.Get()){
                    lights.Set(0.5);
            } else {
                lights.Set(-1.0);
            }
        }
        void DisabledInit() {
            // Anything you need to initialize in disabled mode
            log_info("Start of Disabled mode.\n");
        }
        void DisabledPeriodic() {
            // This method runs periodically during disabled mode. However, you
            //  cannot do any sort of movement code in here legally, so it is
            //  only here for some basic error and logging utility.
            // Also, during this mode the motors are completely disabled during
            //  this mode anyway, so don't bother
        }
    private:
        Joystick* joy;       //Drive Joystick
        Joystick* joy2;      //Arduino "Joystick"
        Talon frontLeft;     //Front-left Mecanum wheel
        Talon backLeft;      //Back-left Mecanum wheel
        Talon frontRight;    //Front-right Mecanum wheel
        Talon backRight;     //Back-right Mecanum wheel
        Talon shooter;       //Shooter Motor
        //climbMotor1
        //ClimbMotor2
        RobotDrive* drive;   //Drive Train
        Servo gear1;         //Gear 1 to move recepticle
        Servo gear2;         //Gear 2 " "
        Servo ballReleaser;  //hopper servo that releases ball when pressed.
       // Talon agitator;      //The ball agitator to make sure balls dont get jammed
        Talon lights;
        RobotDrive* climber;
        Timer clock;         //Creates the clock object used for getting time
        Timer shooterClock; //clock dedicated to shooter.
        DigitalInput inputA; //TEST Input for DIO port 0 from a switch
        DigitalInput inputB; //TEST Input for DIO port 1      " "
        DigitalInput inputC; //TEST Input for DIO port 2      " "
};
START_ROBOT_CLASS(Robot);
