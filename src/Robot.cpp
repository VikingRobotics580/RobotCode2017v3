#include <WPILib.h>

/*
 * The Robot Load Order, after calling START_ROBOT_CLASS, is as follows:
 *
 * Constructor -> RobotInit -> DisabledInit -> DisabledPeriodic
 * Once a mode changes, the function execution order looks like the following:
 * PreviousModePeriodic -> NextModeInit -> NextModePeriodic
 *
 */

class Robot : public IterativeRobot {
    public:        Robot():
            IterativeRobot(),
            frontLeft(0),
            backLeft(1),
            frontRight(2),
            backRight(3),
            shooter(4)
        {
            // You should only initialize value here. Try not to have any other
            //  executable code here.
            joy = new Joystick(0);
            drive = NULL;
        }

        void RobotInit() {
            // Initialize the Robot and all functions that need to be called.
            printf("Initializing Robot.\n");
            drive = new RobotDrive(frontLeft, backLeft, frontRight, backRight);
            drive->SetSafetyEnabled(false); // This is dangerous, but our robot
                                            //  has some problems and needs
                                            //  this.
        }

        void AutonomousInit() {
            // Here is where you would initialize all of your autonomous code
            printf("Initializing Autonomous mode.");
        }

        void AutonomousPeriodic() {
            // Here is where you would put all of your autonomous code.
            // It is important to note that all code here will run as though
            //  autonomous mode is endless, so you have to carefully time how
            //  long your code will run for.
            // Just drive straight.
            drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);
        }

        void TeleopInit() {
            printf("Initializing Teleop mode.\n");
            // We don't have anything to initialize here.
        }

        void TeleopPeriodic() {

        	// During teleop we want to pass Mecanum Drive the joystick axes.
            // void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
            // Note that these axes may be reversed, so you may need to switch
            //  the GetX and GetY functions
            drive->MecanumDrive_Cartesian(joy->GetX(), joy->GetY(),
                                          joy->GetTwist(), 0.0f);


            if(joy->GetRawButton(1)){
            	shooter.Set(joy->GetThrottle()); /*GET VALUE FROM SHOOTER BUILD TEAM*/

            } else {
            	shooter.Set(0.0f);
            }


        }

        void DisabledInit() {
            // Anything you need to initialize in disabled mode
            printf("Start of Disabled mode.\n");
        }

        void DisabledPeriodic() {
            // This method runs periodically during disabled mode. However, you
            //  cannot do any sort of movement code in here legally, so it is
            //  only here for some basic error and logging utility.
            // Also, during this mode the motors are completely disabled during
            //  this mode anyway, so don't bother
        }

        // You can also have a Test mode, though this will only run if you
        //  specifically start it from your driver station. It will not be
        //  run during the competitions.

    private:
        Joystick* joy;
        Talon frontLeft;
        Talon backLeft;
        Talon frontRight;
        Talon backRight;
        Talon shooter;
        RobotDrive* drive;
};

START_ROBOT_CLASS(Robot);

