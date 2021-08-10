#!/bin/bash

echo "--------------------------------------------------------"
echo "Starting Chaos:"

cd /home/pi/chaos/raw-gadget-timeout/raw_gadget
./insmod.sh

#while true;
#do
	/home/pi/chaos/build/examples/TLOU2/tlou2
#done

echo "Done."
echo "--------------------------------------------------------"
