/* A script made for TUC science day. 
 * It is combination of the stationary ball tracking and the active ball tracking.
 * Change between those two happens with the left and the right bumper.
 * The touch sensor at the top of the nao works as kill switch.
 * */
 
option(ScienceDaySoccerBehavior)
{
	/** Initially, all robot joints are off until the chest button is pressed. */
	
	initial_state(playDead)
	{
		transition
		{
			if(action_done) // After standing up successfully.
			{
				OUTPUT_TEXT("Waiting for left bumper.");
				goto waitForBumper;
			}
			
			// Skip playDead state at a restart after a crash
			
			else if(Global::getSettings().recover)
			{
				goto walkToBall;//trackTheBall;
			}
			if(SystemCall::getMode() == SystemCall::simulatedRobot)
			{
				goto walkToBall;//trackTheBall; // Don't wait for the button in SimRobot
			}
		}
		action
		{
			// Look forward.
            
            theHeadControlMode = HeadControl::lookForward;
			
			HeadControl();
			
			Stand();
			
		}
	}
	
	// In this state the nao waits for the press of a bumper.
	
	state(waitForBumper)
	{
		transition
		{
			if(action_done) // left bumper button pressed and released
			{
				OUTPUT_TEXT("Tracking the ball.");
				goto trackTheBall;
			}
		}
		action
		{
			Activity(BehaviorStatus::unknown);
			
			ButtonPressedAndReleased(KeyStates::leftFootLeft, 1000, 0);
		}
	}
	
	// This state acts as kill switch and its called in order to stop the robot.
	
	state(halt)
	{
		transition
		{
			// Check whether the left bumper is being pressed in order to restart the behavior.
			OUTPUT_TEXT(action_done);
			if(action_done) // left bumper button pressed and released
			{
				goto trackTheBall;
			}
			//if 
			//headMiddle
		}
		action
		{
			// Look forward.
            
            theHeadControlMode = HeadControl::lookForward;
			
			HeadControl();
			
			Stand();
			
			Activity(BehaviorStatus::unknown);
			
			ButtonPressedAndReleased(KeyStates::leftFootLeft, 1000, 0);
		}
	}
	 
	/* Turn in order to face the ball.
	* */
	state(trackTheBall)
	{
		transition
		{
			if(action_done)
			{
				OUTPUT_TEXT("Changing behavior");
				
				goto stand;
			}
			// Check if the ball has been seen recently.
        
			if(theLibCodeRelease.timeSinceBallWasSeen > 4000)//theBehaviorParameters.ballNotSeenTimeOut)
			{
				// If not we need to relocate it.

				OUTPUT_TEXT("searchForBall");

				goto searchForBallStill;
			}
		}
		action
		{
			theHeadControlMode = HeadControl::lookBall;
			
			HeadControl();
			
			ButtonPressedAndReleased(KeyStates::leftFootLeft, 1000, 0);
		}
	}
	
	// In this state the robot stands up.
	
	state(stand)
	{
		transition
		{
            if(action_done)
            {
                // If you have reached the ball sit down. Can I track while sitted?
                
                OUTPUT_TEXT("Walking to ball.");
                goto walkToBall;

            }
        }
        action
        {
            //Activity(BehaviorStatus::gettingUp); Way to aggressive

            theHeadControlMode = HeadControl::lookForward;
            
			HeadControl();
			
            Stand();
        }
	}
	
	// In this state the nao heads to the ball.
	
	state(walkToBall)
	{
		transition
		{
			// In order to stop.
			
			if(action_done) // When the head sensor is press/touched.
			{
				OUTPUT_TEXT("Halt");
				goto halt;
			}
			
            // Check if the ball has been seen recently.
            
            if(theLibCodeRelease.timeSinceBallWasSeen > 4000)//theBehaviorParameters.ballNotSeenTimeOut)
            {
                // If not, we need to relocate it.
                
                OUTPUT_TEXT("searchForBall");
                goto searchForBall;
            }
            
            // If you have reached the ball sit down. Can I track while sitted?
			//if(theLibCodeRelease.between(theBallModel.estimate.position.angle(),-10_deg,10_deg) && 
			if(theBallModel.estimate.position.x() <= 250)
            {
                OUTPUT_TEXT("In position to kick the ball. Kicking");
                goto kick;
            }
        }
        action
        {
            // Look at the ball so you will head correctly there.
            
            theHeadControlMode = HeadControl::lookBall;
            
			HeadControl();
			
            // Also head towards the ball.

            //WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
            WalkAtRelativeSpeed(Pose2f(theBallModel.estimate.position.angle(), 0.45f, 0.f));
			
			// And also wait in case nao needs to be stopped.
			
			Activity(BehaviorStatus::unknown);
			
			ButtonPressedAndReleased(KeyStates::headMiddle, 1000, 0);
		}
    }
	
	// A default state where Nao searchs for the ball.

	state(searchForBallStill)
	{
		transition
		{
			if(action_done)
			{
				OUTPUT_TEXT("Changing behavior");
				
				goto stand;
			}
			
			// Could this be done otherwise? Like if the ball is in the frame?
			if(theLibCodeRelease.timeSinceBallWasSeen < 500) // This must be small.
			{
				OUTPUT_TEXT("Found the ball, tracking...");
				OUTPUT_TEXT("trackTheBall");
				goto trackTheBall;
			}
		}
		action
		{
			// Turn around your head around in order to define the position of the ball.  

			theHeadControlMode = HeadControl::lookLeftAndRight;
			
			HeadControl();
			
			ButtonPressedAndReleased(KeyStates::leftFootLeft, 1000, 0);
		}
	}
	
	// A default state where Nao searchs for the ball.
    
    state(searchForBall)
    {
        transition
        {
			// In order to stop.
			
			if(action_done) // When the head sensor is press/touched.
			{
				OUTPUT_TEXT("Halt");
				goto halt;
			}
			
            // Could this be done otherwise? Like if the ball is in the frame? This must be based with the walk to ball state in order to avoid needless loops.
            if(theLibCodeRelease.timeSinceBallWasSeen < 500) // 300 is debatable maybe make it shorter.
            {
                OUTPUT_TEXT("Found the ball, heading there...");
                OUTPUT_TEXT("walkToBall");
                goto walkToBall;
            }
        }
        action
        {
            // Turn around your head around in order to define the position of the ball.  
            
            theHeadControlMode = HeadControl::lookLeftAndRight;
            
			HeadControl();
			
            WalkAtRelativeSpeed(Pose2f(0.8f, 0.1f, 0.f)); // last known position?
			
			// And also wait in case nao needs to be stopped.
			
			Activity(BehaviorStatus::unknown);
			
			ButtonPressedAndReleased(KeyStates::headMiddle, 1000, 0);
        }
    }
	
	// This state stops nao complitely.
	
	state(stop)
	{
		transition
		{
			if(action_done)
			{
				goto playDeadDoNotRecover;
			}
		}
		action
		{
			Activity(BehaviorStatus::unknown);
			LookForward();
			SpecialAction(SpecialActionRequest::sitDown);
		}
	}
	
	state(kick)
	{
		transition
		{
			if(state_time > 3000 || (action_done))//(state_time > 1000 && action_done))
			{
				OUTPUT_TEXT("Walking to ball.");
				goto walkToBall;
			}
		}
		action
		{
			theHeadControlMode = HeadControl::lookBall;
			
			HeadControl();
		
			// Different type of kicks?

			InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::left), Pose2f(theBallModel.estimate.position.angle(), theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
		}
	}
	
	/** After pressing the chest button thrice we don't want the robot to recover */
	state(playDeadDoNotRecover)
	{
		transition
		{
		}
		action
		{
			Activity(BehaviorStatus::unknown);
			SpecialAction(SpecialActionRequest::playDead);
			ButtonPressedAndReleased(KeyStates::chest, 1000, 0);
		}
	}
}
