#!/bin/sh
DATA_CALL_PROC_NAME=data_call
SGMIII_PROC_NAME=sgmiii_ex
SYSTEM_MONITOR_PROC_NAME=system_monitor
APP_EX_PROC_NAME=app_ex
COUNT_RECORD=0
PING_UNCONNECT=0

SYSTEM_MONITOR_COUNT=0
APP_EX_COUN=0

# data_call_proc
data_call_proc()
{
	ProcNumber=`ps -ef |grep -w $DATA_CALL_PROC_NAME|grep -v grep|wc -l`
	if [ $ProcNumber -le 0 ];then
		echo "data_call is not run"
		reset_data_call
	else
		echo "data_call is  running.."
	fi
}

#reset_data_call

reset_data_call()
{
	echo "you can do something in this file !"
	cd /usr/local/sgw/
	./data_call &
}

# sgmiii_proc
sgmiii_proc()
{
	ProcNumber=`ps -ef |grep -w $SGMIII_PROC_NAME|grep -v grep|wc -l`
	if [ $ProcNumber -le 0 ];then
		echo "sgmiii is not run"
		reset_sgmiii
	else
		echo "sgmiii is  running.."
	fi
}

#reset_sgmiii

reset_sgmiii()
{
	echo "you can do something in this file !"
	cd /usr/local/sgw/
	./sgmiii_ex &
}
# system_monitor_proc
system_monitor_proc()
{
	ProcNumber=`ps -ef |grep -w $SYSTEM_MONITOR_PROC_NAME|grep -v grep|wc -l`
	if [ $ProcNumber -le 0 ];then
                echo "system_monitor is not run"
		SYSTEM_MONITOR_COUNT=$((${SYSTEM_MONITOR_COUNT} + 1))
                reset_system_monitor
		if [ $SYSTEM_MONITOR_COUNT -ge 4 ];then
			reset_device			
		fi
        else
#                echo "system_monitor is  running.."
		SYSTEM_MONITOR_COUNT=0
        fi
}
#reset_system_monitor
reset_system_monitor()
{
	echo "now begain to reboot the system_monitor !"
	cd /usr/local/sgw/
	./system_monitor >/dev/null  &
}

# app_ex_proc
app_ex_proc()
{
        ProcNumber=`ps -ef |grep -w $APP_EX_PROC_NAME|grep -v grep|wc -l`
        if [ $ProcNumber -le 0 ];then
                echo "app_ex is not run"
		APP_EX_COUN=$((${APP_EX_COUN} + 1))
                reset_app_ex
		if [ $APP_EX_COUN -ge 4 ];then
			reset_device
		fi
        else
 #               echo "app_ex is  running.."
		APP_EX_COUN=0
        fi
}
#reset_app_ex
reset_app_ex()
{
      echo "now begain to reboot the app_ex  !"
	cd /usr/local/sgw/
	./app_ex   &
}


#net_work_proc
net_work_proc()
{
	COUNT_RECORD=$((${COUNT_RECORD} + 1))
#	echo "+++++++++++++++++++++++++++++++++++++++++++++COUNT_RECORD = $COUNT_RECORD !+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	if [ $COUNT_RECORD -ge 10 ];then
		COUNT_RECORD=0
		ping -c 1 www.baidu.com
		if [ $? -ne 0 ];then
                	echo "now the network has Disconnection !"
                	PING_UNCONNECT=$((${PING_UNCONNECT} + 1))
			echo "*--------------------------------------------- PING_UNCONNECT = $PING_UNCONNECT !------------------------------------------*"
                	if [ $PING_UNCONNECT -ge 4 ];then
				PING_UNCONNECT=0
                        	reset_net_work
                	fi
		else
#                	echo "the network is run  connected !"
			 PING_UNCONNECT=0
		fi
        fi
}
#reset_net_work
reset_net_work()
{
	echo "now begain to reboot the device !"
	reboot
	exit 1
}

# RAM_monitor_proc
RAM_monitor_proc()
{
	total=$(free -m | sed -n '2p' | awk '{print $2}')
	used=$(free -m | sed -n '2p' | awk '{print $3}')
	free=$(free -m | sed -n '2p' | awk '{print $4}')
	shared=$(free -m | sed -n '2p' | awk '{print $5}')
	buff=$(free -m | sed -n '2p' | awk '{print $6}')
	cached=$(free -m | sed -n '2p' | awk '{print $7}')
	rate=`awk 'BEGIN{printf "%d\n",(100*'$used' / '$total')}'`
#	echo -e "total	used	free	shared	buffer	available"
#	echo -e "${total}M	${used}M	${free}M	${shared}M	${buff}M	${cached}M	rate:${rate}%"
	
	if [ $rate -ge 94 ]
		then    echo "Memory Warn"
		ps aux | grep -v USER | sort -rn -k4 | head
		reset_RAM_monitor_proc
	fi		
}
#reset_RAM_monitor_proc
reset_RAM_monitor_proc()
{
	echo "now begain to reboot the device !"
	reset_device
}


#reset_device
reset_device()
{
	reboot
	exit 1
}

# Main
main()
{
	sleep 60
	data_call_proc
#	sgmiii_proc
	while true;do
	sleep 30
#	echo "this was a test !"
#	data_call_proc
	RAM_monitor_proc
	net_work_proc
	system_monitor_proc
	app_ex_proc
	done
}
# Main
main
