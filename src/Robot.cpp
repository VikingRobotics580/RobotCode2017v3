//2017 RoboCode v3.0.2 Source Code
#include <WPILib.h> // Worchester Polytechnic Institute (WPI) Library for Robot classes,
#include <stdio.h> //We use some of this for logging purposes.
#include <logging.h> //This is where we have our logging code
#include <Math.h>
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
            shooter(9),     //port 9
            gear1(5),       //port 5
            gear2(6),       //port 6
            ballReleaser(7),//port 7
        //    agitator(8),    //(ex)port 8
            climb1(8),      //NEW port 8
            climb2(4),      //port 4
            clock(),
            shooterClock(),
            inputA(0), //HOPEFULLY DIO port 0
            inputB(1), //HOPEFULLY DIO port 1
            inputC(2), //HOPEFULLY DIO port 2
            lights(3)
        {
            // You should only initialize value here. Try not to have any other
            //  executable code here.
            joy = new Joystick(0);  //Drive Joystick
            joy2 = new Joystick(1); //Shooter Joystick
            drive = NULL;
            climber = NULL;
        }
    void base(){
        // void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
        if(clock.Get() <= 3.7f){
            drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //range of motion is inverted from -1.0-1.0
        } else if(clock.Get() >= 14.9){
            clock.Reset();
            clock.Stop();
        } else {
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //0.0 = not moving
        }
    }
 
    void simple(){
        if (clock.Get() <= 1.73){
            drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //move forward 100%
            } else {
                drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //do nothing
                clock.Stop();
 
            }
    }
 
    void right(){ //field position right, shoot left
        if (clock.Get() <= 1.5){
        drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //drive forward 50%
 
        } else if (clock.Get() > 1.5 && clock.Get() < 2.125){
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, -0.5f, 0.0f); //rotate 50%
 
        } else if (clock.Get() >= 2.125 && clock.Get() < 3.315){
            drive->MecanumDrive_Cartesian(0.0f, -0.25f, 0.0f, 0.0f); //drive forward 50%
 
        } else if (clock.Get() >= 2.72 && clock.Get() < 7.7){
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //do nothing 5 seconds
 
       /* } else if (clock.Get() >= 7.7 && clock.Get() < 8.2){
            drive->MecanumDrive_Cartesian(0.0f, 0.5f, 0.0f, 0.0f); //reverse 50%
 
        } else if (clock.Get() > 8.2 && clock.Get() < 8.825){
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.5f, 0.0f); //turn back
 
        } else if (clock.Get() > 8.825 && clock.Get() < 11.0){
            drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //drive forward 50%
*/
        } else if (clock.Get() >= 14.9){
            clock.Reset();
            clock.Stop();
 
        } else {
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); // do nothing
        }
    }
 
    void left(){ //field position left
        if (clock.Get() <= 1.55){
                drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //drive forward 50%
 
                } else if (clock.Get() > 1.5 && clock.Get() < 2.125){
                    drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.5f, 0.0f); //rotate 50% right
 
                } else if (clock.Get() >= 2.125 && clock.Get() < 4.505){
                    drive->MecanumDrive_Cartesian(0.0f, -0.125f, 0.0f, 0.0f); //drive forward 25%
 
                } else if (clock.Get() >= 2.72 && clock.Get() < 7.7){
                    drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //do nothing 5 seconds
 
               /* } else if (clock.Get() >= 7.7 && clock.Get() < 8.2){
                    drive->MecanumDrive_Cartesian(0.0f, 0.5f, 0.0f, 0.0f); //reverse 50%
 
                } else if (clock.Get() > 8.2 && clock.Get() < 8.825){
                    drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.5f, 0.0f); //turn back
 
                } else if (clock.Get() > 8.825 && clock.Get() < 11.0){
                    drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //drive forward 50%
        */
                } else if (clock.Get() >= 14.9){
                    clock.Reset();
                    clock.Stop();
 
                } else {
                    drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); // do nothing
                }
 
    }
 
    void middle(){
            if (clock.Get() <= 1.43){
                drive->MecanumDrive_Cartesian(0.0f, -0.45f, 0.0f, 0.0f); //move forward 100%
                } else if(clock.Get() >= 14.9){
                    clock.Reset();
                    clock.Stop();
                } else {
                    drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //do nothing
                }
        }
 
    double superBallShooterPro(double time){
        return (0.25*sin(2*time)+0.25);
    }
 
    void RobotInit() {
            // Initialize the Robot and all functions that need to be called.
            log_info("Initializing Robot.\n");
            drive = new RobotDrive(frontLeft, backLeft, frontRight, backRight);
            drive->SetSafetyEnabled(false); // This is dangerous, but our robot
                                            //  has some problems and needs
                                            //  this.
            climber = new RobotDrive(climb1, climb1, climb2, climb2);
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
             *      Midd Gear Dis: t= 1.43@ 45%                                            *
             *      Side Base Dis: t= 2.75@ 50%                                            *
             *                                                                             *
             *                                                                             *
            \*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */
 
        /*  if (inputA.Get()){
                base();
            } else {
                simple();
            } */
            if (inputA.Get()){
                if (inputB.Get()){ //Yes on A
                    if (inputC.Get()){
                        base(); //111
                    } else {
                        //110
                    }
                } else if(inputC.Get()){
                    //101
                } else {
                    //100
                    left();
                }
            } else if (inputB.Get()){ //No on A, yes on B
                if(inputC.Get()){
                    //011
                } else{
                    //010
                }
            } else if (inputC.Get()){// No on A and B, yes on C
                //001 pene;
                right();
            } else {
                //000
                simple();//
            }
 
        /*  if (inputB.Get()){ //010
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
          } else if (clock.Get() <= 14){
              drive->MecanumDrive_Cartesian(0.0f, -1.0f, 0.0f, 0.0f); //move forward 100%
          } else if(clock.Get() == 149){
              clock.Reset();
              clock.Stop();
          } else {
              drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f); //do nothing
          } */
 
        }
        void TeleopInit() {
            log_info("Initializing Teleop mode.\n");
            clock.Reset(); //This is so we can reset Autonomous mode through TeleOp for testing
        }
        void TeleopPeriodic() {
            //Drive Train
            // During TeleOp we want to pass Mecanum Drive the joystick axes.
            // void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
            drive->MecanumDrive_Cartesian((joy->GetX()*1.25), (joy->GetY()*1.25),
                                          (joy->GetTwist()*1.25), 0.0f);
 
            //climber
            if(joy2->GetRawButton(3)){
                climber->MecanumDrive_Cartesian(0.0f, ((exp(-0.075*(joy2->GetY())))/5), 0.0f, 0.0f);
            } else if (joy2->GetRawButton(4)){
                climber->MecanumDrive_Cartesian(0.0f, -1*((joy2->GetY()-1)/2), 0.0f, 0.0f);
            } else {
                climber->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);
            }
            //Gears 1 and 2
            if(joy2->GetRawButton(10)) { // button 10 on the joystick will initiate the gear stopper
                gear1.Set(-0.1); //previous value:
                gear2.Set(1.1); //previous value:
            } else {
                gear1.Set(0.70); //previous value:
                gear2.Set(0.30); //previous value:
            }
            //Trigger
            if(joy2 ->GetRawButton(1)){ // trigger on the joystick will release the balls into the motor.
                ballReleaser.Set(0);
                shooterClock.Stop();
                shooterClock.Reset();
            } else if (joy2 -> GetRawButton(7)){
                shooterClock.Start();
                superBallShooterPro(shooterClock.Get());
            } else {
                ballReleaser.Set(0.5);
                shooterClock.Stop();
                shooterClock.Reset();
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
        Servo gear1;         //Gear 1 to move recepticle
        Servo gear2;         //Gear 2 " "
        // Talon agitator;      //The ball agitator to make sure balls dont get jammed
        Servo ballReleaser;  //hopper servo that releases ball when pressed.
        Talon climb1;
        Talon climb2;
        RobotDrive* drive;   //Drive Train
        RobotDrive* climber;
        Timer clock;         //Creates the clock object used for getting time
        Timer shooterClock; //clock dedicated to shooter.
        DigitalInput inputA; //TEST Input for DIO port 0 from a switch
        DigitalInput inputB; //TEST Input for DIO port 1      " "
        DigitalInput inputC; //TEST Input for DIO port 2      " "
        DigitalOutput lights;
};
START_ROBOT_CLASS(Robot);