
#ifndef _TUNTAP_H_
#define _TUNTAP_H_

void TUN_init(char *dev_name);
int TUN_read(char* buf, int len);
int TUN_write(char* buf, int len);

#endif // _TUNTAP_H_
