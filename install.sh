#!/bin/bash

echo "--------------------------------------------------------"
echo "Installing Chaos:"

#build dependencies:
declare -a depencencies=(build-essential libncurses5-dev libusb-1.0-0-dev cmake git libzmq3-dev raspberrypi-kernel-headers python3-dev libatlas-base-dev python3-pip)
toInstall=()
echo "Dependencies:" ${depencencies[@]}
for dependency in "${depencencies[@]}"
do
	echo "Checking for" $dependency
	if [ $(dpkg-query -W -f='${Status}' $dependency 2>/dev/null | grep -c "ok installed") -eq 0 ];
	then
		echo "not installed:" $dependency
		toInstall+=($dependency)
	fi
done
echo ${toInstall[@]}

if [ ${#toInstall[@]} -ne 0 ];
then
	sudo apt-get update
	sudo apt-get install -y ${toInstall[@]}
fi

git submodule init
git submodule update

if [ ! -d "build" ];
then
	mkdir build
fi
cd build
cmake ..
make -j4
make install
cd ..


# add dwc2 to /boot/config.txt
sudo -s eval "grep -qxF 'dtoverlay=dwc2' /boot/config.txt  || echo 'dtoverlay=dwc2' >> /boot/config.txt "


sudo -s eval "pip3 install flexx pyzmq numpy --system"

if [ ! -f "/home/pi/chaosConfig.json" ];
then
	cp blankConfig.json /home/pi/chaosConfig.json
fi

if [ ! -d "/home/pi/chaosLogs" ];
then
	mkdir /home/pi/chaosLogs
fi

cd scripts/
sudo bash install.sh
cd ..

#raw-gadget patch:
patch raw-gadget-timeout/raw_gadget/raw_gadget.c raw_gadget_timeout.patch
cd raw-gadget-timeout/raw_gadget/
make
cd ..

echo "Done."
echo "--------------------------------------------------------"
