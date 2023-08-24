#!/bin/bash

FILE="$HOME/Masaüstü/USBDISKKEY"

# init
# look for empty dir 
rm $FILE
if [ -f $FILE ]; then
ls -s /media/usbkeydisk $FILE
chmod 777 $FILE
chown $HOME:$HOME $FILE
     echo "dosya var"
else
ls -s /media/usbdiskkey $FILE
chmod 777 $FILE
chown $HOME:$HOME $FILE
echo "Dosya Kopyalandı.."
   
fi

exit

