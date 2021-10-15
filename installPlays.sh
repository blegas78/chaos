#!/bin/bash

echo "--------------------------------------------------------"
echo "Installing Plays:"

sudo bash install.sh

cd scripts/
sudo bash installPlays.sh
cd ..


echo "Done."
echo "--------------------------------------------------------"
