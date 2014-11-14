
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "_awpipl.h"
#include <limits.h>

/*
	преобразование исходного изображения pSrc в изображение pDst
	
*/
AWPRESULT awpConvert2(awpImage* pSrc, awpImage* pDst, long lOptions)
{
	AWPRESULT res = AWP_OK;
	_ERROR_EXIT_RES_(res)
CLEANUP:
	return res;
}

/*
	преобразование исходного изображения в изображение другого типа.
	в случае совпадения типов выполняется копирование изображения. 
	Функция создает преобразованное изображение с использованием awpCreateImage
*/
AWPRESULT awpConvert1(awpImage* pImage, awpImage** ppImage, long lOptions)
{
	AWPRESULT res = AWP_OK;
	_ERROR_EXIT_RES_(res)
CLEANUP:
	return res;
}

AWPRESULT _awpConvertByteToAWPAWPDOUBLE(awpImage* Image)
{
    AWPRESULT res;
    AWPDOUBLE* pD;
    AWPINT i;
    AWPBYTE* pB;
    long lSize;

    res = AWP_OK;/*initialize result */

    lSize=Image->sSizeX*Image->sSizeY*Image->bChannels;

    /* allocate memory for AWPDOUBLE data */
    pD = (AWPDOUBLE*)malloc(sizeof(AWPDOUBLE)*lSize);

    if(pD==NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pB=(AWPBYTE*)Image->pPixels;


    if(res!=AWP_OK){
        free((void*)pD);
        _ERR_EXIT_
    }

    for(i=0;i<lSize;i++){
        pD[i]=pB[i];
    }

    free((void*)pB);
    Image->dwType=AWP_DOUBLE;
    Image->pPixels=(void*)pD;

CLEANUP:

    return res;
}

AWPRESULT _awpConvertByteToFloat(awpImage* Image)
{
    AWPRESULT res;
    AWPFLOAT* pD;
    AWPINT i;
    AWPBYTE* pB;
    long lSize;

    res = AWP_OK;/*initialize result */

    lSize=Image->sSizeX*Image->sSizeY*Image->bChannels;

    /* allocate memory for AWPFLOAT data */
    pD = (AWPFLOAT*)malloc(sizeof(AWPFLOAT)*lSize);

    if(pD==NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pB=(AWPBYTE*)Image->pPixels;


    if(res!=AWP_OK){
        free((void*)pD);
        _ERR_EXIT_
    }

    for(i=0;i<lSize;i++){
        pD[i]=pB[i];
    }

    free((void*)pB);
    Image->dwType=AWP_FLOAT;
    Image->pPixels=(void*)pD;

CLEANUP:

    return res;
}

AWPRESULT _awpConvertByte3To1(awpImage* Image)
{
    AWPRESULT res;
    long lSize;
    AWPBYTE* pB=NULL;
    AWPBYTE* pBSrc=NULL;
    AWPINT i,ii;

    res = AWP_OK;/*initialize result */
    
    if(Image->dwType!=AWP_BYTE || Image->bChannels!=3){
	res = AWP_NOTSUPPORT;
        _ERR_EXIT_
    }

    lSize=Image->sSizeX*Image->sSizeY;

    pBSrc=(AWPBYTE*)Image->pPixels;

    /* allocate memory for AWPDOUBLE data */
    pB = (AWPBYTE*)malloc(sizeof(AWPBYTE)*lSize);
    if (pB == NULL)
    {
	res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    for(i=0;i<lSize;i++){
	ii=3*i;
	pB[i]=(pBSrc[ii]+pBSrc[ii+1]+pBSrc[ii+2])/3;
    }
    
    free(pBSrc);
    
    Image->bChannels=1;
    Image->pPixels=pB;
    
CLEANUP:

    return res;
    
}

AWPRESULT _awpConvertAWPAWPDOUBLEToByte(awpImage* Image)
{
    AWPRESULT res;
    AWPDOUBLE* pD;
    AWPINT i;
    AWPBYTE* pB;
    long lSize;

    res = AWP_OK;/*initialize result */

    lSize=Image->sSizeX*Image->sSizeY*Image->bChannels;

    /* allocate memory for byte data */
    pB = (AWPBYTE*)malloc(sizeof(AWPBYTE)*lSize);

    if(pB==NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pD=(AWPDOUBLE*)Image->pPixels;

    for(i=0;i<lSize;i++){
        pB[i]=(AWPBYTE)(pD[i]<0)?0:(pD[i]>255?255:(AWPBYTE)pD[i]);
    }

    free((void*)pD);
    Image->dwType=AWP_BYTE;
    Image->pPixels=(void*)pB;

CLEANUP:

    return res;
}
AWPRESULT _awpConvertFloatToByte(awpImage* Image)
{
    AWPRESULT res;
    AWPFLOAT* pD;
    AWPINT i;
    AWPBYTE* pB;
    long lSize;

    res = AWP_OK;/*initialize result */

    lSize=Image->sSizeX*Image->sSizeY*Image->bChannels;

    /* allocate memory for byte data */
    pB = (AWPBYTE*)malloc(sizeof(AWPBYTE)*lSize);

    if(pB==NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pD=(AWPFLOAT*)Image->pPixels;

    for(i=0;i<lSize;i++){
        pB[i]=(AWPBYTE)(pD[i]<0)?0:(pD[i]>255?255:(AWPBYTE)pD[i]);
    }

    free((void*)pD);
    Image->dwType=AWP_BYTE;
    Image->pPixels=(void*)pB;

CLEANUP:

    return res;
}


AWPRESULT _awpConvertAWPAWPDOUBLEToByteWithNorm(awpImage* Image)
{
    AWPRESULT res;
    AWPDOUBLE* pD;
    AWPINT i;
    AWPBYTE ch;
    AWPBYTE* pB;
    long lSize;
    AWPDOUBLE* pMax,*pMin;

    res = AWP_OK;/*initialize result */
    pMax=NULL;pMin=NULL;

    lSize=Image->sSizeX*Image->sSizeY*Image->bChannels;

    /* allocate memory for byte data */
    pB = (AWPBYTE*)malloc(sizeof(AWPBYTE)*lSize);

    if(pB == NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    /* allocate memory for min and max  data */
    pMax = (AWPDOUBLE*)malloc(sizeof(AWPDOUBLE)*Image->bChannels);
    if(pMax == NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pMin = (AWPDOUBLE*)malloc(sizeof(AWPDOUBLE)*Image->bChannels);
    if(pMin == NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    /* full max and min data default value */
    for(ch=0;ch<Image->bChannels;ch++){
        pMax[ch]=-1e30;
        pMin[ch]=1e30;
    }

    pD=(AWPDOUBLE*)Image->pPixels;

    /* found min and max in image data */
    for(i=0;i<lSize;i+=Image->bChannels){
        for(ch=0;ch<Image->bChannels;ch++){
            if(pMax[ch]<pD[i+ch]){
                pMax[ch]=pD[i+ch];
            }
            if(pMin[ch]>pD[i+ch]){
                pMin[ch]=pD[i+ch];
            }
        }
    }

    /* normalize data */
    for(i=0;i<lSize;i+=Image->bChannels){
        for(ch=0;ch<Image->bChannels;ch++){
            pB[i+ch]=(AWPBYTE)(255.0*(pD[i+ch]-pMin[ch])/(pMax[ch]-pMin[ch]+0.0001));
        }
    }

    free((void*)pD);
    Image->dwType=AWP_BYTE;
    Image->pPixels=(void*)pB;

CLEANUP:
    if(pMax!=NULL){
	free(pMax);
    }

    if(pMin!=NULL){
	free(pMin);
    }

    return res;
}


AWPRESULT _awpConvertFloatToByteWithNorm(awpImage* Image)
{
    AWPRESULT res;
    AWPFLOAT* pD;
    AWPINT i;
    AWPBYTE ch;
    AWPBYTE* pB;
    long lSize;
    AWPFLOAT* pMax,*pMin;

    res = AWP_OK;/*initialize result */
    pMax=NULL;pMin=NULL;

    lSize=Image->sSizeX*Image->sSizeY*Image->bChannels;

    /* allocate memory for byte data */
    pB = (AWPBYTE*)malloc(sizeof(AWPBYTE)*lSize);

    if(pB == NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    /* allocate memory for min and max  data */
    pMax = (AWPFLOAT*)malloc(sizeof(AWPFLOAT)*Image->bChannels);
    if(pMax == NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pMin = (AWPFLOAT*)malloc(sizeof(AWPFLOAT)*Image->bChannels);
    if(pMin == NULL){
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    /* full max and min data default value */
    for(ch=0;ch<Image->bChannels;ch++){
        pMax[ch]=-1e-30f;
        pMin[ch]=1e30f;
    }

    pD=(AWPFLOAT*)Image->pPixels;

    /* found min and max in image data */
    for(i=0;i<lSize;i+=Image->bChannels){
        for(ch=0;ch<Image->bChannels;ch++){
            if(pMax[ch]<pD[i+ch]){
                pMax[ch]=pD[i+ch];
            }
            if(pMin[ch]>pD[i+ch]){
                pMin[ch]=pD[i+ch];
            }
        }
    }

    /* normalize data */
    for(i=0;i<lSize;i+=Image->bChannels){
        for(ch=0;ch<Image->bChannels;ch++){
            pB[i+ch]=(AWPBYTE)(255.0*(pD[i+ch]-pMin[ch])/(pMax[ch]-pMin[ch]+0.0000000000001));
        }
    }

    free((void*)pD);
    Image->dwType=AWP_BYTE;
    Image->pPixels=(void*)pB;

CLEANUP:
    if(pMax!=NULL){
	free(pMax);
    }

    if(pMin!=NULL){
	free(pMin);
    }

    return res;
}

AWPRESULT awpConvert(awpImage* Image, long lOptions)
{
    AWPRESULT res;

    res = AWP_OK;/*initialize result */

    /* check image */
    _CHECK_RESULT_((res = awpCheckImage(Image)))

    /* select type image */
    switch(Image->dwType){
    case AWP_BYTE:
        switch(lOptions)
		{
        case AWP_CONVERT_TO_DOUBLE:
            _CHECK_RESULT_((res=_awpConvertByteToAWPAWPDOUBLE(Image)))
            break;
		case AWP_CONVERT_3TO1_BYTE:
			_CHECK_RESULT_((res=_awpConvertByte3To1(Image)))
			break;
        case AWP_CONVERT_TO_FLOAT:
            _CHECK_RESULT_((res=_awpConvertByteToFloat(Image)))
            break;
        default:
            res = AWP_NOTSUPPORT;
            _ERR_EXIT_
        }
        break;

    case AWP_DOUBLE:
        switch(lOptions)
		{
        case AWP_CONVERT_TO_BYTE:
            _CHECK_RESULT_((res=_awpConvertAWPAWPDOUBLEToByte(Image)))
            break;
        case AWP_CONVERT_TO_BYTE_WITH_NORM:
            _CHECK_RESULT_((res=_awpConvertAWPAWPDOUBLEToByteWithNorm(Image)))
            break;
        default:
            res = AWP_NOTSUPPORT;
            _ERR_EXIT_
        }
        break;
    case AWP_FLOAT:
        switch(lOptions)
        {
            case AWP_CONVERT_TO_BYTE:
                _CHECK_RESULT_((res=_awpConvertFloatToByte(Image)))
            break;
            case AWP_CONVERT_TO_BYTE_WITH_NORM:
                _CHECK_RESULT_((res=_awpConvertFloatToByteWithNorm(Image)))
            break;
        default:
            res = AWP_NOTSUPPORT;
            _ERR_EXIT_
        }
    break;
    default:
        res = AWP_NOTSUPPORT;
        _ERR_EXIT_
    }

CLEANUP:
    return res;
}


#define SHIFT 12
    
static const AWPINT div_table[] =
{
       0, 1044480,  522240,  348160,  261120,  208896,  174080,  149211,
  130560,  116053,  104448,   94953,   87040,   80345,   74606,   69632,
   65280,   61440,   58027,   54973,   52224,   49737,   47476,   45412,
   43520,   41779,   40172,   38684,   37303,   36017,   34816,   33693,
   32640,   31651,   30720,   29842,   29013,   28229,   27486,   26782,
   26112,   25475,   24869,   24290,   23738,   23211,   22706,   22223,
   21760,   21316,   20890,   20480,   20086,   19707,   19342,   18991,
   18651,   18324,   18008,   17703,   17408,   17123,   16846,   16579,
   16320,   16069,   15825,   15589,   15360,   15137,   14921,   14711,
   14507,   14308,   14115,   13926,   13743,   13565,   13391,   13221,
   13056,   12895,   12738,   12584,   12434,   12288,   12145,   12006,
   11869,   11736,   11605,   11478,   11353,   11231,   11111,   10995,
   10880,   10768,   10658,   10550,   10445,   10341,   10240,   10141,
   10043,    9947,    9854,    9761,    9671,    9582,    9495,    9410,
    9326,    9243,    9162,    9082,    9004,    8927,    8852,    8777,
    8704,    8632,    8561,    8492,    8423,    8356,    8290,    8224,
    8160,    8097,    8034,    7973,    7913,    7853,    7795,    7737,
    7680,    7624,    7569,    7514,    7461,    7408,    7355,    7304,
    7253,    7203,    7154,    7105,    7057,    7010,    6963,    6917,
    6872,    6827,    6782,    6739,    6695,    6653,    6611,    6569,
    6528,    6487,    6447,    6408,    6369,    6330,    6292,    6254,
    6217,    6180,    6144,    6108,    6073,    6037,    6003,    5968,
    5935,    5901,    5868,    5835,    5803,    5771,    5739,    5708,
    5677,    5646,    5615,    5585,    5556,    5526,    5497,    5468,
    5440,    5412,    5384,    5356,    5329,    5302,    5275,    5249,
    5222,    5196,    5171,    5145,    5120,    5095,    5070,    5046,
    5022,    4998,    4974,    4950,    4927,    4904,    4881,    4858,
    4836,    4813,    4791,    4769,    4748,    4726,    4705,    4684,
    4663,    4642,    4622,    4601,    4581,    4561,    4541,    4522,
    4502,    4483,    4464,    4445,    4426,    4407,    4389,    4370,
    4352,    4334,    4316,    4298,    4281,    4263,    4246,    4229,
    4212,    4195,    4178,    4161,    4145,    4128,    4112,    4096
};
    
#define mmax(a,b)    ((b) + (((a)-(b))&((a)>(b)?-1:0)))
#define mmin(a,b)    ((a) - (((a)-(b))&((a)>(b)?-1:0)))


// Added by Radzhabova Julia
// Converts RGB image to HSL image
AWPRESULT awpRGBtoHSLImage(const awpImage* Image,awpImage** ppHSL)
{

    const AWPINT planes = 3;
    AWPINT   x, y, x1;
    AWPINT b, g, r;
    AWPINT h, s, v;
    AWPINT vmin, diff;
    AWPINT vr, vg;
    AWPINT        min_ch_val[6];
    AWPINT        max_ch_val[6];

	AWPBYTE* src = (AWPBYTE*)Image->pPixels;
	AWPINT    src_pitch = Image->sSizeX;
	AWPBYTE* dst = NULL;
    awpImage* im = NULL;
	AWPINT    dst_pitch = 0;


	AWPINT i			= 0;
	AWPRESULT res	= AWP_OK;

	res = awpCheckImage(Image);
	if (res != AWP_OK)
        return res;

	if (Image->dwType != AWP_BYTE && Image->bChannels != 3)
		return res;

	res = awpCreateImage(&im, Image->sSizeX,
		Image->sSizeY, 3, AWP_BYTE);
	if (res != AWP_OK)
        return res;

	dst = (AWPBYTE*)im->pPixels;
	dst_pitch = im->sSizeX;

	for( i = 0; i < 6; i++ )
    {
        min_ch_val[i] = 0;
        max_ch_val[i] = 255;
    }


	for( y = 0; y < Image->sSizeY; y++, src += src_pitch, dst += dst_pitch )
		for( x = 0, x1 = 0; x < Image->sSizeX * planes; x+=planes, x1++ )
        {
            
            b = src[x];
            g = src[x+1];
            r = src[x+2];
            
            // convert rgb->hsv
            {
                
                v    = mmax(r,g);
                v    = mmax(v,b);
                vmin = mmin(r,g);
                vmin = mmin(vmin,b);
                
                diff = v - vmin;
                vr = v == r ? -1 : 0;
                vg = v == g ? -1 : 0;
                
                s  = diff*div_table[v]>>SHIFT;
                h  = (vr&(g-b)) + (~vr&((vg&(b-r+2*diff)) + ((~vg)&(r-g+4*diff))));
                h  = ((h*div_table[diff]*15+(1<<(SHIFT+6)))>>(7+SHIFT)) + (h < 0 ? 30*6 : 0);
            }
            
            if( s >= min_ch_val[1] && v >= min_ch_val[2] && v <= max_ch_val[2] )
                dst[x1] = (AWPBYTE)h;
            else
                dst[x1] = 0;
        }
/*
	pc = (awpColor*)Image->pPixels;
	pcHSL = (awpColor*)(*ppHSL)->pPixels;
	c = Image->sSizeX*Image->sSizeY;
	for (i = 0; i < c; i++)
	{
		_awpRGBtoHSL(pc[i].bBlue ,pc[i].bGreen ,pc[i].bRed ,&h, &s, &l );
		h *= 255.0; s *= 255.0; l *= 255.0;
		pcHSL[i].bRed = (AWPBYTE)h;
		pcHSL[i].bGreen = (AWPBYTE)s;
		pcHSL[i].bBlue = (AWPBYTE)l;
	}
*/
    *ppHSL = im;
    return AWP_OK;
}

// Convert HSL image to RGB image
AWPRESULT awpHSLtoRGBImage(const awpImage* Image,awpImage** ppRGB)
{
	AWPINT i			= 0;
	AWPINT c			= 0;
	awpColor* pc	= NULL;
	awpColor* pcRGB	= NULL;
	AWPRESULT res	= AWP_OK;
	AWPDOUBLE h,s,l;
	AWPBYTE r,g,b;

	res = awpCheckImage(Image);
	if (res != AWP_OK)
        return res;

	if (Image->dwType != AWP_BYTE && Image->bChannels != 3)
		return AWP_BADARG;

	res = awpCreateImage(ppRGB, Image->sSizeX,
		Image->sSizeY, 3, AWP_BYTE);
	if (res != AWP_OK)
        return res;
        
	pc = (awpColor*)Image->pPixels;
	pcRGB = (awpColor*)(*ppRGB)->pPixels;
	c = Image->sSizeX*Image->sSizeY;
	for (i = 0; i < c; i++)
	{
		h = pc[i].bRed / 255.0;
		s = pc[i].bGreen / 255.0;
		l = pc[i].bBlue / 255.0;
		_awpHLStoRGB( h, s, l, &r, &g, &b);
		pcRGB[i].bRed = b;
		pcRGB[i].bGreen = g;
		pcRGB[i].bBlue = r;
	}

	return AWP_OK;
}

// converts Image of type AWP_GRAYSCALE to ppProb of AWP_GRAYSCALE
// Histogramm - normal awpHistogramm
AWPRESULT awpBackProjection(awpImage* Image, awpImage** ppProb, awpHistogramm* Histogramm)
{
	AWPINT i		= 0;
	AWPINT count	= 0;
	AWPBYTE* pProb		= NULL;
	AWPDOUBLE db = 0.0;
	AWPDOUBLE min_sat = 24;
	AWPDOUBLE min_val = 111;
	AWPDOUBLE max_val = 246;
	AWPDOUBLE max_sat = 246;
	/*check image*/
	AWPRESULT res = AWP_OK;
	res = awpCheckImage(Image);
	if (res != AWP_OK)
        return res;

	count = Image->sSizeX*Image->sSizeY;
	res = awpCreateImage(ppProb, Image->sSizeX,	Image->sSizeY, 1,
						AWP_BYTE);
    if (res != AWP_OK)
        return res;

	pProb = (AWPBYTE*)(*ppProb)->pPixels;
	if (Image->dwType == AWP_BYTE && Image->bChannels == 1)
	{
		AWPBYTE* p = (AWPBYTE*)Image->pPixels;

		for (i = 0; i < count; i++)
		{
			//db = (AWPDOUBLE)p[i]*(AWPDOUBLE)Histogramm->Intensity[p[i]];
			db = (AWPDOUBLE)Histogramm->Intensity[p[i]]*255.0;
			pProb[i] = (AWPBYTE)(db+0.5);
		}

	}
	if (Image->dwType == AWP_BYTE && Image->bChannels == 3)
	{
		awpColor* p = (awpColor*)Image->pPixels;

		for (i = 0; i < count; i++)
		{

			//db = (AWPDOUBLE)p[i]*(AWPDOUBLE)Histogramm->Intensity[p[i]];
			if ((p[i].bGreen >= min_sat ) && (p[i].bBlue >= min_val) && (p[i].bBlue <= max_val)&&(p[i].bGreen <= max_sat)&& (p[i].bRed!=0))
				db = (AWPDOUBLE)Histogramm->Intensity[p[i].bRed]*255.0;
			else
				db = 0.0;
			pProb[i] = (AWPBYTE)(db + 0.5);
		}

	}

    return AWP_OK;
}

// converts Image of type AWP_BYTE with HSV color space to ppProb of AWP_GRAYSCALE
// Histogramm - normal awpHistogramm
AWPRESULT awpBackProjection2D(awpImage* Image, awpImage** ppProb, awpImage* pPreset, AWPBYTE min_v, AWPBYTE max_v)
{
    AWPRESULT res;
    /*pointers to the pixels*/
    AWPDOUBLE*         dblPresetPix;
    awpColor*       cPix;
    AWPBYTE*           bPix;
    AWPBYTE            h,s,v;
    AWPINT i;
    AWPDOUBLE          max_preset;
    res = AWP_OK;/*initialize result */

    if (ppProb == NULL)
    {
        res = AWP_BADARG;
        _ERR_EXIT_
    }

    _CHECK_RESULT_((res = awpCheckImage(Image)))
    _CHECK_RESULT_((res = awpCheckImage(pPreset)))

    /*изображение Image должно иметь 3 байта на точку
    для получения изображения ppProb используются первые
    два канала Image*/
    if (Image->dwType != AWP_BYTE && Image->bChannels != 3)
    {
        res = AWP_BADARG;
        _ERR_EXIT_
    }

    /*изображение pPreset должно иметь тип AWP_DOUBLE, содержать
    один канал данных и быть нормированным на единицу, размеры
    изображения pPreset должны быть 256 х 256*/
    if (pPreset->dwType != AWP_DOUBLE || pPreset->bChannels != 1
    && pPreset->sSizeX != 256 || pPreset->sSizeY != 256)
    {
        res = AWP_BADARG;
        _ERR_EXIT_
    }

    /*получим указатели на пиксели изображения pPreset*/
    dblPresetPix = (AWPDOUBLE*)pPreset->pPixels;
    max_preset = dblPresetPix[0];
    for (i = 1; i < pPreset->sSizeX*pPreset->sSizeY; i++)
        if (dblPresetPix[i] > max_preset)
            max_preset = dblPresetPix[i];
//    if (max_preset != 255)
//    {
//        /*Гистограмма должна быть отнормированной на 1*/
//        res = AWP_BADARG;
//        _ERR_EXIT_
//
//    }
    /*создадим изображение для BackProjection*/
    _CHECK_RESULT_((res = awpCreateImage(ppProb, Image->sSizeX, Image->sSizeY, 1, AWP_BYTE)))

    cPix = (awpColor*)Image->pPixels;
    bPix = (AWPBYTE*)(*ppProb)->pPixels;
    /*Получим обратную проекцию*/
    for (i = 0; i < Image->sSizeX*Image->sSizeY;i++)
    {
       h = cPix[i].bRed;
       s = cPix[i].bGreen;
       v = cPix[i].bBlue;
       if (v >= min_v && v <= max_v)
           bPix[i] = (AWPBYTE)(dblPresetPix[s*256+h]);
       else
           bPix[i] = 0;
    }
CLEANUP:
    return res;
}

AWPRESULT awpRGBtoHSVImage(const awpImage* Image,awpImage** ppHSV)
{
	AWPINT i			= 0;
	AWPINT c			= 0;
	awpColor* pc	= NULL;
	awpColor* pcHSV	= NULL;
	AWPRESULT res	= AWP_OK;
	AWPFLOAT h,s,v;

	res = awpCheckImage(Image);
	if (res != AWP_OK)
        return res;
	if (Image->dwType != AWP_BYTE || Image->bChannels != 3)
		return AWP_BADARG;

	res = awpCreateImage(ppHSV, Image->sSizeX,
		Image->sSizeY, 3, AWP_BYTE);
	if (res != AWP_OK)
        return res;

	pc = (awpColor*)Image->pPixels;
	pcHSV = (awpColor*)(*ppHSV)->pPixels;
	c = Image->sSizeX*Image->sSizeY;
	for (i = 0; i < c; i++)
	{
		_awpRGBtoHSV(pc[i].bBlue ,pc[i].bGreen ,pc[i].bRed ,&h, &s, &v );
		if (h == -1)
        {
       		pcHSV[i].bRed = 0;
	    	pcHSV[i].bGreen = 0;
		    pcHSV[i].bBlue = 0;

            continue;
        }
		h *= 255.0; s *= 255.0; v *= 255.0;
		pcHSV[i].bRed = (AWPBYTE)h;
		pcHSV[i].bGreen = (AWPBYTE)s;
		pcHSV[i].bBlue = (AWPBYTE)v;
	}
	return AWP_OK;
}

// Convert HSV image to RGB image
AWPRESULT awpHSVtoRGBImage(const awpImage* Image,awpImage** ppRGB)
{
	AWPINT i			= 0;
	AWPINT c			= 0;
	awpColor* pc	= NULL;
	awpColor* pcRGB	= NULL;
	AWPRESULT res	= AWP_OK;
	AWPDOUBLE h,s,v;
	AWPBYTE r,g,b;

	res = awpCheckImage(Image);
	if (res != AWP_OK)
        return res;

	if (Image->dwType != AWP_BYTE && Image->bChannels != 3)
		return AWP_BADARG;

	res = awpCreateImage(ppRGB, Image->sSizeX,
		Image->sSizeY, 3, AWP_BYTE);
	if (res != AWP_OK)
        return res;

	pc = (awpColor*)Image->pPixels;
	pcRGB = (awpColor*)(*ppRGB)->pPixels;
	c = Image->sSizeX*Image->sSizeY;
	for (i = 0; i < c; i++)
	{
		h = pc[i].bRed / 255.0;
		s = pc[i].bGreen / 255.0;
		v = pc[i].bBlue / 255.0;
		_awpHSVtoRGB( h, s, v, &r, &g, &b);
		pcRGB[i].bRed = b;
		pcRGB[i].bGreen = g;
		pcRGB[i].bBlue = r;
	}

	return AWP_OK;
}

#ifdef WIN32
/*

*/
AWPRESULT awpImageToDIB( const awpImage* pImage, BITMAPINFO* pBMI, void** pBMData, AWPBOOL replace )
{
	AWPBYTE *dib_pixels  = NULL;
	AWPBYTE *awp_row     = NULL;
	AWPBYTE *dib_row     = NULL;
	AWPBYTE *tmp_row, *top_row, *bottom_row;
	AWPINT row, col, dib_width, awp_width;
    AWPINT ind_awp, ind_bm;

	if (pBMI == NULL)
		return AWP_BADARG;

	if (awpCheckImage(pImage) != AWP_OK)
		return AWP_BADARG;

	if (pImage->dwType != AWP_BYTE)
		return AWP_BADARG;

	if (pImage->bChannels != 3 && pImage->bChannels != 1)
		return AWP_BADARG;

	pBMI->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	pBMI->bmiHeader.biWidth         = pImage->sSizeX;
	pBMI->bmiHeader.biHeight        = pImage->sSizeY;
	pBMI->bmiHeader.biPlanes        = 1;
    pBMI->bmiHeader.biBitCount      = 24;
	pBMI->bmiHeader.biCompression   = BI_RGB;
	pBMI->bmiHeader.biSizeImage     = 0;
	pBMI->bmiHeader.biXPelsPerMeter = 0;
	pBMI->bmiHeader.biYPelsPerMeter = 0;
	pBMI->bmiHeader.biClrUsed       = 0;
	pBMI->bmiHeader.biClrImportant  = 0;

	awp_width  = 3*pImage->sSizeX;
    dib_width  = ((pImage->sSizeX*24 + 31)/32) * 4; /* aligned to 4 bytes */
    if (replace)
        dib_pixels = (AWPBYTE*)*pBMData;
    else
        dib_pixels = (AWPBYTE *)malloc(dib_width * pImage->sSizeY * sizeof(AWPBYTE));

    if (dib_pixels == NULL)
      	return AWP_NOTENOUGH_MEM;
    memset(dib_pixels, 0, dib_width*pImage->sSizeY*sizeof(AWPBYTE));

    switch (pImage->bChannels)
    {
    case 1: /* Grayscale */
  	    for (row=0; row<pImage->sSizeY; row++)
		    for (col=0; col<pImage->sSizeX; col++)
		    {
			    ind_bm  = row*dib_width + 3*col;
				ind_awp = (pImage->sSizeY - 1 - row)*pImage->sSizeX + col;

     			dib_pixels[ind_bm]     = ((AWPBYTE *)pImage->pPixels)[ind_awp];
	    		dib_pixels[ind_bm + 1] = dib_pixels[ind_bm];
		    	dib_pixels[ind_bm + 2] = dib_pixels[ind_bm];
			}
        break;
    case 3: /* Color */
        /* AWP width aligned to 4 bytes - simple copy pixels */
    	if (pImage->sSizeX % 4 == 0)
		    memcpy(dib_pixels, pImage->pPixels, awp_width*pImage->sSizeY*sizeof(AWPBYTE));
    	else
	   	    for (row=0; row<pImage->sSizeY; row++)
			{
				dib_row = dib_pixels + row*dib_width;
				awp_row = (AWPBYTE *)pImage->pPixels + row*awp_width;
				memcpy(dib_row, awp_row, awp_width*sizeof(AWPBYTE));
			}

        /* Flip DIB image around horizontal axis */
	    tmp_row = (AWPBYTE*)malloc(dib_width*sizeof(AWPBYTE));
    	if (tmp_row == NULL)
	    {
    		free(dib_pixels);
	    	return AWP_NOTENOUGH_MEM;
    	}

    	bottom_row = dib_pixels;
	    top_row    = bottom_row + (pImage->sSizeY - 1)*dib_width;
    	for (row=0; row<pImage->sSizeY/2; row++)
	    {
		    memcpy(tmp_row, top_row, dib_width*sizeof(AWPBYTE));
    		memcpy(top_row, bottom_row, dib_width*sizeof(AWPBYTE));
	    	memcpy(bottom_row, tmp_row, dib_width*sizeof(AWPBYTE));

		    bottom_row += dib_width*sizeof(AWPBYTE);
    		top_row    -= dib_width*sizeof(AWPBYTE);
	    }
        free(tmp_row);
        break;
	}
    if (!replace)
    	*pBMData = dib_pixels;

	return AWP_OK;
}
/*

*/
AWPRESULT awpDIBToImage( const BITMAPINFO* pBMI, const void* pBMData, awpImage** ppImage )
{
	awpImage *pImage  = NULL;
	AWPRESULT res     = AWP_OK;
//	awpColor *pix     = NULL;
	AWPBYTE *pBytes      = (AWPBYTE *)pBMData;
	AWPBYTE *dib_row, *awp_row;
	WORD awp_width, awp_height, dib_width;
	WORD row;


	if (pBMI == NULL)
		return AWP_BADARG;

	if (pBMI->bmiHeader.biSize != sizeof(BITMAPINFOHEADER))
		return AWP_BADARG;

	if (pBMI->bmiHeader.biBitCount != 24)
		return AWP_BADARG;

	awp_width  = (WORD)pBMI->bmiHeader.biWidth;
	awp_height = (WORD)pBMI->bmiHeader.biHeight;
	dib_width  = ((awp_width*24 + 31)/32 ) * 4;

	res = awpCreateImage(&pImage, awp_width, awp_height, 3, AWP_BYTE);
	if (res != AWP_OK)
		return res;

	if (awp_width % 4 == 0)
	{
		memcpy(pImage->pPixels, pBytes, 3*pImage->sSizeX*pImage->sSizeY*sizeof(AWPBYTE));
	}
	else
		for (row=0; row<pImage->sSizeY; row++)
		{
			dib_row = pBytes + row*dib_width;
			awp_row = (AWPBYTE *)pImage->pPixels + 3*row*awp_width;
			memcpy(awp_row, dib_row, 3*awp_width*sizeof(AWPBYTE));
		}
	awpFlip(&pImage, AWP_FLIP_VERT);

	*ppImage = pImage;

	return AWP_OK;
}
#endif //WIN32

AWPRESULT _awpConvertByteToShort(awpImage* Image)
{
    AWPRESULT res;
    SHORT* pS;
    AWPINT i;
    AWPBYTE* pB;
    long lSize;

    res = AWP_OK;/*initialize result */

    lSize = Image->sSizeX * Image->sSizeY * Image->bChannels;

    /* allocate memory forAWPSHORT data */
    pS = (SHORT*)malloc(sizeof(SHORT) * lSize);

    if(pS == NULL)
	{
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pB = (AWPBYTE*)Image->pPixels;

    for(i = 0; i < lSize; i++)
	{
        pS[i] = pB[i];
    }

    free((void*)pB);
    Image->dwType = AWP_SHORT;
    Image->pPixels = (void*)pS;

CLEANUP:
    return res;
}

AWPRESULT _awpConvertDoubleToShort(awpImage* Image)
{
    AWPRESULT res;
    SHORT* pS;
    AWPINT i;
    AWPDOUBLE* pD;
    long lSize;

    res = AWP_OK;/*initialize result */

    lSize = Image->sSizeX * Image->sSizeY * Image->bChannels;

    /* allocate memory forAWPSHORT data */
    pS = (SHORT*)malloc(sizeof(SHORT) * lSize);

    if(pS == NULL)
	{
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pD = (AWPDOUBLE*)Image->pPixels;

    for(i = 0; i < lSize; i++)
	{
		pS[i] = (pD[i]<SHRT_MIN)?SHRT_MIN:(pD[i]>SHRT_MAX?SHRT_MAX:(SHORT)pD[i]);
    }

    free((void*)pD);
    Image->dwType = AWP_SHORT;
    Image->pPixels = (void*)pS;

CLEANUP:
    return res;
}

AWPRESULT _awpConvertShortToByte(awpImage* Image)
{
    AWPRESULT res;
    AWPBYTE* pB;
    AWPINT i;
    SHORT* pS;
    long lSize;

    res = AWP_OK;/*initialize result */

    lSize = Image->sSizeX * Image->sSizeY * Image->bChannels;

    /* allocate memory for byte data */
    pB = (AWPBYTE*)malloc(sizeof(AWPBYTE) * lSize);

    if(pB == NULL)
	{
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pS = (SHORT*)Image->pPixels;

    for(i = 0; i < lSize; i++)
	{
		pB[i] = (pS[i]<0)?0:(pS[i]>255?255:pS[i]);
    }

    free((void*)pS);
    Image->dwType = AWP_BYTE;
    Image->pPixels = (void*)pB;

CLEANUP:
    return res;
}

AWPRESULT _awpConvertShortToByteWithNorm(awpImage* Image)
{
    AWPRESULT res;
    AWPBYTE* pB;
    AWPINT i;
	AWPBYTE ch;
    SHORT* pS;
    long lSize;
	SHORT* pMax = NULL, *pMin = NULL;

    res = AWP_OK;/*initialize result */

    lSize = Image->sSizeX * Image->sSizeY * Image->bChannels;

    /* allocate memory for byte data */
    pB = (AWPBYTE*)malloc(sizeof(AWPBYTE) * lSize);

    if(pB == NULL)
	{
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    /* allocate memory for min and max  data */
    pMax = (SHORT*)malloc(sizeof(SHORT) * Image->bChannels);
    if(pMax == NULL)
	{
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pMin = (SHORT*)malloc(sizeof(SHORT) * Image->bChannels);
    if(pMin == NULL)
	{
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    /* full max and min data default value */
    for(ch = 0; ch < Image->bChannels; ch++)
	{
        pMax[ch] = SHRT_MIN;
        pMin[ch] = SHRT_MAX;
    }

    pS = (SHORT*)Image->pPixels;

    /* found min and max in image data */
    for(i=0;i<lSize;i+=Image->bChannels){
        for(ch=0;ch<Image->bChannels;ch++){
            if(pMax[ch]<pS[i+ch]){
                pMax[ch]=pS[i+ch];
            }
            if(pMin[ch]>pS[i+ch]){
                pMin[ch]=pS[i+ch];
            }
        }
    }

    /* normalize data */
    for(i=0;i<lSize;i+=Image->bChannels){
        for(ch=0;ch<Image->bChannels;ch++){
            pB[i+ch]=(AWPBYTE)(255.0*(pS[i+ch]-pMin[ch])/(pMax[ch]-pMin[ch]+0.0001));
        }
    }

    free((void*)pS);
    Image->dwType = AWP_BYTE;
    Image->pPixels = (void*)pB;

CLEANUP:
    if(pMax!=NULL){
	free(pMax);
    }
    
    if(pMin!=NULL){
	free(pMin);
    }

    return res;
}

AWPRESULT _awpConvertShortToAWPAWPDOUBLE(awpImage* Image)
{
    AWPRESULT res;
    AWPDOUBLE* pD;
    AWPINT i;
    SHORT* pS;
    long lSize;

    res = AWP_OK;/*initialize result */

    lSize = Image->sSizeX * Image->sSizeY * Image->bChannels;

    /* allocate memory for AWPDOUBLE data */
    pD = (AWPDOUBLE*)malloc(sizeof(AWPDOUBLE) * lSize);

    if(pD == NULL)
	{
        res = AWP_BADMEMORY;
        _ERR_EXIT_
    }

    pS = (SHORT*)Image->pPixels;

    for(i = 0; i < lSize; i++)
	{
        pD[i] = pS[i];
    }

    free((void*)pS);
    Image->dwType = AWP_DOUBLE;
    Image->pPixels = (void*)pD;

CLEANUP:
    return res;
}

AWPRESULT awpConvertV2(awpImage* Image, long lOptions)
{
    AWPRESULT res;

    res = AWP_OK;/*initialize result */

    /* check image */
    _CHECK_RESULT_((res = awpCheckImage(Image)))

    /* select type image */
    switch(Image->dwType){
    case AWP_BYTE:
        switch(lOptions){
		case AWP_CONVERT_TO_SHORT:
			_CHECK_RESULT_((res = _awpConvertByteToShort(Image)))
			break;
        case AWP_CONVERT_TO_DOUBLE:
//			_CHECK_RESULT_((res=_awpConvertByteToAWPAWPDOUBLE(Image)))
            break;
		case AWP_CONVERT_3TO1_BYTE:
//			_CHECK_RESULT_((res=_awpConvertByte3To1(Image)))
			break;
        default:
            res = AWP_NOTSUPPORT;
            _ERR_EXIT_
        }
        break;
    case AWP_DOUBLE:
        switch(lOptions){
        case AWP_CONVERT_TO_BYTE:
//			_CHECK_RESULT_((res=_awpConvertAWPAWPDOUBLEToByte(Image)))
            break;
		case AWP_CONVERT_TO_SHORT:
			_CHECK_RESULT_((res = _awpConvertDoubleToShort(Image)))
			break;
        case AWP_CONVERT_TO_BYTE_WITH_NORM:
//			_CHECK_RESULT_((res=_awpConvertAWPAWPDOUBLEToByteWithNorm(Image)))
            break;
        default:
            res = AWP_NOTSUPPORT;
            _ERR_EXIT_
        }
        break;
	case AWP_SHORT:
		switch(lOptions){
		case AWP_CONVERT_TO_BYTE:
			_CHECK_RESULT_((res = _awpConvertShortToByte(Image)))
			break;
		case AWP_CONVERT_TO_BYTE_WITH_NORM:
			_CHECK_RESULT_((res = _awpConvertShortToByteWithNorm(Image)))
			break;
		case AWP_CONVERT_TO_DOUBLE:
			_CHECK_RESULT_((res = _awpConvertShortToAWPAWPDOUBLE(Image)))
			break;
		default:
			res = AWP_NOTSUPPORT;
			_ERR_EXIT_
		}
		break;
    default:
        res = AWP_NOTSUPPORT;
        _ERR_EXIT_
    }

CLEANUP:
    return res;
}

#define _AWP_NORM_L1(u)\
	{u* src; \
	src  = (u*)pSrc->pPixels;\
	for (i = 0; i < pSrc->sSizeX*pSrc->sSizeY; i++)\
	{\
		for (k = 0; k < pSrc->bChannels; k++)\
		{\
			norm[k] += (src[i*pSrc->bChannels + k] < 0 ? -src[i*pSrc->bChannels + k] : src[i*pSrc->bChannels + k]);\
		}\
	}\
}


static void _awpNormL1(awpImage* pSrc, awpImage* pNorm)
{
	AWPDOUBLE* norm;
	AWPINT i,k;

    norm = (AWPDOUBLE*)pNorm->pPixels;

	switch(pSrc->dwType)
	{
	case AWP_BYTE:
		_AWP_NORM_L1(AWPBYTE)
		break;
	case AWP_SHORT:
		_AWP_NORM_L1(AWPSHORT)
		break;
	case AWP_FLOAT:
		_AWP_NORM_L1(AWPFLOAT)
		break;
	case AWP_DOUBLE:
		_AWP_NORM_L1(AWPDOUBLE)
		break;
	}
}

#define _AWP_NORM_L2(u)\
	{u* src; \
	AWPDOUBLE* norm;\
	src  = (u*)pSrc->pPixels;\
	for (i = 0; i < pSrc->sSizeX*pSrc->sSizeY; i++)\
	{\
		for (k = 0; k < pSrc->bChannels; k++)\
		{\
			norm[k] += src[i*pSrc->bChannels + k]*src[i*pSrc->bChannels + k];\
		}\
	}\
    for(k = 0; k < pSrc->bChannels; k++)\
        norm[k] = sqrt(norm[k]);\
}

static void _awpNormL2(awpImage* pSrc, awpImage* pNorm)
{
	AWPDOUBLE* norm;
	AWPINT i,k;

    norm = (AWPDOUBLE*)pNorm->pPixels;
	switch(pSrc->dwType)
	{
	case AWP_BYTE:
		_AWP_NORM_L2(AWPBYTE)
		break;
	case AWP_SHORT:
		_AWP_NORM_L2(AWPSHORT)
		break;
	case AWP_FLOAT:
		_AWP_NORM_L2(AWPFLOAT)
		break;
	case AWP_DOUBLE:
		_AWP_NORM_L2(AWPDOUBLE)
		break;
	}
}

AWPRESULT awpNorm(awpImage* pSrc, awpImage** ppNorm, long lOptions)
{
    AWPRESULT res;
    res = AWP_OK;/*initialize result */

	_CHECK_RESULT_((res = awpCheckImage(pSrc)))

	if (ppNorm == NULL)
		_ERROR_EXIT_RES_(AWP_BADARG)

	if (*ppNorm == NULL)
		_CHECK_RESULT_((res = awpCreateImage(ppNorm, pSrc->bChannels, 1, 1, AWP_DOUBLE)))
	else
	{
		_CHECK_RESULT_((res = awpCheckImage(*ppNorm)))
		if ((*ppNorm)->bChannels != 1)
			_ERROR_EXIT_RES_(AWP_BADARG)
		if ((*ppNorm)->sSizeY > 1)
			_ERROR_EXIT_RES_(AWP_BADARG)
		if ((*ppNorm)->dwType != AWP_DOUBLE)
			_ERROR_EXIT_RES_(AWP_BADARG)
		if ((*ppNorm)->sSizeX != pSrc->bChannels)
			_ERROR_EXIT_RES_(AWP_BADARG)
	}


	awpZeroImage(*ppNorm);

	if (lOptions == AWP_NORM_L1)
		_awpNormL1(pSrc, *ppNorm);
	else if (lOptions ==  AWP_NORM_L2)
		_awpNormL2(pSrc, *ppNorm);
	else
		_ERROR_EXIT_RES_(AWP_BADARG)

CLEANUP:
    return res;
}

#define _AWP_NORMALIZE_(v)\
{\
    int i, k; \
    v* src = (v*)pSrc->pPixels;\
    AWPDOUBLE* norm = (AWPDOUBLE*)pNorm->pPixels;\
    AWPDOUBLE* res = (AWPDOUBLE*)pNormImg->pPixels;\
    for (i = 0; i < pSrc->sSizeX*pSrc->sSizeY; i++) \
    {               \
        for (k = 0; k < pSrc->bChannels; k++)\
        {\
            res[i*pSrc->bChannels + k] = src[i*pSrc->bChannels + k] / norm[k];\
        }\
    }\
}\

static void _awpNormalize(awpImage* pSrc, awpImage* pNormImg, awpImage* pNorm)
{
	switch(pSrc->dwType)
	{
	case AWP_BYTE:
		_AWP_NORMALIZE_(AWPBYTE)
		break;
	case AWP_SHORT:
		_AWP_NORMALIZE_(AWPSHORT)
		break;
	case AWP_FLOAT:
		_AWP_NORMALIZE_(AWPFLOAT)
		break;
	case AWP_DOUBLE:
		_AWP_NORMALIZE_(AWPDOUBLE)
		break;
	}
}

AWPRESULT awpNormalize(awpImage* pSrc, awpImage** ppNorm, long lOptions)
{
    AWPRESULT res;
    awpImage* pNorm;

    res = AWP_OK;/*initialize result */
    pNorm = NULL;

    _CHECK_RESULT_((res = awpCheckImage(pSrc)))

	if (ppNorm == NULL)
		_ERROR_EXIT_RES_(AWP_BADARG)

	if (*ppNorm == NULL)
		_CHECK_RESULT_((res = awpCreateImage(ppNorm, pSrc->sSizeX, pSrc->sSizeY, pSrc->bChannels, AWP_DOUBLE)))
	else
	{
		_CHECK_RESULT_((res = awpCheckImage(*ppNorm)))
		if ((*ppNorm)->bChannels != pSrc->bChannels)
			_ERROR_EXIT_RES_(AWP_BADARG)
		if ((*ppNorm)->sSizeY != pSrc->sSizeY)
			_ERROR_EXIT_RES_(AWP_BADARG)
		if ((*ppNorm)->sSizeX != pSrc->sSizeX)
			_ERROR_EXIT_RES_(AWP_BADARG)
		if ((*ppNorm)->dwType != AWP_DOUBLE)
			_ERROR_EXIT_RES_(AWP_BADARG)
	}

    _CHECK_RESULT_((res = awpNorm(pSrc, &pNorm, lOptions)))


    //call normalize
    _awpNormalize(pSrc, *ppNorm, pNorm);


CLEANUP:
    _SAFE_RELEASE_(pNorm)
    return res;
}