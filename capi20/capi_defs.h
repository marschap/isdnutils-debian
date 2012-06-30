/*
 * CAPI 2.0 library
 * 
 * Capi definitions
 *
 * This program is free software and may be modified and 
 * distributed under the terms of the GNU Public License.
 * 2011-03-08 Louis Lagendijk
 */

#ifndef __CAPI_DEFS_H__
#define __CAPI_DEFS_H__

#ifdef HAVE_LINUX_CAPI_H

/*
 * We have the kernal CAPI, use its headers
 */

#define _LINUX_LIST_H
#include <linux/capi.h>

#else
/* make sure uint16_t and uint32_t are defined */
#include <stdint.h>

/* 
 * We do NOT have a kernal capi, provide our own definitions
 * These are derived from the kernal CAPI, only types are changed to 
 * values defined in stdint.h
 */

#define CAPI_MANUFACTURER_LEN   64
#define CAPI_SERIAL_LEN         8

typedef struct capi_version {
        uint32_t majorversion;
        uint32_t minorversion;
        uint32_t majormanuversion;
        uint32_t minormanuversion;
} capi_version;

typedef struct capi_profile {
        uint16_t ncontroller;      /* number of installed controller */
        uint16_t nbchannel;        /* number of B-Channels */
        uint32_t goptions;         /* global options */
        uint32_t support1;         /* B1 protocols support */
        uint32_t support2;         /* B2 protocols support */
        uint32_t support3;         /* B3 protocols support */
        uint32_t reserved[6];      /* reserved */
        uint32_t manu[5];          /* manufacturer specific information */
} capi_profile;

#endif /* KERNEL_CAPI == 1 */

#endif /* __CAPI_DEFS_H */

