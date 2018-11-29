/** An option intended for scanning purposes in order for the robot to determine 
 * several things regards its surroundings.*/

option(LookBall)
{
    /* Simply sets the necessary angles 
    initial_state(lookForward)
    {
        transition
        {
            if(state_time > 200)
            {
                goto lookBall;
            }
        }

    }
    */
    initial_state(lookBall)
    {
        transition
        {
        }
        action
        {
            /* > Pan is from side to side. As a result of this the +0.90 degres is to the left of
            * the robot, while -0.90 is to the right.
            * > Tilt is from up and down. This time the + is for the down ward movement of the 
            * robots head while the - moves the robot head upwards.
            * > The last parameter is the speed that the robot is asked to do the head movement.
            */
            
            // Do we want to make the speed relative to the distance? There was some oscilation occuring during suddent movements. Also we need to add an threshold.
            
            // Need to get the current angle and the new one. 
            
            SetHeadPanTilt(theBallModel.estimate.position.angle(), 0.34f ,135_deg);
        }
    }
    
}