# HoltzmanPong_RTOS
Final project for ECEN 3753 - Real Time Operating Systems

A game played on Sillicon Labs Pearl Gecko using Micrium OS. The goal of the 
game is to defend your base from the oncoming Harkonnen onslaught.
Control your platform to stop the rocks from hitting your planet and launch the 
rocks back at your enemy.

# Controls

The goal of the game is to bounce back the meteors launched at your base by bouncing them off your platform when your shield is active. If you can't reach the meteor in time, you can blow them up with your laser cannon.

### In Menu

BTN1 - Move Cursor Down

BTN0 - Cycle/Select Option

### In Game

BTN1 - Turn on Shield (Must Hold for duration)

BTN0 - Fire Laser

Capacitive Sensor - Acceleration (broken into 4 sections of sensor)

# Difficulty

### Easy
- 1 Projectiles at a time
- Max Platform Speed 100 px/s
- Gravity 25 px/s^2
### Medium
- 1 Projectiles at a time
- Max Platform Speed 50 px/s
- Gravity 25 px/s^2
### Hard
- 2 Projectiles at a time
- Max Platform Speed 50 px/s
- Gravity 25 px/s^2
### Insane
- 2 Projectiles at a time
- Max Platform Speed 50 px/s
- Gravity 35 px/s^2
# Task Diagram
![alt text](https://github.com/willsnider927/HoltzmanPong_RTOS/blob/main/task_diagram.drawio.png?raw=true)

