#!/bin/bash

echo "--------------------------------------------------------"
echo "Starting Twitch Plays:"

cd /home/pi/chaos/raw-gadget-timeout/raw_gadget
./insmod.sh

#while true;
#do
	/home/pi/chaos/build/examples/TwitchPlays/twitch-plays
#done

echo "Done."
echo "--------------------------------------------------------"
