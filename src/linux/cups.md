# cups(1)

## Discover
```sh
# List available printer driver.
lpinfo -m

# List available printer devices (connected + network).
lpinfo -v
```
## Install printer
```sh
# Add device with PRINTER name, practically all modern network printer use the
# everywhere driver.
lpadmin -p PRINTER -m everywhere -v DEVICE_URI

# Delete named printer.
lpadmin -x PRINTER
```

## Printer & Printing options
```sh
# List printer options.
# CHECK printer-make-and-model
lpoptions -p Brother_MFC_L2750DW_series
# List printing options.
lpoptions -p Brother_MFC_L2750DW_series -l

# Set the default printer (stored in ~/.cups/lpoptions).
lpoptions -d Brother_MFC_L2750DW_series
```

## Inspect installed printer.
```sh
# List default printer.
lpstat -d
# List installed printer (-l for longer output).
lpstat -p
# List printer accepting state.
lpstat -a
# List printer and the attached device (eg device uri).
lpstat -v

# List all states at once.
lpstat -t
```
## Print jobs
```sh
# Create print job.
lp -d PRINTER FILE
    -n NUM                           number of copies
    -P PAGE_LIST                     pages to print (eg 1,3-5,10)
    -o media=a4                      paper format
    -o number-up={2|4|6|9|16}        input pages per output page
    -o sides=one-sided               print front-page only
    -o sides=two-sided-long-edge     print duplex

# Remove pending print job.
lprm JOOBID
```

## Control printer
```sh
# Enable/disable printer.
cupsenable PRINTER
cupsdisable PRINTER

# Accept/rejects jobs for printer.
cupsaccept PRINTER
cupsreject PRINTER
```
