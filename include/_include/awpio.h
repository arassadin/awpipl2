
#ifndef _AWP_IO_H_
#define _AWP_IO_H_
/*utilites*/
AWPRESULT _awpGetFileExt(const char* lpFileName, char* lpFileExt);
/*awp image io*/
AWPRESULT _awpLoadAWPImage(const char* lpFileName, awpImage** ppImage);
AWPRESULT _awpSaveAWPImage(const char* lpFileName, awpImage* pImage);
/*bmp image io*/
AWPRESULT _awpLoadBMPImage(const char* lpFileName, awpImage** ppImage);
AWPRESULT _awpSaveBMPImage(const char* lpFileName, awpImage* pImage);

#ifdef HAVE_JPEG
/*jpeg image io*/
AWPRESULT _awpLoadJPEGImage(const char* lpFileName, awpImage** ppImage);
AWPRESULT _awpSaveJPEGImage(const char* lpFileName, awpImage* pImage);
#endif /* HAVE_JPEG */

#endif /*_AWP_IO_H_*/
