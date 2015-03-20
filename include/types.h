#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

#define NULL 0

typedef unsigned short  __kernel_dev_t;
typedef unsigned long   __kernel_ino_t;
typedef unsigned short  __kernel_mode_t;
typedef unsigned short  __kernel_nlink_t;
typedef long            __kernel_off_t;
typedef int             __kernel_pid_t;
typedef unsigned short  __kernel_ipc_pid_t;
typedef unsigned short  __kernel_uid_t;
typedef unsigned short  __kernel_gid_t;
typedef unsigned int    __kernel_size_t;
typedef int             __kernel_ssize_t;
typedef int             __kernel_ptrdiff_t;
typedef long            __kernel_time_t;
typedef long            __kernel_suseconds_t;
typedef long            __kernel_clock_t;
typedef int             __kernel_daddr_t;
typedef char *          __kernel_caddr_t;
typedef unsigned short  __kernel_uid16_t;
typedef unsigned short  __kernel_gid16_t;
typedef unsigned int    __kernel_uid32_t;
typedef unsigned int    __kernel_gid32_t;

typedef unsigned short  __kernel_old_uid_t;
typedef unsigned short  __kernel_old_gid_t;

/*sec define */
typedef __kernel_size_t         size_t;
typedef __kernel_ptrdiff_t      ptrdiff_t;

/* bsd */
typedef unsigned char           u_char;
typedef unsigned short          u_short;
typedef unsigned int            u_int;
typedef unsigned long           u_long;

/* sysv */
typedef unsigned char           unchar;
typedef unsigned short          ushort;
typedef unsigned int            uint;
typedef unsigned long           ulong;

typedef unsigned char			byte;

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

typedef char int8_t;
typedef int int16_t;
typedef long int32_t;


typedef uint8_t le8_t;		/*  8-bit byte */
typedef uint8_t le16_t[2];	/* 16-bit word */
typedef uint8_t le32_t[4];	/* 32-bit dword */



#endif /* _LINUX_TYPES_H */
