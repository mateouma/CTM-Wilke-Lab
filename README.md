# CTM-Wilke-Lab
Code for Continuous T-Maze (CTM)

## Development Log
7.11 - Added print lines to all preexisting functions in original motor script, created button functions in `mini_motor_tester.ino` (MU)

7.12 - Button inputs recognized, attempted to add functionality to buttons in `mini_motor_tester.ino` (MU)

7.13 - Removed button functionality, successfully labeled and implemented door *open* functions in `setup()` in `mini_motor_tester.ino` (MU)

7.13 - Added `PIR_Sensor_Checker.ino` to repository (MU), added sensor pin initialization and attempted sensor functionality (TG)

11.10 - Removed outdated scripts, added most recent and stable scripts (`master_board.ino` and `door_script.ino`) for the master and motor boards. Motor board can open doors and master board can open doors via the motor board, identify PIR sensor movements, and raise/lower effort barriers at will (MU)

11.11 - Added reward script (MU)

1.11 - Updated scripts for master and motor boards (MU) - active trial state is defined as when the start sensor is triggered. Sensors can only run code (i. e. open doors) once during an active trial until another trial is started.
