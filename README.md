# s60-positioning
Module for recieve location with GPS for Symbian OS.

There are two main classes:
- CPositionRequestor
- CDynamicPositionRequestor 

## CPositionRequestor
Obtains location with **fixed** time intervals (for example: every second).

## CDynamicPositionRequestor
Position refresh rate **depends on moving speed**:

> update interval = distance / speed

Where:
- `speed` — maximum speed measured in the last 60 seconds (in meters per second)
- `distance`— desired distance between neighboring locations (in meters, at the moment hardcoded as 30m)

Also there are additional restrictions for position update interval: minimum — 1 second,  maximum — 10 seconds.

***ToDo:** Make these values customizable*
