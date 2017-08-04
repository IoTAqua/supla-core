
Installation of supla-scheduler
===============================

    ./build.sh
    sudo cp Release/supla-scheduler /usr/sbin/
    sudo cp supla-min supla-daily supla-scheduler /etc/cron.d/
    sudo cp supla-scheduler-init /etc/init.d/supla-scheduler
    sudo update-rc.d supla-scheduler defaults
