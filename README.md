# Fingerprint for wilton

Allow to create fingerprint image using Futronic FD80.
Converts FD80's bitmap image to png, using libpng12.

```
Usage: fingerprint_for_wilton [-h] [-f|--file=STRING] [-t|--timeout=INT] [-h|--help]
  -f, --file=STRING     Name of file to save image. By default: [image.png]
  -t, --timeout=INT     Timeout for fingerprint reading (ms). By default: [10000]
  -h, --help            Show this help message
```

## warning
1. Uses 32-bit library libScanAPI.

2. To use without "sudo" You should add special usb rule. By default FD80 connected in "root" goup under ubuntu, not "dialout".
Add file with rules to "/etc/udev/rules.d", for example:
"42-fingerprint-futronic.rules"
```
ATTR(manufacturer)=="Futronic Technology Company Ltd.", GROUP="dialout"
```

At real machine looks like:
```
/etc/udev/rules.d$ cat 42-fingerprint-futronic.rules
ATTR{manufacturer}=="Futronic Technology Company Ltd.", GROUP="dialout"
```

Now You may add user to dialout group to use fingerprint without sudo rights:
```
  sudo usermod -a -G dialout userName
```

of couse you can use group other than dialout.

### Work with reader based on:
https://github.com/erikssm/futronics-fingerprint-reader

