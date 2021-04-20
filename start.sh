#!/bin/bash

echo "--------------------------------------------------------"
echo "Starting Chaos:"

cd /home/pi/chaos/raw-gadget/raw_gadget
./insmod.sh

while true;
do
	/home/pi/chaos/build/chaos
done

echo "Done."
echo "--------------------------------------------------------"
