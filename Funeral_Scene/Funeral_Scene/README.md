
# Funeral Scene

## Processing Sketch:

The processing sketch layers two sounds together where one is playing on mute and the other
is playing on full volume. When a magnetic sensor (hall effect) triggers it will crossfade
each song. Where one song lowers in volume and the other raises.

## Arduino Sketch:

The arduino sketch is responsible for brightening the LED lights in the room and is all
contingent on a single sensor. Which fades a volume variable from 0 -> 255. This value
is passed to the processing sketch which will crossfade the audio tracks.

# Pin Information

Funeral Scene:

	Inductive Sensor is on Pin 7
	DMX uses pin 4
## ssh
If you need to rework things on the pi, reverse the steps in the Disable Swap and Filesystem section and
then ssh into the pi.  
**The command on linux is:**  
```ssh -X pi@192.168.1.188```  
```PW :: raspberry```
