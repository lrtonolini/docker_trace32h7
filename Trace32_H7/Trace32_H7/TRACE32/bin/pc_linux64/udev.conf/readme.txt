
  Lauterbach rules files for udev
  ===============================

Choose the appropriate 10-lauterbach.rules file from the subdirectories here
according to the version of your Linux kernel ('uname -r') and the version 
of udev (try '/sbin/udevadm -V', 'udevinfo -V' or '/sbin/udevd -V').
Then copy the chosen file to the /etc/udev/rules.d/ directory. For this you
need 'root' (superuser) privileges.

Example:

	sudo cp 10-lauterbach.rules /etc/udev/rules.d/

