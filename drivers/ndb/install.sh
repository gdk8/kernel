#!/bin/bash

echo "Installing ndb service (be sure you have root privilege) ..."
GDK_DRV_ROOT=/opt/gedu/gdk/
mkdir -p $GDK_DRV_ROOT
cp -f ndb.ko /$GDK_DRV_ROOT
cp -f ndsrv /$GDK_DRV_ROOT
cp -f ndsrv.service /$GDK_DRV_ROOT

if [ ! -f /$GDK_DRV_ROOT/ndb.ko ]; then
  echo "File [ndb.ko] not copied"
  exit
fi
if [ ! -f /$GDK_DRV_ROOT/ndsrv ]; then
  echo "File [ndsrv] not copied"
  exit
fi
if [ ! -f /$GDK_DRV_ROOT/ndsrv.service ]; then
  echo "File [ndsrv.service] not copied"
  exit
fi

chmod a+x /$GDK_DRV_ROOT/ndsrv
pushd /etc/systemd/system/
ln -s /$GDK_DRV_ROOT/ndsrv.service . 2>/dev/null #we don't care the error
popd
sudo systemctl enable ndsrv.service

cp /$GDK_DRV_ROOT/ndb.ko /lib/modules/$(uname -r)/kernel/drivers/target

GREPED=`fgrep ndb /etc/modules`
if [ "A$GREPED" == "A" ]; then
  echo ndb >>/etc/modules
fi

NDBMODULE=`lsmod | grep ndb`
if [[ $NDBMODULE =~ ndb ]];then
  echo "Remove old ndb module"
  rmmod ndb
fi

depmod
insmod /lib/modules/$(uname -r)/kernel/drivers/target/ndb.ko

echo ""
echo "Done! Press [y] to reboot, or any key else to reboot by yourself:"
read WANTPFF

if [ "A$WANTPFF" == "Ay" ]; then
   history -c
   reboot
fi
