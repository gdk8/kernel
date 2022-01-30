cp ndb.ko /lib/modules/`uname -r`/kernel/drivers/target/ndb.ko
# echo ndb >> /etc/modules
depmod
