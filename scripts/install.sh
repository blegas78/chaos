#!/bin/bash

cp chaos.service /etc/systemd/system/
cp chaosface.service /etc/systemd/system/

systemctl daemon-reload
systemctl enable chaos
systemctl enable chaosface

echo "Please reboot the system"
