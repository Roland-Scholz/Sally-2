#known issues / quirks:

- Connection to the SIO-bus needs to be revised as some A8 won't communicate properly. Erhard has looked into it and a fix is on its way...

- If a drive is not working properly or a disk media is not in order, a disk format will show ERROR-173 bad sector at format. Clean your drive or use better media.

- Best boot from Sally or first read a sector from one of the drives (e.g. via show Directory) if you booted from another device.
  Otherwise you won't be able to get or set a percom block

- If you want to format HD disks (500khz), make sure the one to be formatted has been formatted in another HD format before (e.g. via a USB PC drive).
  Sally's HD format will be 77Trks/26Sec/256Bytes. This is a workaround as Sally would normally only allow 3600Rpm type floppys to be used with HD.
  8" and 5 1/4" PC/AT drives normally spin at 360rpm. So I implemented a read IDAM in HD, if it succeeds the drive will be considered 360rpm even though it only spins at 300rpm like normal 3 1/2" PC/AT drives do.

- Always configure Percom-Block using "P" and "O" when using another disk geometry. Also consider to write DOS file anew, MyDos will rember last settings then. 
  This is especially important if drive A (boot drive) should use another format.
  
 