#!/bin/bash

echo "--------------------------------------------------------"
echo "Installing Plays:"

bash install.sh

cd scripts/
sudo bash installPlays.sh
cd ..


echo "Done."
echo "--------------------------------------------------------"
