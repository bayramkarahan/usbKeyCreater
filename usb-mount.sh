#!/bin/bash
ACTION=$1
DEVBASE=$2
DEVICE="/dev/${DEVBASE}"
MOUNT_POINT=$(/bin/mount | /bin/grep ${DEVICE} | /usr/bin/awk '{ print $3 }')  # See if this drive is already mounted
mountdir="/media/${DEVBASE}"
case "${ACTION}" in
    add)
        if [[ -n ${MOUNT_POINT} ]]; then exit 1; fi          # Already mounted, exit
         
        eval $(/sbin/blkid -o udev ${DEVICE})                # Get info for this drive: $ID_FS_LABEL, $ID_FS_UUID, and $ID_FS_TYPE
        OPTS="rw,relatime"                                   # Global mount options
        if [[ ${ID_FS_TYPE} == "vfat" ]]; then OPTS+=",users,gid=100,umask=000,shortname=mixed,utf8=1,flush"; fi     # File system type specific mount options
       

    	/bin/mkdir -p ${mountdir}
    	
        if ! /bin/mount -o ${OPTS} ${DEVICE} ${mountdir}
        then 
       
        exit 1
        else
		_file="${mountdir}/.ebaqr"
		if [ -f ${_file} ]
		then
		
			#cryp=$(printf '%s' $ID_FS_PARTUUID | md5sum | cut -d ' ' -f 1)
			seri=$(udevadm info --query=all -n  /dev/${DEVBASE} |grep ID_SERIAL_SHORT=| cut -d '=' -f 2)
			cryp=$(printf '%s' $seri | md5sum | cut -d ' ' -f 1)
			#echo "${DEVBASE}">>/home/by/disk
			#	echo "${cryp}">>/home/by/disk
			#	echo "${seri}">>/home/by/disk
				
			if [ $cryp == $(cat ${_file}) ]
			then
				psw=$(cat /etc/qrpsw)
				/usr/bin/sshlogin ebaqr $psw
				#/usr/bin/sshlogin ebaqr $psw
				
				
			fi
		fi
        fi          # Error during mount process: cleanup mountpoint
        ;;
    remove)
        if [[ -n ${MOUNT_POINT} ]]
        then 
        /bin/umount -l ${DEVICE}
        /bin/rmdir ${mountdir}
        fi
        ;;
esac
