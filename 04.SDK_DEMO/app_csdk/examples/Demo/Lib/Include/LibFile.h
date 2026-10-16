//-----------------------------------------------------------------------------
// @file    : LibFile.h
// @brief   : File functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef __LIB_FILE_H__
#define __LIB_FILE_H__

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

// File system operation error code
enum lfs_error {
    LFS_ERR_OK       = 0,    // No error
    LFS_ERR_IO       = -5,   // Error during device operation
    LFS_ERR_CORRUPT  = -52,  // Corrupted
    LFS_ERR_NOENT    = -2,   // No directory entry
    LFS_ERR_EXIST    = -17,  // Entry already exists
    LFS_ERR_NOTDIR   = -20,  // Entry is not a dir
    LFS_ERR_ISDIR    = -21,  // Entry is a dir
    LFS_ERR_NOTEMPTY = -39,  // Dir is not empty
    LFS_ERR_BADF     = -9,   // Bad file number
    LFS_ERR_FBIG     = -27,  // File too large
    LFS_ERR_INVAL    = -22,  // Invalid parameter
    LFS_ERR_NOSPC    = -28,  // No space left on device
    LFS_ERR_NOMEM    = -12,  // No more memory available
};

enum lfs_open_flags {
    // open flags
    LFS_O_RDONLY = 1,        // Open a file as read only
    LFS_O_WRONLY = 2,        // Open a file as write only
    LFS_O_RDWR   = 3,        // Open a file as read and write
    LFS_O_CREAT  = 0x0100,   // Create a file if it does not exist
    LFS_O_EXCL   = 0x0200,   // Fail if a file already exists
    LFS_O_TRUNC  = 0x0400,   // Truncate the existing file to zero size
    LFS_O_APPEND = 0x0800,   // Move to end of file on every write

    // internally used flags
    LFS_F_DIRTY   = 0x10000, // File does not match storage
    LFS_F_WRITING = 0x20000, // File has been written since last flush
    LFS_F_READING = 0x40000, // File has been read since last flush
    LFS_F_ERRED   = 0x80000, // An error occured during write
};

// File seek flags
typedef enum
{
    LFS_SEEK_SET = 0,   // Seek relative to an absolute position
    LFS_SEEK_CUR = 1,   // Seek relative to the current file position
    LFS_SEEK_END = 2,   // Seek relative to the end of the file
}LFS_SEEK_FLAG;

//-----------------------------------------------------------------------------
// Name    : Lib_FileOpen
// Func    : Open File
// Paras   : pscFileName[in],    input the file name
//           iMode[in],         input the mode such as LFS_O_RDONLY or LFS_O_WRONLY and so on.
// Ret     : File handle,More then zero is successful, less than 0 is failure
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileOpen(char *pscFileName, int iMode);

//-----------------------------------------------------------------------------
// Name    : Lib_FileRead
// Func    : Read content from file
// Paras   : slFileHandle[in],   input the file handle
//           pucData[in/out],    input the data buffer and output the content
//           ulLen[in],          input length 
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileRead(int slFileHandle, unsigned char *pucData, unsigned long ulLen);

//-----------------------------------------------------------------------------
// Name    : Lib_FileWrite
// Func    : write data to file
// Paras   : slFileHandle[in],   input the file handle
//           pucData[in],        input the data buffer
//           ulLen[in],          input length of data
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileWrite(int slFileHandle, unsigned char *pucData, unsigned long ulLen);

//-----------------------------------------------------------------------------
// Name    : Lib_FileClose
// Func    : Close file
// Paras   : slFileHandle[in],   input the file handle
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileClose(int slFileHandle);

//-----------------------------------------------------------------------------
// Name    : Lib_FileSeek
// Func    : Move the file pointer to the specified location
// Paras   : slFileHandle[in],   input the file handle
//           slOffset[in],       input the number of bytes moved
//           fromWhere[in],    input starting position,please refer to LFS_SEEK_FLAG
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileSeek(int slFileHandle, int slOffset, LFS_SEEK_FLAG fromWhere);

//-----------------------------------------------------------------------------
// Name    : Lib_FileSize
// Func    : Get the file size
// Paras   : pscFileName[in],   input the file name
// Ret     : return the file size
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileSize(char *pscFileName);

//-----------------------------------------------------------------------------
// Name    : Lib_FileRemove
// Func    : remove the file
// Paras   : pscFileName[in],   input the file name
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileRemove(const char *pscFileName);

//-----------------------------------------------------------------------------
// Name    : Lib_FileFreeSize
// Func    : Get the remaining space of the file system
// Paras   : 
// Ret     : return the remaining space size
// Desc    :
//-----------------------------------------------------------------------------
long Lib_FileFreeSize(void);

//-----------------------------------------------------------------------------
// Name    : Lib_FileMkdir
// Func    : Create file directory
// Paras   : pscName[in],    input the file directory name
//           ucMode[in],     input mode,you can transmit 0.
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileMkdir(const char *pscName, unsigned char ucMode);

//-----------------------------------------------------------------------------
// Name    : Lib_FileOpendir
// Func    : Open file directory
// Paras   : pscName[in],    input the file directory name
// Ret     : return file directory handle
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileOpendir(const char *pscPath);

//-----------------------------------------------------------------------------
// Name    : Lib_FileReaddir
// Func    : Read filenames in the file directory
// Paras   : fd[in],           input file directory handle
//           namebuf[in/out],  output filenames in the directory
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileReaddir(int fd, char *namebuf);

//-----------------------------------------------------------------------------
// Name    : Lib_FileClosedir
// Func    : Close file directory
// Paras   : fd[in],   input file directory handle
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileClosedir(int fd);

//-----------------------------------------------------------------------------
// Name    : Lib_FileGetAllFiles
// Func    : Get all filenames in the file directory
// Paras   : pscPath[in],    input the file directory name
//           buf[in/out],    input data buffer and output all filenames
//           size[in],       input the maximum length of data buffer
// Ret     : return the total length of all filenames, less than zero is failure.
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileGetAllFiles(const char *pscPath, char *buf, int size);

//-----------------------------------------------------------------------------
// Name    : Lib_FileClearPathAllFiles
// Func    : Remove all file in the file directory
// Paras   : pscPath[in],    input the file directory name
// Ret     : 0 - success, others - fail
// Desc    :
//-----------------------------------------------------------------------------
int Lib_FileClearPathAllFiles(const char *pscPath);

#endif