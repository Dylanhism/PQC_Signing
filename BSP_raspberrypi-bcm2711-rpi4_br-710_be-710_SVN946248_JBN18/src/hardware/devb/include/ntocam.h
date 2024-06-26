/*
 * $QNXLicenseC: 
 * Copyright 2008, QNX Software Systems.  
 *  
 * Licensed under the Apache License, Version 2.0 (the "License"). You  
 * may not reproduce, modify or distribute this software except in  
 * compliance with the License. You may obtain a copy of the License  
 * at: http://www.apache.org/licenses/LICENSE-2.0  
 *  
 * Unless required by applicable law or agreed to in writing, software  
 * distributed under the License is distributed on an "AS IS" basis,  
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied. 
 * 
 * This file may contain contributions from others, either as  
 * contributors under the License or as licensors under other terms.   
 * Please review this entire file for other proprietary rights or license  
 * notices, as well as the QNX Development Suite License Guide at  
 * http://licensing.qnx.com/license-guide/ for other information. 
 * $ 
 */

#ifndef __NTO_CAM_H_INCLUDED
#define __NTO_CAM_H_INCLUDED

#include <stddef.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/iomsg.h>
#include <sys/resmgr.h>
#include <sys/queue.h>
#include <sys/syspage.h>
#include <sys/dcmd_cam.h>
#include <sys/slogcodes.h>
#include <sys/cam_device.h>

#include <cam.h>
#include <xpt.h>
#include <ntoscsi.h>

#define _NTO_CAM_VERSION					100

#define XSTR(x)								#x
#define CAM_STRINGIZE(x)					XSTR(x)
#define CAM_ARRAY_SIZE( _x )				( sizeof( _x ) / sizeof( ( _x )[0] ) )

#define CAM_PDRV_VERSION					0x4
#define CAM_INVALID_NUM						-0xBAD1
#define CAM_INVALID_NUM64					-0xBAD1LL

#define CAM_ALIGN( _val, _align )			CAM_ALIGN_MASK( _val, ( typeof( _val ) )(_align) - 1 )
#define CAM_ALIGN_MASK( _val, _mask )		( ( (_val) + (_mask) ) & ~(_mask) )

#define CAM_ENDIAN_SWAP32( _d, _c )			do { ENDIAN_SWAP32( (_d) ); (_d)++; } while( --(_c) )

#define CAM_L32_BITS( _b )					((_Uint32t)(_b))
#define CAM_U32_BITS( _b )					((_Uint32t)((_b) >> 32))

#define CAM_BITMASK_LE64( _hbit, _lbit )	( ( ( ~0ULL ) << ( _lbit ) ) & ( ~0ULL >> ( 64 - ( _lbit ) - (_hbit) ) ) )
#define CAM_BITMASK_LE32( _hbit, _lbit )	( ( ( ~0UL ) << ( _lbit ) ) & ( ~0UL >> ( 32 - ( _lbit ) - (_hbit) ) ) )
#define CAM_BIT_LE64( _bit )				_ONEBIT64L( _bit )
#define CAM_BIT_LE32( _bit )				_ONEBIT32L( _bit )
#define CAM_BIT_BE64( _bit )				_ONEBIT64B( _bit )
#define CAM_BIT_BE32( _bit )				_ONEBIT32B( _bit )

#define CAM_STACK_SIZE						16384
#define CAM_TSTATE_CREATING					0x00
#define CAM_TSTATE_INITIALIZED				0x01
#define CAM_TSTATE_INIT_FAILURE				0x02

#define SCSI_IO_CBF_PRIORITY				21
#define SCSI_IO_CBF_PULSE_CODE				0x73


// Useful TAILQ manifests possibly not in sys/queue.h
#ifndef TAILQ_EMPTY
	#define	TAILQ_EMPTY(head) ((head)->tqh_first == NULL)
#endif
#ifndef TAILQ_FIRST
	#define	TAILQ_FIRST(head) ((head)->tqh_first)
#endif
#ifndef TAILQ_LAST
	#define	TAILQ_LAST(head) ((head)->tqh_last)
#endif
#ifndef TAILQ_NEXT
	#define	TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)
#endif
#ifndef TAILQ_PREV
	#define TAILQ_PREV(elm, field) ((elm)->field.tqe_prev)
#endif

typedef struct _devobj_extension			DEVEXT;
typedef struct _devobj						DEVOBJ;
typedef struct _pdrvr						PDRVR;
typedef struct _peripheral_driver_extension	PDRVR_EXT;
typedef struct _scsi_req_block				SRB;

#define MAX_RETRIES					10
#define SCSI_START_UNIT_TIMEOUT		30
#define SCSI_LOAD_TIMEOUT			30
#define BLOCK_SIZE					512
#define BOUNCE_BUF_SIZE				32768
#define BOUNCE_X64_BUF_SIZE			65536	// Max cross 64k bounce buffer size
#define UNLOCK_MEDIA				0
#define LOCK_MEDIA					1
#define LOAD_MEDIA					1
#define EJECT_MEDIA					0
#define MAX_TARGET					16		// Max number of SCSI targets
#define MAX_WIDE_TARGET				16		// Max number of wide SCSI targets
#define MAX_NARROW_TARGET			8		// Max number of narrow SCSI targets
#define MAX_LUN						8		// Max number of LUNs per target
#define CMD_6_BYTE					0
#define CMD_10_BYTE					1
#define MAX_IO_BLKS					4096
#define MAX_SG						256
#define MAX_SG_EXP					31
#define MAX_QDEPTH_EXP				31
#define QDEPTH_DFLT					4

static inline paddr64_t cam_mphys( const void *addr )
{
	off64_t offset;

	if( mem_offset64( addr, -1, 1, & offset, 0 ) == -1 ) {
		return( (paddr64_t) -1 );
	}

	return( offset );
}

int cam_cdb( CCB *);
#define cam_cdb( ccb )				( (CDB *)ccb->cam_cdb_io.cam_cdb_bytes )

int cam_cdb_lun( DEVEXT *);				// only set lun in the cdb when SCSI version < 2
#define cam_cdb_lun( dext )			( ( (dext)->version & INQ_VANSI ) < 2 ? SC_LUN_CDB( (dext)->lun ) : 0 )

void cam_set_pdrvr( CCB *, void *);
#define cam_set_pdrvr( ccb, pdrv )	( ccb->cam_pdrv_ptr = (void *)pdrv )

void cam_set_cbf( CCB *, void *);
#define cam_set_cbf( ccb, cbf )		( ccb->cam_cbfcnp = (void *)cbf )

void cam_data_ptr( CCB_SCSIIO *, void *);
#define cam_data_ptr( ccb, data )	ccb->cam_data.cam_data_ptr = 					\
									( ccb->cam_ch.cam_flags & CAM_DATA_PHYS ) ?		\
									cam_mphys( (void *)(uintptr_t)(data) ) : (uintptr_t)(data)


#define cam_clear_ccb(_x)			( memset( ((char *)(_x)) +							\
									offsetof(struct ccb_header, cam_func_code), 0x00,	\
									_x->cam_ch.cam_ccb_len -							\
									offsetof(struct ccb_header, cam_func_code)) )

void cam_set_sense(CCB *, void *);
#define cam_set_sense(ccb, sense)	( ((CCB_SCSIIO *)ccb)->cam_sense_ptr =	\
									(unsigned char *)(sense),				\
									(((CCB_SCSIIO *)ccb)->cam_sense_len =	\
									sizeof(*(sense))) )

void cam_target( CCB_SCSIIO *, DEVEXT * );
#define cam_target( ccb, dext )		( (ccb)->cam_ch.cam_path_id		= (dext)->path_id,	\
									  (ccb)->cam_ch.cam_target_id	= (dext)->target,	\
									  (ccb)->cam_ch.cam_target_lun	= (dext)->lun,		\
									  (ccb)->cam_vu_flags			= CAM_VUF_OTHER )

typedef struct _cam_entry CAM_ENTRY;

typedef struct _cam_entry {
	int		nfuncs;
	int		(*scsi_scan_bus)( int path );
	int		(*scsi_scan_devs)( int type, int (*func)( CCB_PATHINQ *, CCB_GETDEV *));
	int		(*scsi_setasync_cb)( ASYNC_INFO **alist, CCB_SETASYNC *ccb );
	CCB		*(*scsi_ccb_alloc)(void);
	void	(*scsi_ccb_free)(CCB *);
	void	*(*scsi_malloc)( int size, int mflags );
	void	(*scsi_free)( void *addr, int size );
	DEVOBJ	*(*scsi_create_device)( CCB_PATHINQ *ccb_path, CCB_GETDEV *ccb_getdev, int ext_size, int max_io );
	int		(*scsi_io)( void *hdl, ioreq_t *ioreq );
	int		(*scsi_abort)( void *hdl, ioreq_t *ioreq, unsigned flags );
	int		(*scsi_devctl)(void *hdl, resmgr_context_t *ctp, int ioflag, ioreq_t *tag, unsigned dcmd, void *data, size_t size, int *ret );
	int		(*scsi_iomsg)( void *hdl, resmgr_context_t *ctp, int ioflag, ioreq_t *tag, io_msg_t *msg );
	int		(*scsi_interpret_sense)( DEVOBJ *dobj, CCB_SCSIIO *ccb, int *status, int rcount );

	int		(*scsi_action)( DEVOBJ *dobj, CCB_SCSIIO *ccb );
	int		(*scsi_action_wait)( DEVOBJ *dobj, CCB_SCSIIO *ccb, int retries );
	int		(*scsi_release_simq)( DEVOBJ *dobj );
	int		(*scsi_read_capacity)( DEVOBJ *dobj );
	int		(*scsi_mode_sense)( DEVOBJ *dobj, int page, char *mdata, int len );
	int		(*scsi_mode_select)( DEVOBJ *dobj, char *pdata, int len, int save );
	void	*(*scsi_mode_page)( char *msdata, int page, int len, int type );
	int		(*scsi_reserve)( DEVOBJ *dobj );
	int		(*scsi_release)( DEVOBJ *dobj );
	int		(*scsi_prevent)( DEVOBJ *dobj, int prevent );
	int		(*scsi_synchronize_cache)( DEVOBJ *dobj, baddr_t *lba );
	int		(*scsi_set_timeout)( DEVOBJ *dobj, int g1, int g2, int g3 );
} cam_entry_t;

#define SRB_FLG_BOUNCE		0x01		// bounce data to/from cache blocks
#define SRB_FLG_CACHE		0x02		// cache needs to be flushed/invalidated
struct _scsi_req_block {
	DEVOBJ			*dobj;
	ioreq_t			*ioreq;
 	CCB_SCSIIO		ccb;
	int				flags;
	int				nblks;
	mdl_t			*mdl;

	mdl_t			*mdl_nxt;
	int				mdl_off;		// current mdl offset
	int				mdl_noff;		// next mdl offset

	unsigned		rcount;			// retry count
	unsigned		b_blkoff;		// block offset in ioreq
	SG_ELEM			*sg_list;
	SCSI_SENSE		sense;
	ioreq_t			bioreq;
	mdl_t			bmdl;
	int				bmd_size;
	void			*bmd_vaddr;
};

// This extension structure is specific to each peripheral driver.
// Peripheral drivers that use blkio can use the following structure.
struct _peripheral_driver_extension {
	PDRVR					*pdrv;
	char					*prefix;
	uint8_t					*async_buffer;	// requires 22 bytes for AEN
	const io_entry_t		*io_funcs;
	cam_entry_t				*cam_funcs;
	void					*dll;			// io-blk dll handle
	int						dcount;			// device count
	int						argc;
	char					**argv;
	int						rsvd[4];
};

// This structure is common to all peripheral drivers.  It must
// contain at least the following members at the start of the
// structure.
struct _pdrvr
{
	PDRVR_EXT				*pext;
	TAILQ_HEAD(,_devobj)	dlist;			// linked list of devices
	int						pflags;			// various flags
	int						rsvd[4];
};
#define PDRVR_FLG_MEDIA_CHECK		0x01

struct _devobj {
	PDRVR					*pdrvr;			// pointer to peripheral driver
	TAILQ_ENTRY(_devobj)	dlink;			// pointer to next device
	void					*dext;			// device extension
	void					*dentry;
	int						verbosity;
	int						vflags;
	int						rsvd[3];
};

// This structure may be different for each peripheral type.
// Devices that require io-blk can use the following structure.
// Note:  Device extenstions must include at least the common
// members at the start of the structure.
#define DEXT_EFLG_INITIALIZING		0x01
#define DEXT_EFLG_NOSYNC			0x02	// don't enable sync/wide transfers
#define DEXT_EFLG_NOBIOS			0x04	// don't use BIOS for translation
#define DEXT_EFLG_NOPTAB			0x08	// don't use partition table for translation
#define DEXT_EFLG_TP_SUP			0x20	// timeout and protect mode page support
#define DEXT_EFLG_GET_EVENT_SUP		0x40    // get event/status notification
#define DEXT_EFLG_BUS_RESET			0x80
#define DEXT_EFLG_MP_FLEX_DSK_SUP	0x100	// mode sense flexible page support
#define DEXT_EFLG_NO_FLUSH			0x200
#define DEXT_EFLG_VERIFY			0x400
#define DEXT_EFLG_VERIFY_BYTCHK		0x800
#define DEXT_EFLG_TRIM_SUP			0x1000
#define DEXT_EFLG_RDCAP16_SUP		0x2000
#define DEXT_EFLG_TRIM_UNMAP		0x4000

struct _devobj_extension {
	DEVOBJ					*dobj;			// Device object
	void					*mext;			// Model extension
	void					*mentry;		// Model extension
	int						eflags;			// extension flags
	path_id_t				path_id;		// Cam path id to device
	target_id_t				target;			// SCSI target id of device
	lun_id_t				lun;			// SCSI logical unit number
	int						dtype;			// Peripherial Device Type
	int						dflags;			// Device flags ie read only, no media
	int						cflags;			// CAM flags ie CAM_DIS_DISCONNECT, CAM_DIS_SYNC
	int						mflags;			// Memory allocation flags
	uint64_t				paddr_msk;		// paddr msk used to determine bounce buffer usage
											// (derived from hc_paddr_msk, bounce buffer command line, memory attributes)

#define CAM_G1_TIMEOUT_DFLT		60
#define CAM_G2_TIMEOUT_DFLT		90
#define CAM_G3_TIMEOUT_DFLT		10
#define CAM_RW_TIMEOUT_DFLT		10
	int						g1_timeout;		// Current group 1 timeout (in seconds)
	int						g2_timeout;		// Current group 2 timeout (in seconds)
	int						g3_timeout;		// Current group 3 timeout (in seconds)
	int						rw_timeout;		// read/write timeout

	int						max_sg;			// Maximum scatter/gather hba can handle
	int						max_io_blks;	// Maximum blocks device can handle (Obsolete)
	int						max_active;		// Maximum number of active srbs
	int						max_speed;

	int						locks;			// Number of media locks
	int						media_changes;	// Number of media changes
	int						retries;
	int						starvation;

	ioque_t					queue;			// Request queue
	ioque_t					aqueue;			// Active request queue
	void					*srb_pool;		// IO request pool

	cam_devinfo_t			dinfo;			// Device info (geometry)

	int						(*error)( DEVOBJ *, CCB_SCSIIO *, int *, int * );

	void					*handle;		// handle (ie io-blk handle from mount)

	int						b_shift;
	int						version;		// Inquiry Version
	int						max_unmap_lba;
	int						max_unmap_desc_count;
	char					*prefix;
	uint64_t				hc_paddr_msk;	// host cntrl paddr msk (derived from CAM_VUHBA_EFLAG_DMA_XXX)

	uint32_t				max_io_rd;		// Maximum read blocks device can handle
	uint32_t				max_io_wr;		// Maximum write blocks device can handle
};

int		scsi_init( void );
int		scsi_dinit( void );
CCB		*scsi_ccb_alloc( void );
void	scsi_ccb_free( CCB *ccb );
int		scsi_scan_devs( int type, int (*func)( CCB_PATHINQ *, CCB_GETDEV *) );
int		scsi_scan_bus( int path );
int		scsi_setasync_cb( ASYNC_INFO **alist, CCB_SETASYNC *ccb );
void	*scsi_malloc( int size, int mflags );
void	scsi_free( void *addr, int size );
int		scsi_io( void *hdl, ioreq_t *ioreq );
int		scsi_iomsg( void *hdl, resmgr_context_t *ctp, int ioflag, ioreq_t *tag, io_msg_t *msg );
int		scsi_abort( void *hdl, ioreq_t *ioreq, unsigned flags );
int		scsi_devctl( void *hdl, resmgr_context_t *ctp, int ioflag, ioreq_t *tag, unsigned dcmd, void *data, size_t size, int *ret );
int		scsi_action( DEVOBJ *dobj, CCB_SCSIIO *ccb );
int		scsi_action_wait( DEVOBJ *dobj, CCB_SCSIIO *ccb, int retries );
int		scsi_release_simq( DEVOBJ *dobj );
int		scsi_read_capacity( DEVOBJ *dobj );
int 	scsi_log_sense( DEVOBJ *dobj, int page, char *ldata, int opt, int pc, int parm_ptr, int len );
int		scsi_log_select( DEVOBJ *dobj, char *pdata, int len, int pc, int opt );
int		scsi_mode_sense( DEVOBJ *dobj, int page, char *mdata, int len );
int		scsi_mode_select( DEVOBJ *dobj, char *pdata, int len, int save );
void	*scsi_mode_page( char *msdata, int page, int len, int type );
int		scsi_reserve( DEVOBJ *dobj );
int		scsi_release( DEVOBJ *dobj );
int		scsi_synchronize_cache( DEVOBJ *dobj, baddr_t *lba );
int		scsi_test_ready( DEVOBJ *dobj );
int		scsi_error( DEVOBJ *dobj, CCB_SCSIIO *ccb, int *status, int rcount );
int		scsi_interpret_sense( DEVOBJ *dobj, CCB_SCSIIO *ccb, int *status, int rcount );
int		scsi_inquiry( DEVOBJ *dobj, int opt, int pc, void *inq, int inq_len );
int		scsi_unit_ready( DEVOBJ *dobj );
int		scsi_removal( DEVOBJ *dobj, int prevent );
DEVOBJ	*scsi_create_device( CCB_PATHINQ *ccb_path, CCB_GETDEV *ccb_getdev, int ext_size, int max_io );
int		scsi_set_timeout( DEVOBJ *dobj, int g1, int g2, int g3 );
int		scsi_destroy_device( DEVOBJ *dobj );
int		scsi_negotiate( DEVOBJ *dobj );
int		scsi_sense_data( void *sdata, int *error, int *key, int *asc, int *ascq );
void	scsi_display_ccb( CCB *ccb_ptr, int verbosity );
void	scsi_action_wait_cbf( CCB_SCSIIO *ccb );
paddr64_t scsi_mphys( void *vaddr );
void	* scsi_virt( paddr64_t paddr );
void	*_scsi_malloc( int size, uint32_t protf, uint32_t mapf );
IO_VU_DATA	*scsi_ccb_vdata( CCB *vccb );

const char *typetostr( int type );

ssize_t __attribute__((__format__(__printf__, 5, 6))) cam_slogf( int opcode, int severity, int verbosity, int vlevel, const char *fmt, ... );
void	cam_strip_spaces( char *src, char *dst, int len );
void	cam_dump( char *buffer, int cnt );
int		cam_parse_size( const char *str );
int		cam_parse_number( const char *str );
int64_t	cam_parse_number64( const char *str );
int		cam_strtol( const char *str, char **cp );
int64_t	cam_strtoll( const char *str, char **cp );
int		cam_parse_tuple( char *string, char separator, char **entry, ... );
int		cam_device_opt( path_id_t path_id, target_id_t target_id, lun_id_t lun, char *value );
char	*cam_module_args( int argc, char *argv[], char *mname );
int		cam_blk_shift( unsigned blk_len );
int		cam_pdrvr_options( PDRVR_EXT *pext, DEVOBJ *dobj, char *options );
int		cam_set_thread_state( uint32_t *tstate, int state );
int		cam_create_thread( pthread_t *tid, pthread_attr_t *aattr, void *(*func)(void *), void *arg, int priority, uint32_t *tstate, char *name );

#ifdef CAM_MEM_DEBUG
extern void _cam_free( void *p, char *f, int l );
extern void *_cam_calloc( size_t n, size_t sz, char *f, int l );
extern void *_cam_realloc( void *p, size_t sz, char *f, int l );

#define cam_free( _p ) _cam_free( (_p), __FILE__, __LINE__ )
#define cam_calloc( _n, _sz ) _cam_calloc( (_n), (_sz), __FILE__, __LINE__ )
#define cam_realloc( _p, _sz ) _cam_realloc( (_p), (_sz), __FILE__, __LINE__ )
#else
#define cam_free		free
#define cam_calloc		calloc
#define cam_realloc		realloc

#endif

#endif

int cam_configure( const MODULE_ENTRY *sim_entry, int nsims, int argc, char *argv[] );

int xpt_configure( const MODULE_ENTRY *sim_entry, int nsims, int argc, char *argv[] );

int dll_pdrvr_init( cam_entry_t *cam_funcs, int argc, char *argv[] );

void *scsi_start_resmgr( DEVEXT *dext );
int scsi_destroy_resmgr( void *hdl );

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devb/include/ntocam.h $ $Rev: 915926 $")
#endif
