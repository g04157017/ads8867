#!/bin/bash
cp -r /home/ramandrom/sharedir/ads131a04_softarm/lib/*        $PWD/pkg/lib/
cp -r /home/ramandrom/sharedir/ads131a04_softarm/ql_oe/lib/*  $PWD/pkg/lib/
cp -r /home/ramandrom/sharedir/ads131a04_softarm/output/*     $PWD/pkg/sgw/
cd /home/ramandrom/sharedir/ads131a04_softarm/pkginstall/pkg/
tar -czvf  ../kyhl_sgw_pkg/kyhl_sgw_v1.0.tar *
cp /home/ramandrom/sharedir/ads131a04_softarm/pkginstall/pkg/install.sh    /home/ramandrom/sharedir/ads131a04_softarm/pkginstall/kyhl_sgw_pkg/
cd /home/ramandrom/sharedir/ads131a04_softarm/pkginstall/
tar -czvf kyhl_sgw_pkg.tar.gz  kyhl_sgw_pkg/
rm -rf /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-rootfs/home/root/kyhl_sgw_pkg.tar.gz
cp kyhl_sgw_pkg.tar.gz /home/ramandrom/sharedir/ql-ol-sdk/ql-ol-rootfs/home/root/

