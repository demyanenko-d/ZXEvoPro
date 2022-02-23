## Running IDF environment

Run **idf.bat** from a *project dir* with Right Shift.

`idf.bat`

```shell
call d:\SoC\ESP32\idf\install.bat
call d:\SoC\ESP32\idf\export.bat
```

Change **d:\SoC\ESP32\idf** to your IDF path.



## Compilation

To build and flash app + bootloader (must be only run once, unless you change bootloader options):

`idf.py -p com9 flash`

To build and flash only app:

`idf.py -p com9 app-flash`

Change **com9** to your serial adapter port.



**WARNING:** `idf.py set-target` overwrites **sdkconfig**, be careful running it.