#!/bin/bash


systemctl disable chaos
systemctl disable chaosface


cp plays.service /etc/systemd/system/
cp playsface.service /etc/systemd/system/

systemctl daemon-reload
systemctl enable plays
systemctl enable playsface

echo "Please reboot the system"
