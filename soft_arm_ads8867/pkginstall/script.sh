#!/bin/bash
cp -r /home/ramandrom/lusxGitRepositories/soft_arm_ads8867/lib/*        $PWD/pkg/lib/
cp -r /home/ramandrom/lusxGitRepositories/soft_arm_ads8867/ql_oe/lib/*  $PWD/pkg/lib/
cp -r /home/ramandrom/lusxGitRepositories/soft_arm_ads8867/output/*     $PWD/pkg/sgw/
cd /home/ramandrom/lusxGitRepositories/soft_arm_ads8867/pkginstall/pkg/
tar -czvf  ../kyhl_sgw_pkg/kyhl_sgw_v2.0.tar *
cp /home/ramandrom/lusxGitRepositories/soft_arm_ads8867/pkginstall/pkg/install.sh    /home/ramandrom/lusxGitRepositories/soft_arm_ads8867/pkginstall/kyhl_sgw_pkg/
cd /home/ramandrom/lusxGitRepositories/soft_arm_ads8867/pkginstall/
tar -czvf kyhl_sgw_pkg.tar.gz  kyhl_sgw_pkg/
rm -rf /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-rootfs/home/root/kyhl_sgw_pkg.tar.gz
cp kyhl_sgw_pkg.tar.gz /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-rootfs/home/root/

