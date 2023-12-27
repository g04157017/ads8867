#!/bin/sh
cd /usr/local/sgw/
./data_call & 
./sgmiii_ex &
./system_monitor >/dev/null  &
./app_ex   &
cd /usr/local/scripts/
./monitor.sh	&
