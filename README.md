# ScienceDayBehaviorWithBHuman
A basic behavior of ball tracking for Nao using BHuman

## Requirements
BHumanCodeRelease2017 must be the working project. The behavior wont work at 2018 since the project hierarcy differs.
- https://github.com/bhuman/BHumanCodeRelease/tree/coderelease2017

## Behavior summary

 ScienceDaySoccerBehaviorV2 is the complete behavior. It is combination of a stationary ball tracking and an active ball tracking.
Change between those two happens with the left bumper. At first it tracks stationary and after the second press goes to active
tracking. The touch sensor at the top of the nao works as kill switch.
This is the version 2 in which Nao also listens for falling and fallen states while on the halt state, after the first cycle of behaviors has occured in order to try to recover form the fall.

## Required files

- ScienceDaySoccerBehaviorV2.h
- ScienceDaySoccerBehavior.h
- HeadControl.h
- LookBall.h
- LookForward.h
- LookLeftAndRight.h
- behaviorControl.cfg

## Required files location

- /BHumanCodeRelease/Src/Modules/BehaviorControl/BehaviorControl/Options/Roles

## Changed files

## Src

### Options.h 

In Options.h we include our files in order for our behaviors to work.

- #include "Options/Roles/Example.h"
- #include "Options/Roles/ScienceDaySoccerBehavior.h"
- #include "Options/Roles/ScienceDaySoccerBehaviorV2.h"

### HeadControl.h

- We add the necessary code in order to for the head control to handle the rest of the head movements.

## Config

### behaviorControl.cfg

- roots = [ScienceDaySoccerBehaviorV2];

## How to run

* We add the files to their corresponding location
* Then we go to the corresponding directory at then BHumanCodeRelease/Make/. For linux is BHumanCodeRelease/Make/Linux
* We complile by typing in a terminal in that directory
make
* Then be we need to copy the compliled code in the Nao via running ./copyfiles script in a terminal.
An example of that is
./copyfiles Develop 192.168.3.25 -b -v 100 [-b in order to restart bhuman after the copy][-v in order to set the Nao's volume]
for more information about copyfiles write 
./copyfiles -h 



