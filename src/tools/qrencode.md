# qrencode(1)

Generate wifi qr code for WPA2 secured network.
```sh
# Generate on terminal.
qrencode -t ansiutf8 'WIFI:S:<wifiname>;T:WPA2;P:<wifipasswd>;;'

# Generate picture for printing.
qrencode -t png -o wifi.png 'WIFI:S:<wifiname>;T:WPA2;P:<wifipasswd>;;'
```
