#!/bin/bash

# Install services
cp driver/mbdriver.service /etc/systemd/system/mbdriver.service
#systemctl enable mbdriver.service
#systemctl start mbdriver.service
cp graphics/mbapp.service /etc/systemd/system/mbapp.service
#systemctl enable mbapp.service
#systemctl start mbapp.service

