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
Uses 32-bit library libScanAPI.
