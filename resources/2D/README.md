MAR1D - /resources/2D
===

Files are named `<obj>_<frame>.png` where `<obj>` is the hex value of the char associated with the object (in `/resources/data`)

All objects need at least one 2D file (for the 0th frame), though it can just be a blank .png if the object doesn't need to be displayed

In bash, you can convert from hex to char with `printf "\\x<hex>"` and from char to hex with `printf "%x" "'"'<char>'`
