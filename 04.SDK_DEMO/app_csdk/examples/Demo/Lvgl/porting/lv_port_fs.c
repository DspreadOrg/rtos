/**
 * @file lv_port_fs_templ.c
 *
 */

/*Copy this file as "lv_port_fs.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_fs.h"
#include "../lvgl.h"
#include "LibApi.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence);
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);

static void * fs_dir_open(lv_fs_drv_t * drv, const char * path);
static lv_fs_res_t fs_dir_read(lv_fs_drv_t * drv, void * rddir_p, char * fn);
static lv_fs_res_t fs_dir_close(lv_fs_drv_t * drv, void * rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

typedef struct png_file {
    char name[64];
    int size;
    unsigned char *data;
    int off;
    struct png_file *pNext;
} png_file;

png_file *png_list;
png_file *png_cur;

void lv_port_fs_init(void)
{
    /*----------------------------------------------------
     * Initialize your storage device and File System
     * -------------------------------------------------*/
    fs_init();

    /*---------------------------------------------------
     * Register the file system interface in LVGL
     *--------------------------------------------------*/

    /*Add a simple drive to open images*/
    static lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    /*Set up fields...*/
    fs_drv.letter = 'P';
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;

    fs_drv.dir_close_cb = fs_dir_close;
    fs_drv.dir_open_cb = fs_dir_open;
    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your Storage device and File system.*/
static void fs_init(void)
{
    /*E.g. for FatFS initialize the SD card and FatFS itself*/
	LV_LOG_INFO("tyfs_init");
    /*You code here*/
}

/**
 * Open a file
 * @param drv       pointer to a driver where this function belongs
 * @param path      path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode      read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return          a file descriptor or NULL on error
 */
static void * fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode)
{
    png_file **png_next;
    int fd;
    int file_size = 0;

    static unsigned int malloc_len = 0;

    png_cur = png_list;

    LV_LOG_INFO("tyfs_open %s", path);

    if (png_list != NULL)
    {
        do
        {
            if (strcmp(path, png_cur->name) == 0)
            {
                if (png_cur->size == 0 && png_cur->data == NULL)
                {
                    return NULL;
                }
                png_cur->off = 0;
                return (void *)png_cur;
            }
        } while (png_cur->pNext != NULL && (png_cur = png_cur->pNext));

        png_cur->pNext = Lib_malloc(sizeof(png_file));
        if (NULL == png_cur->pNext)
        {
            LV_LOG_ERROR("tyfs_open malloc error!");
            return NULL;
        }
        malloc_len += sizeof(png_file);
        LV_LOG_INFO("tyfs_open malloc_len: %d", malloc_len);
        memset(png_cur->pNext, 0x00, sizeof(png_file));

        png_next = &png_cur->pNext;
    }
    else
    {
        png_list = (png_file *)Lib_malloc(sizeof(png_file));
        if (NULL == png_list)
        {
        	LV_LOG_ERROR("tyfs_open malloc error!");
            return NULL;
        }
        malloc_len += sizeof(png_file);
        LV_LOG_INFO("tyfs_open malloc_len: %d", malloc_len);
        memset(png_list, 0x00, sizeof(png_file));

        png_next = &png_list;
    }

    sprintf((*png_next)->name, "%s", path);

    file_size = Lib_FileSize((char *)path);
    if (file_size <= 0)
    {
        LV_LOG_ERROR("tyfs_open LV_FS_RES_NOT_EX");
        (*png_next)->size = 0;
        (*png_next)->data = NULL;
        (*png_next)->off = 0;
        return NULL;
    }

    (*png_next)->size = file_size;
    (*png_next)->data = Lib_malloc((*png_next)->size);
	if (NULL == (*png_next)->data)
    {
        memset(*png_next, 0, sizeof(png_file));
        Lib_free(*png_next);
        *png_next = NULL;

        LV_LOG_ERROR("tyfs_open malloc error!");
        return NULL;
    }
    malloc_len += (*png_next)->size;
    LV_LOG_INFO("tyfs_open malloc_len: %d", malloc_len);
    fd = Lib_FileOpen((char *)path, LFS_O_RDONLY);
    Lib_FileRead(fd, (*png_next)->data, (*png_next)->size);
    Lib_FileClose(fd);

    LV_LOG_INFO("tyfs_open name: %s, file_p: %p", (*png_next)->name, *png_next);
    (*png_next)->off = 0;

    return (void *)(*png_next);
}

/**
 * Close an opened file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable. (opened with fs_open)
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_close(lv_fs_drv_t * drv, void * file_p)
{
    png_file *png = (png_file *)file_p;
    LV_LOG_INFO("tyfs_close, file_p: %p", file_p);
    if (png == NULL)
    {
        LV_LOG_ERROR("tyfs_read file is NULL!");
        return LV_FS_RES_NOT_EX;
    }

    png->off = 0;

    return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable.
 * @param buf       pointer to a memory block where to store the read data
 * @param btr       number of Bytes To Read
 * @param br        the real number of read bytes (Byte Read)
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    LV_LOG_INFO("tyfs_read file_p: %p, btr: %d", file_p, btr);

    png_file *png = (png_file *)file_p;

    if (png == NULL)
    {
        LV_LOG_ERROR("tyfs_read file is NULL!");
        return LV_FS_RES_NOT_EX;
    }

    memcpy(buf, png->data + png->off, btr);

    *br = btr;
    png->off += btr;

    LV_LOG_INFO("tyfs_read actual read len: %d", *br);

    return LV_FS_RES_OK;
}

/**
 * Write into a file
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable
 * @param buf       pointer to a buffer with the bytes to write
 * @param btw       Bytes To Write
 * @param bw        the number of real written bytes (Bytes Written). NULL if unused.
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/
    LV_LOG_INFO("tyfs_write!");

    return res;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable. (opened with fs_open )
 * @param pos       the new position of read write pointer
 * @param whence    tells from where to interpret the `pos`. See @lv_fs_whence_t
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence)
{
    LV_LOG_INFO("tyfs_seek %d %d", whence, pos);

    png_file *png = (png_file *)file_p;

    if (png == NULL)
    {
        return LV_FS_RES_NOT_EX;
    }

    switch (whence)
    {
    case LV_FS_SEEK_SET:
        png->off = pos;
        break;
    case LV_FS_SEEK_CUR:
        png->off += pos;
        break;
    case LV_FS_SEEK_END:
        png->off = png->size + pos;
        break;
    }

    return LV_FS_RES_OK;
}
/**
 * Give the position of the read write pointer
 * @param drv       pointer to a driver where this function belongs
 * @param file_p    pointer to a file_t variable.
 * @param pos_p     pointer to to store the result
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    lv_fs_res_t res = LV_FS_RES_OK;

    png_file *png = (png_file *)file_p;

    if (png == NULL)
    {
        LV_LOG_ERROR("tyfs_read file is NULL!");
        return LV_FS_RES_NOT_EX;
    }

    /* Add your code here*/
    *pos_p = png->off;
    LV_LOG_INFO("tyfs_tell pos: %d", *pos_p);

    return res;
}

/**
 * Initialize a 'lv_fs_dir_t' variable for directory reading
 * @param drv       pointer to a driver where this function belongs
 * @param path      path to a directory
 * @return          pointer to the directory read descriptor or NULL on error
 */
static void * fs_dir_open(lv_fs_drv_t * drv, const char * path)
{
    void * dir = NULL;
    /*Add your code here*/
//    dir = ...           /*Add your code here*/
          return dir;
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv       pointer to a driver where this function belongs
 * @param rddir_p   pointer to an initialized 'lv_fs_dir_t' variable
 * @param fn        pointer to a buffer to store the filename
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read(lv_fs_drv_t * drv, void * rddir_p, char * fn)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}

/**
 * Close the directory reading
 * @param drv       pointer to a driver where this function belongs
 * @param rddir_p   pointer to an initialized 'lv_fs_dir_t' variable
 * @return          LV_FS_RES_OK: no error or  any error from @lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close(lv_fs_drv_t * drv, void * rddir_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /*Add your code here*/

    return res;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
