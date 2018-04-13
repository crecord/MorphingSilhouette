#!/bin/bash
sudo apt install python-pip
pip install slackclient
sudo apt install wmctrl
crontab new.crontab
COMP_NAME='Rogers' python /home/studio/Documents/of_v20170714_linux64gcc5_release/apps/myApps/MorphingSilhouette/bin/slack/postImageToSlack.py
