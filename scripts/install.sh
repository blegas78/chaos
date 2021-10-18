#!/bin/bash

cp chaos.service /etc/systemd/system/
cp chaosface.service /etc/systemd/system/

systemctl daemon-reload
systemctl enable chaos
systemctl enable chaosface

if [ -f /etc/systemd/system/plays.service ]; then
	systemctl disable plays
	systemctl disable playsface
fi

echo "Please reboot the system"
