/var/log/isdn/isdnlog {
    monthly
    missingok
    rotate 36
    compress
    delaycompress
    postrotate
        if [ -s /var/run/isdnlog.isdnctrl0.pid ]; then
            kill -1 `cat /var/run/isdnlog.isdnctrl0.pid` 2>/dev/null
        fi
    endscript
}
