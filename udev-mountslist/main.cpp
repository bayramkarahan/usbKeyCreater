#include <QCoreApplication>

#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <sys/statvfs.h>
#include <string_view>
#include <optional>
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
// g++ udev-mountslist -ludev -lcrypto -lssl

using namespace std;
bool disk_add_drm=false;
bool disk_remove_drm=false;
std::string login_serial="";
bool login_status=false;
static
struct udev_device*
get_child(struct udev* udev, struct udev_device* parent, const char* subsystem) {
  struct udev_device* child = NULL;
  struct udev_enumerate *enumerate = udev_enumerate_new(udev);

  udev_enumerate_add_match_parent(enumerate, parent);
  udev_enumerate_add_match_subsystem(enumerate, subsystem);
  udev_enumerate_scan_devices(enumerate);

  struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
  struct udev_list_entry *entry;

  udev_list_entry_foreach(entry, devices) {
      const char *path = udev_list_entry_get_name(entry);
      child = udev_device_new_from_syspath(udev, path);
      break;
  }

  udev_enumerate_unref(enumerate);
  return child;
}

static void enumerate_usb_mass_storage(struct udev* udev) {
  struct udev_enumerate* enumerate = udev_enumerate_new(udev);

  udev_enumerate_add_match_subsystem(enumerate, "scsi");
  udev_enumerate_add_match_property(enumerate, "DEVTYPE", "scsi_device");
  udev_enumerate_scan_devices(enumerate);

  struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
  struct udev_list_entry *entry;

  udev_list_entry_foreach(entry, devices) {
      const char* path = udev_list_entry_get_name(entry);
      struct udev_device* scsi = udev_device_new_from_syspath(udev, path);

      struct udev_device* block = get_child(udev, scsi, "block");
      struct udev_device* scsi_disk = get_child(udev, scsi, "scsi_disk");

      struct udev_device* usb
          = udev_device_get_parent_with_subsystem_devtype(scsi, "usb", "usb_device");

      if (block && scsi_disk && usb) {
          printf("block = %s, usb = %s:%s, scsi = %s, name = %s, size = %s, x = %i, x = %s, x = %s\n",
                 udev_device_get_devnode(block),
                 udev_device_get_sysattr_value(usb, "idVendor"),
                 udev_device_get_sysattr_value(usb, "idProduct"),
                 udev_device_get_sysattr_value(scsi, "vendor"),
udev_device_get_sysattr_value(usb, "udev"),
udev_device_get_sysattr_value(usb, "udev_root"),
udev_device_get_sysattr_value(usb, "size"),
udev_device_get_sysattr_value(usb, "speed"),
udev_device_get_sysattr_value(usb, "bMaxPower"));

      }

      if (block) {
          udev_device_unref(block);
      }

      if (scsi_disk) {
          udev_device_unref(scsi_disk);
      }
      udev_device_unref(scsi);
  }

  udev_enumerate_unref(enumerate);
}

/*************************************mounts list**************************************/
std::optional<std::string> get_device_of_mount_point(std::string_view devicepath)
{
   std::ifstream mounts{"/proc/mounts"};
   std::string device, mountpoint, type;
   std::string line;


if (mounts.is_open()) {
    while (std::getline(mounts, line)) {

    std::istringstream iss(line);

    getline(iss, device, ' ');
    getline(iss, mountpoint, ' ');
    getline(iss, type, ' ');
         if (device == devicepath)
         {
            //std::cout << device << '|'<< mountpoint << '|'<< type<< std::endl;
            return mountpoint;
         }
    }
    mounts.close();
}

  return std::nullopt;
}
/******************************************************************************************/
/*************************************md5 cmp**************************************/
bool exist_md5(std::string mountpoint,std::string_view md5kod)
{
mountpoint.append("/.ebaqr");

   std::ifstream file(mountpoint);
    std::string line;


if (file.is_open()) {
    while (std::getline(file, line)) {

    std::istringstream iss(line);

  //  getline(iss, device, ' ');
  //  getline(iss, mountpoint, ' ');
  //  getline(iss, type, ' ');
         if (line == md5kod)
         {
            //std::cout << "kod var açılabilir "<< std::endl;
            return true;
         }
    }
    file.close();
}

  return false;
}
/******************************************************************************************/
/*************************************qrpsw cmp**************************************/
std::string get_qrpsw()
{

   std::ifstream file("/etc/qrpsw");
    std::string line;


if (file.is_open()) {
    while (std::getline(file, line)) {

                return line;

    }
    file.close();
}

  return "";
}
/******************************************************************************************/
/*************************************get_umountdisk_command cmp**************************************/
std::string get_umountdisk_command()
{

   std::ifstream file("/usr/share/usbkeycreater/command.conf");
    std::string line;


if (file.is_open()) {
    while (std::getline(file, line)) {

                return line;

    }
    file.close();
}

  return "";
}
/******************************************************************************************/
/****************************************md5sum********************************************/
char *str2md5(const char *str, int length) {
    int n;
    MD5_CTX c;
    unsigned char digest[16];
    char *out = (char*)malloc(33);

    MD5_Init(&c);

    while (length > 0) {
        if (length > 512) {
            MD5_Update(&c, str, 512);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= 512;
        str += 512;
    }

    MD5_Final(digest, &c);

    for (n = 0; n < 16; ++n) {
        snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    return out;
}
/******************************************************************************************/
int main(int argc, char *argv[]){

    QCoreApplication a(argc, argv);



    struct udev* udev = udev_new();
    enumerate_usb_mass_storage(udev);


    struct udev_device *dev;
    struct udev_monitor *mon;
    int fd;

    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", NULL);
    udev_monitor_enable_receiving(mon);
    fd = udev_monitor_get_fd(mon);

    while (1) {

        fd_set fds;
        struct timeval tv;
        int ret;

        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        ret = select(fd+1, &fds, NULL, NULL, &tv);

        /* Check if our file descriptor has received data. */
        if (ret > 0 && FD_ISSET(fd, &fds))
        {
            printf("\nselect() says there should be data\n");

            dev = udev_monitor_receive_device(mon);
            if (dev) {
                printf("Got Device\n");
                printf("   Node: %s\n", udev_device_get_devnode(dev));
                printf("   Subsystem: %s\n", udev_device_get_subsystem(dev));
                printf("   Devtype: %s\n", udev_device_get_devtype(dev));
                printf("   Action: %s\n", udev_device_get_action(dev));
                ///cout << udev_get_run_path (dev) << endl;

                if( strcmp(udev_device_get_action(dev),"add")==0 && strcmp(udev_device_get_devtype(dev),"partition")==0)
                {
                    disk_add_drm=true;
                    cout << "conectat" << endl;
                    //tmp/addusb
                    system("umount /media/usbkeydisk");

                    system("mkdir /media/usbkeydisk");
                    std::string komut="mount ";
                    komut.append(udev_device_get_devnode(dev));
                    komut.append(" /media/usbkeydisk");

                    //printf("komut: %s",komut.c_str());
                    //std::cout <<"komut:"<<komut<< "\n";
                    system(komut.c_str());
                    printf("disk bağlandı..");
                }
                if( strcmp(udev_device_get_action(dev),"remove")==0 )
                {
                    disk_remove_drm=true;
                    auto sserial=udev_device_get_property_value(dev, "ID_SERIAL_SHORT");
                    //login olunmuş ve login olan disk ile çıkartılan disk aynıysa logout yap
                    std::string kmt="umount ";
                        kmt.append(udev_device_get_devnode(dev));
                        system(kmt.c_str());
                        //system("systemctl restart usb-mount.service|true");
                      if(sserial==login_serial&&login_status)
                    {
                        login_serial="";
                        login_status=false;
                        disk_add_drm=false;
                        printf("oturum açılan disk umount yapıldı");
                        //usb disk çıkartıldığında çalışacak komut çalıştırılıyor...
            //system("loginctl terminate-seat seat0");
            system(get_umountdisk_command().c_str());

                    }

                }
                /********************************disk bağlandımı*****************************/
                if( strcmp(udev_device_get_action(dev),"add")==0&&(disk_add_drm==true&&login_status==false) )
                {
                    disk_add_drm=false;
                    auto sserial=udev_device_get_property_value(dev, "ID_SERIAL_SHORT");
                    /********************************md5*****************************************************/
                    auto md5kod = str2md5(sserial, strlen(sserial));
                    /******************************************disk bağlantı noktası öğrenme erişme kod kontrolü**************/
                    if (const auto point = get_device_of_mount_point(udev_device_get_devnode(dev)))
                    {
                        // std::cout << *point << "\n";
                        bool durum=exist_md5(*point,md5kod);//disk içindeki md5 kontrol ediliyor
                        //durum==true dosya var ve md5 doğruysa login yap
                        if(durum)
                        {
                            login_status=true;
                            login_serial=sserial;
                            std::string qrpsw=get_qrpsw();
                            //std::cout <<"login yapılabilir disk uygun"<<qrpsw<< "\n";
                            std::string komut="/usr/bin/sshlogin ebaqr ";
                            komut.append(qrpsw);
                            //printf("komut: %s",komut.c_str());
                            //std::cout <<"komut:"<<komut<< "\n";
                            system(komut.c_str());
                            //system("ln -s /media/usbkeydisk /ortak-alan");

                        }
                    }
                    /****************************************************************************************************/


                }

                udev_device_unref(dev);
            }
            else {
                printf("No Device from receive_device(). An error occured.\n");
            }
        }
        usleep(250*1000);
        fflush(stdout);
    }




    udev_unref(udev);

     return a.exec();
    //return 0;
}
/*int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}*/

