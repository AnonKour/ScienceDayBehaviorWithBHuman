option(HeadControl)
{
  common_transition
  {
    if(!theGroundContactState.contact && theGameInfo.state != STATE_INITIAL)
    {
        goto lookForward;
    }
    switch(theHeadControlMode)
    {
      case HeadControl::off:
      {
          goto off;
      }
      case HeadControl::lookForward:
      {
          goto lookForward;
      }
      case HeadControl::lookLeftAndRight:
      {
          goto lookLeftAndRight;
      }
      case HeadControl::lookBall:
      {
          goto lookBall;
      }
      default:
      {
          goto none;
      }
    }
  }

  initial_state(none) 
  {
      goto lookForward;
  }
  
  state(off) 
  {
      action SetHeadPanTilt(JointAngles::off, JointAngles::off, 0.f);
      
  }
  state(lookForward) 
  {
      
      action LookForward();
  }
  state(lookLeftAndRight) 
  {
      
      action LookLeftAndRight();
  }
  state(lookBall) 
  {
      
      action LookBall();
  }
}

struct HeadControl
{
  ENUM(Mode,
  {,
    none,
    off,
    lookForward,
    lookLeftAndRight,
    lookBall,
  });
};

HeadControl::Mode theHeadControlMode = HeadControl::Mode::none; /**< The head control mode executed by the option HeadControl. */
