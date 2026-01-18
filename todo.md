# to be implemented:
- SIO connection need a signal shaping switching transistor (inverting) oder two (non-invertng) per line (SIOOUT, CMD, RDY)

- Tests for disk geometry:
 	- does the drive spin at 360 RPM (Yes -> HD, 500 Kbit MFM) (No -> it's a 300 RPM drive)
 	- test for HD (at 300 RPM) (more than 18 sectors/track, 36 for the HDI)
 	- test for DD (250 Kbit MFM 256 bytes/sector)
 	- test for MD (250 Kbit MFM 128 bytes/sector)
 	- test for SD (125 Kbit FM 128 bytes/sector)
 	- After each successful test (on track 0 side 0), the number of tracks is then determined (on side 0) (possibly check for 35 track drive)
 	- is track 40 (which is the 41st track) readable with these settings (No: 40 track drive, Yes: possibly 77 track drive)
 	- is track 77 (which is the 78th track) readable with these settings (No: 77 track drive, Yes: 80 track drive)
 	- After the last successful track test, it must be checked whether this track is also readable on side 1 (No: single sided, Yes: double sided)
 	- Now it's possible that no disk or an unformatted disk is inserted (a problem I didn't have with the HDI, since the disk-change signal is required and used there).
 	- For this case, perhaps a PERCOM block with zeros should be created (or 40 TR, SSSD). The user will simply need to know what the drive is capable of when formatting.

- HD switching:
	If the format command is preceded by a PERCOM block with more than 18 sectors of 256 bytes per sector, the FDC must be switched to HD mode.