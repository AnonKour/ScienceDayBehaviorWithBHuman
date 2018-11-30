/** A goal keeper striker option */

option(Striker)
{
  // This is the initial/root state of the fsm.
  initial_state(start)
  {
    // Decide on the next state of the finite state machine.
    transition
    {
      if(state_time > 1000)
      {
          OUTPUT_TEXT("turnToGoal");
          goto turnToGoal;//walkToGoal;
      }
    }
    action
    {
      // Do the action for that state. In a way the action is done first? Or paraller with the transition. Moore/Meally?
      theHeadControlMode = HeadControl::lookForward;
      HeadControl();
      theObstacleModel.verify();
      //OUTPUT_TEXT(int(theObstacleModel.obstacles.size()));
      Stand();
    }
  }
  /* In this state the goalkeeper needs to find his goalposts?Or the penalty area and turn towards them in order to 
   * properly posiiton himself.* How should he do that? For now 3 stages 1) walk towards it and after you posiiton yourself turn to face the ball.
   * In order to determine the friendly goal for now we chose the closest one to the keeper.
   */
  state(turnToGoal)
  {
      transition
      {
          //OUTPUT_TEXT(theFieldDimensions.xPosOwnPenaltyMark); -3200 while we want -3400
          //goto searchForPenaltyArea; Is it needed? Since the penalty area place is't dynamic. After its position is found it doenst change.
          
          // Check whether the goalkeeper is facing the goal in order to aproach it.
          //if(theLibCodeRelease.between(theRobotPose.translation.x(), (theFieldDimensions.xPosOwnPenaltyMark - 200.f) + 20.f, (theFieldDimensions.xPosOwnPenaltyMark - 200.f) + 40.f)) // this works
          if(std::abs((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f)).angle()) < 4_deg)
          {
              OUTPUT_TEXT("walkToGoal");
              goto walkToGoal;
          }
      }
      action
      {
          theRobotPose.verify();
          
          // Set the head in order to look forward.
          
          theHeadControlMode = HeadControl::lookForward;
          HeadControl();
          // Walks to the target. The first parameter is the speed and the second one is the target.X is the variable the y is the same for both goals since its the center and its mirrored. 
          WalkAtRelativeSpeed(Pose2f((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f)).angle()));
          //((theRobotPose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f)).angle());
          //WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theFieldDimensions.xPosOwnPenaltyMark, theFieldDimensions.yPosCenterGoal));
      }
      
  }
  /* Walk to the goal in order to defend it.
   * In order to find the goal we search the posts while for the alignment the penalty mark is used.
   */
  state(walkToGoal)
  {
      transition
      {
          /*
           * OUTPUT_ERROR("I E am confuzed.");
          OUTPUT_WARNING("I W am confuzed.");
          OUTPUT_TEXT("I T am confuzed.");
          */
          //OUTPUT_TEXT(theRobotPose.translation.x());
          //OUTPUT_TEXT((theFieldDimensions.xPosOwnPenaltyMark - 1000.f) + 10.f);
          //OUTPUT_TEXT((theFieldDimensions.xPosOwnPenaltyMark - 1000.f) + 100.f);
          //OUTPUT_TEXT((theLibCodeRelease.between(theRobotPose.translation.x(), (theFieldDimensions.xPosOwnPenaltyMark - 1000.f) + 10.f, (theFieldDimensions.xPosOwnPenaltyMark - 1000.f) + 100.f)));
          //OUTPUT_TEXT(theFieldDimensions.xPosOwnPenaltyMark); -3200 while we want -4200
          //goto searchForPenaltyArea; Is it needed? Since the penalty area place is't dynamic. After its position is found it doenst change.
          
          // Check whether the goalkeeper is facing the goal in order to aproach it.
          if(theLibCodeRelease.between(theRobotPose.translation.x(), (theFieldDimensions.xPosOwnPenaltyMark - 1000.f) + 10.f, (theFieldDimensions.xPosOwnPenaltyMark - 1000.f) + 100.f)) // this works
          {
              OUTPUT_TEXT("alingInfrontOfGoal");
              goto alingInfrontOfGoal;
          }
      }
      action
      {
          theRobotPose.verify();
          // Set the head in order to look forward.
          theHeadControlMode = HeadControl::lookForward;
          HeadControl();
          // Walks to the target. The first parameter is the speed and the second one is the target.X is the variable the y is the same for both goals since its the center and its mirrored. 
          //WalkToTarget(Pose2f(1.f, 1.f, 100.f), Pose2f(0,  theFieldDimensions.yPosCenterGoal - theRobotPose.translation.y()));// need to think about it might need an if.
          
          
          
          // Walk to your goal.
          WalkAtRelativeSpeed(Pose2f((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnGroundline, 0.f)).angle(),1.f,0.f));
          //WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theRobotPose.translation.x() -theFieldDimensions.xPosOwnPenaltyMark + 1000.f, theFieldDimensions.yPosCenterGoal));
         /*
          OUTPUT_TEXT((theRobotPose * Vector2f(theFieldDimensions.xPosOpponentGoalPost, theFieldDimensions.yPosRightGoal)).angle());
          //(theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
          
          WalkToTarget(Pose2f(50.f,100.f,100.f),Pose2f(theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGoalPost, 0.f)).angle(), theFieldDimensions.yPosRightGoal-theRobotPose.translation.y()));
          */
          //WalkToTarget(Pose2f(50.f,100.f,100.f),Pose2f((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGoalPost, 0.f)).angle(),theFieldDimensions.xPosOpponentGoalPost - theRobotPose.translation.x(), theFieldDimensions.yPosRightGoal-theRobotPose.translation.y()));
      }
  }
  /* This state is different from the align to goal of a defednder or striker. The goalkeer aligns with the goal he is defending based on the position of the ball.*/
  state(alingInfrontOfGoal)
  {
    transition
    {
        //OUTPUT_TEXT(theRobotPose.translation.y());
        // Check whether you have reached the wanted position.
        
        if(theLibCodeRelease.between(theRobotPose.translation.y(),  - 5.f,  5.f)) //&& (std::abs((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f)).angle()) < 2_deg))
        {
            OUTPUT_TEXT("turnToBall");
            goto turnToBall;
        }
        
    }
    action
    {
      // Do the action for that state. In a way the action is done first? Or paraller with the transition. Moore/Meally?
      
      theHeadControlMode = HeadControl::lookForward;
      
      HeadControl();
      
      // Do a verification and if its needed then do some corrections.
      
      theRobotPose.verify();
      //OUTPUT_TEXT(theRobotPose.translation.y() - theFieldDimensions.yPosCenterGoal);
      //OUTPUT_TEXT(theFieldDimensions.yPosCenterGoal - theRobotPose.translation.y());
      if(std::abs((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f)).angle()) > 90_deg)
      {
          WalkToTarget(Pose2f(1.f, 1.f, 100.f), Pose2f(0, theRobotPose.translation.y() - theFieldDimensions.yPosCenterGoal));
      }
      else
      {
          WalkToTarget(Pose2f(1.f, 1.f, 100.f), Pose2f(0, theFieldDimensions.yPosCenterGoal - theRobotPose.translation.y()));
      }
      
      
      
      
      /*
      if(!theLibCodeRelease.between(theRobotPose.translation.y(),  - 5.f,  5.f))
      {
            
      }
      else
      {
          WalkAtRelativeSpeed(Pose2f((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f)).angle() , 0.f,  0.f));
          
            OUTPUT_TEXT((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f)).angle());
            if((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f)).angle() >= 0)
            {
                // I am a bit confuzed why this actually works.
      
                WalkToTarget(Pose2f(1.f, 1.f, 100.f), Pose2f(0, theRobotPose.translation.y() - theFieldDimensions.yPosCenterGoal));
            }
            else
            {
                WalkToTarget(Pose2f(1.f, 1.f, 100.f), Pose2f(0, theFieldDimensions.yPosCenterGoal - theRobotPose.translation.y()));
            }    
           
      }*/
      
      //WalkAtRelativeSpeed(Pose2f((theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnPenaltyMark, 0.f).angle(), 0.f, 1.f));
      /*
      if(std::abs(theBallModel.estimate.position.angle()) > 4_deg)
      {
          
          
      }
      else
      {
            // I am a bit confuzed why this actually works.
      
            WalkToTarget(Pose2f(1.f, 1.f, 100.f), Pose2f(0, theFieldDimensions.yPosCenterGoal - theRobotPose.translation.y())); 
            
            if(theRobotPose.translation.y()<theFieldDimensions.yPosCenterGoal)
            {
                WalkToTarget(Pose2f(1.f, 1.f, 100.f), Pose2f(0, theRobotPose.translation.y() - theFieldDimensions.yPosCenterGoal)); 
            }
            else
            {
                WalkToTarget(Pose2f(1.f, 1.f, 100.f), Pose2f(0, theFieldDimensions.yPosCenterGoal - theRobotPose.translation.y())); 
            }
            
            //Stand();
            }
       }*/
    }
  }
  /* Search in order to determine which is going to be the goal that the keeper will defend
   * 
  state(searchForPenaltyArea)
  {
    transition
    {
        goto walkToGoal;
    }
    action
    {
        WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theFieldDimensions.xPosOwnPenaltyMark, theFieldDimensions.yPosCenterGoal));
    }
  }*/
  /* Turn in order to face the ball.
   * */
  state(turnToBall)
  {
    transition
    {
      if(!theLibCodeRelease.between(theRobotPose.translation.y(),  - 5.f,  5.f))
      {
          // If not we need to relocate it.
          OUTPUT_TEXT("alingInfrontOfGoal");
          goto alingInfrontOfGoal;
      }
        
      // Check if the ball has been seen recently.
        
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
      {
          // If not we need to relocate it.
          OUTPUT_TEXT("searchForBall");
          goto searchForBall;
      }
      // Check the position of the ball cause the keeper may need to kick it or block its course in order to defend the goal. To become active.

      OUTPUT_TEXT(theBallModel.estimate.position.x());
      OUTPUT_TEXT(theBallModel.estimate.position.angle());
      if(theBallModel.estimate.position.x() < 2000 && theBallModel.estimate.position.angle() < 4_deg) 
          //(theLibCodeRelease.between(theBallModel.estimate.position.x(),  3800,  3900))
      {
          OUTPUT_TEXT("defendTheGoal");
          goto defendTheGoal;
      }
    }
    action
    {
        
        theHeadControlMode = HeadControl::lookForward;
      
        HeadControl();
      
        /*      
        WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        */
        // Rotate around yourself till the body faces the ball.
        if(std::abs(theBallModel.estimate.position.angle()) > 4_deg)
        {
            /** Sets all members of the MotionRequest representation for executing a relativeSpeedMode-WalkRequest
            *  (i.e. Walk at a \c speed)
            *  Walking speeds, in the range [-1.f .. 1.f].
            *   e.g.  Pose2f(0.f, 1.f, 0.f) lets move the robot forward at full speed
            *         Pose2f(0.f, 0.5f, 0.5f) lets move the robot diagonal at half of the possible speed
            *         Pose2f(0.5f, 1.f, 0.f) lets move the robot in a circle
            */
            WalkAtRelativeSpeed(Pose2f(theBallModel.estimate.position.angle()));
            //WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
            //WalkAtRelativeSpeed(Pose2f(-1.f, 0.f, 0.f));
        }
        else
        {
            Stand();
        }
    }
  }
  /* Defend the goal till its time for a save or pass/kick */
  state(defendTheGoal)
  {
    transition
    {
        //goto block;
        OUTPUT_TEXT("walkToBall");
        goto walkToBall;
        

    }
    action
    {
        
    }
      
  }
  /* Do a save action in order to defend the goal | block*/
  state(block)
  {
    transition
    {
        goto alingInfrontOfGoal;
    }
    action
    {
        
    }
      
  }
  
  /* When the ball is in his own half and not in a dangerous scenario then he can stray from his goal a bit in order to sent it away towards the enemy goal.
   * Implement a pass in the feature.
   * */
  state(walkToBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
      {
          OUTPUT_TEXT("searchForBall");
          goto searchForBall;
      }
      if(theBallModel.estimate.position.norm() < 380.f)
      {
          OUTPUT_TEXT("alignToGoal");
          goto alignToGoal;
      }
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      
      HeadControl();
      //WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
      WalkAtRelativeSpeed(Pose2f(theBallModel.estimate.position.angle(), 1.f, 0.f));
    }
  }

  state(searchForBall)
  {
    transition
    {
        if(theLibCodeRelease.timeSinceBallWasSeen < 150) // 300 is debatable maybe make it shorter.
        {
            OUTPUT_TEXT("turnToBall");
            goto turnToBall;
        }
    }
    action
    {
        // Turn around your head around in order to define the position of the ball.  
        
        theHeadControlMode = HeadControl::lookLeftAndRight;
        
        HeadControl();
        
        //theHeadControlMode = HeadControl::lookForward;
      
        // Rotate around yourself.
      
        //WalkAtRelativeSpeed(Pose2f(1.f, 0.f, 0.f)); 
    }
  }
  
  state(alignToGoal)
  {
    transition
    {
       if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
       {
           OUTPUT_TEXT("searchForBall");
           goto searchForBall;
       }
       if(std::abs(theLibCodeRelease.angleToGoal) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
       {
           OUTPUT_TEXT("alignBehindBall");
           goto alignBehindBall;
       }
    }
    action
    {
       theHeadControlMode = HeadControl::lookForward;
       HeadControl();
       WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 400.f, theBallModel.estimate.position.y()));
    }
  }
  
  state(alignBehindBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
      {
          OUTPUT_TEXT("searchForBall");
          goto searchForBall;
      }
      
      OUTPUT_TEXT(theLibCodeRelease.between(theBallModel.estimate.position.y(), 20.f, 50.f));
      OUTPUT_TEXT(theBallModel.estimate.position.x());
      OUTPUT_TEXT(theLibCodeRelease.angleToGoal);
      OUTPUT_TEXT((theLibCodeRelease.between(theBallModel.estimate.position.y(), 20.f, 50.f) && (theBallModel.estimate.position.x() < 200.f) && std::abs(theLibCodeRelease.angleToGoal) < 10_deg));
      if(theLibCodeRelease.between(theBallModel.estimate.position.y(), 20.f, 50.f) && (theBallModel.estimate.position.x() < 200.f) && std::abs(theLibCodeRelease.angleToGoal) < 10_deg)
      {
          OUTPUT_TEXT("kick");
          goto kick;
      }
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      HeadControl();
      WalkToTarget(Pose2f(0.10f, 50.f, 50.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() - 30.f));
    }
  }

  state(kick)
  {
    transition
    {
      if(state_time > 2000 || (state_time > 10 && action_done))
      {
          OUTPUT_TEXT("turnToGoal");
          goto turnToGoal;
      }
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      HeadControl();
      InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::left), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
    }
  }
}
