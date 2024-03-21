#ifndef _DUAL_H_
#define _DUAL_H_

#include <LPC214x.h>
#include "types.h"
void interrupt_handler(void);
s32 password_handle(s32,u8);
s32 check_user(void);
void change_pswd(u32);
void interrupt_handler(void);
void Dual_security_loop(void);
void Dual_Security_Init(void);
#endif
