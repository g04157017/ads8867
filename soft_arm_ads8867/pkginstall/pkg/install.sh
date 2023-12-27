#!/bin/sh

COMPANY=kyh
PRODUCT=sgw
VERSION=1.0

BAKUP_PKG=/usr/local_bak
APP_PKG=/usr/local
SGW_LIB=/usr/lib
SGW_PKG=$APP_PKG/sgw
SGW_LOG=$APP_PKG/log
SGW_SCRIPT=$APP_PKG/scripts

PKG_NAME=$1
PARA_NUM=$#

echo
echo "             $COMPANY $PRODUCT $VERSION - installation"
echo "  Your software will be installed now, this will take some minutes"
echo "             You can abort at any time with CTRL+C ..."
#
# install help 
#

install_help()
{
    echo 
    echo "usage: ./install.sh kyhl_sgw_v1.0.tar"
    echo 
}
#
# check tar image 
#

check_params()
{
    if [ $PARA_NUM -eq 1 ]
    then
        return 0
	echo "test check params !"
    else
        install_help
        error
    fi
}
check_img()
{
    if test -e $PKG_NAME
    then
        echo "check $PKG_NAME file success..."
        return 0
    else
        install_help
        error
    fi
}
#
# decompress image 
#

extract_img()
{
    if [ $? -eq 0 ]
    then
        tar xvf $PKG_NAME > /dev/null
        echo "extract $PKG_NAME file finished..."
        return 0
    else
        echo "check $PKG_NAME file fail..."
        error
    fi
}

#
# install image  
#

install_img()
{
    if [ $? -eq 0 ]
    then
        rm -rf $BAKUP_PKG
        mv -f $APP_PKG $BAKUP_PKG
        mkdir -p $SGW_PKG
        mkdir -p $SGW_LOG
	mkdir -p $SGW_SCRIPT
	
        cp -r $PWD/lib/* $SGW_LIB/  
        mv -f $PWD/sgw/* $SGW_PKG/
	mv -f $PWD/log/* $SGW_LOG/
        mv -f $PWD/scripts/* $SGW_SCRIPT/
	mv -f $PWD/cfg       $APP_PKG/
	mv -f $PWD/certs     $APP_PKG/
        chmod 755 $SGW_PKG/* -R 
	chmod 755 $SGW_SCRIPT/* -R 
	chmod 755 $SGW_LOG/* -R 
        echo "install $PKG_NAME file finished..."
        return 0
    else
        echo "extract $PKG_NAME file fail..."
        error
    fi
}
#
# startup
#

install_startup()
{
    if [ $? -eq 0 ]
    then
    	ln -f -s $SGW_SCRIPT/startup.sh      /etc/rc5.d/S99SGWStartup.sh
#	sed -i -e 's;/usr/local/sgw/data_call &;;g' -e '/^$/d' /etc/init.d/rc
#	sed -i -e 's;/usr/local/sgw/sgmiii_ex &;;g' -e '/^$/d' /etc/init.d/rc
#	grep -r "data_call" /etc/init.d/rc
#	if [ $? -eq 0 ]
#	then 
#		echo "The data_call has been exist !"
#	else
#		echo "The data_call not  exist !"
#		echo "$SGW_PKG/data_call &" >> /etc/init.d/rc
#	fi
#	grep -r "sgmiii_ex" /etc/init.d/rc
#       if [ $? -eq 0 ]
#       then
#       	echo "The sgmiii_ex  has been exist !"
#       else
#       	echo "The sgmiii_ex not  exist !"
#      	echo "$SGW_PKG/sgmiii_ex &" >> /etc/init.d/rc
#        fi
        echo "install startup files finished..."
        return 0
    else
        echo "install $PKG_NAME file fail..."
        error
    fi
}
#
# clear
#

install_clean()
{
    if [ $? -eq 0 ]
    then
	cd /home/root/kyhl_sgw_pkg/
        rm -rf  $PWD/lib/ 
        rm -rf  $PWD/sgw/   
        rm -rf  $PWD/log/   
        rm -rf  $PWD/scripts/ 
        echo "clean install files finished..."
        return 0
    else
        echo "install $PKG_NAME file fail..."
        error
    fi
}
# Main
main() 
{
    check_params
    sleep 1

    check_img
    sleep 3

    extract_img
    sleep 3

    install_img
    sleep 3

    install_startup
    sleep 3
	
    install_clean
    sleep 3

    finish
}
# Finish
finish()
{
    echo "installation successfully finished..."
    exit 0
}

# Error
error() 
{
    echo "installation error abort..."
    exit 1
}

# Main

main

