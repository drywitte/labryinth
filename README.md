# labyrinth

## Description

Forget solving puzzles with your brain, solve puzzles with your muscles. We're solving the Labryinth maze using EMGs to control the x/y tilting of the puzzle. The EMGs will read a signal from each of the user's forearm muscles based on the user's flexing of the muscles. This signal will translate to x or y axis rotation performed by servo motors hooked up to the game.


## Team
* Zach Gittelman
* Dan Witte (danwitte)

## Prototype
![Diagram](labryinth%20diagram.png)

We're gonnna need longer cables
![Prototype](prototype.jpg)


## Parts

* Arduino
* 2 x continuous servos
* 2 x EMGs
* 2 x 3d printed servo mounts
* 6 Electrodes
* 9v battery

## Interaction Plan

Setup: Users will need to attach 6 electrodes, 3 to each arm. Once the electrodes are connected, the user will to test their ability to rotate the servos and get use to the controls. 

Play: Once users are ready to play, they (or possibly someone else) will need to drop the marble in the maze. The left arm EMG will controll the Y axis, the right arm EMG will control the X axis. Flexing will increase the angle (turn the servo clockwise), half flexing will bring the servo to a stand still, and relaxing the muscle will decrease the angle (turn the servo counter clockwise). Users lose if the marble falls through a hole or they get too tired of flexing :muscle:. Users win if they manage to get the marble to the end of the maze.

## Progress

[EMG setup was a struggle](https://photos.app.goo.gl/P11KadpmMwnDEAYr6)

![Zach is EMG'd up](IMG_0815.jpg)

![Measuring signals](IMG_0818.jpg)

[Single servo hookup](https://photos.app.goo.gl/kSsvtbHaXER2PTeY9)

[Two servos hooked up](https://photos.app.goo.gl/J16m6d5j8TJG6g228)

After testing the mechanics of the game we updated our interaction plan. Originally our mapping was:
* No flex: stop
* Half flex: turn servo clockwise
* Full flex: turn servo counterclockwise

To make the game feel more responsive and reduce confusion, we changed the following:
* Least flex: turn servo clockwise faster
* Less flex: turn servo clockwise slower
* Half flex: stop
* More flex: turn servo counterclockwise slower
* Most flex: turn servo counterclockwise faster
We also removed the signal averaging we were doing as that made the game feel unresponsive.
 

[Zach plays solo](https://photos.app.goo.gl/uBBdr7Vkce5WfSMp9)


## Outcome

In the end the mechanics of the game worked fairly well. For new users the learning curve was fairly quick, although there were moments where it was clear that the interaction wasn't entirely intuitive. For example, it was not uncommon for users to forget that they needed to hold a half flex to keep the board still. They also commonly forgot which arm controlled which axis of rotation. Testers were able to get fairly far into the puzzle, although an invididual user could have very wide variance in how well they do, which is not surprising given that the controls are less fine tuned than your hands. 

## Lessons learned

* Initially we tried a moving average of signals to determine which way to move, however through playing the game we found that it was better to be more responsive and subject to noise than less responsive and smooth.
* Through playing we also found that it was beneficial to have 2 speeds for each movement direction, allowing for better control.
* Quality soldering will save you hours of pain down the road
