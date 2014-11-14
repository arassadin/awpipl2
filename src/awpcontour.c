#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "_awpipl.h"

AWPRESULT awpCreateContour(awpContour** ppContour, AWPINT NumPoints, AWPBOOL dir)
{
    AWPRESULT res = AWP_OK;
    if (NumPoints < 0)
        _ERROR_EXIT_RES_(AWP_BADARG);
    if (ppContour == NULL)
        _ERROR_EXIT_RES_(AWP_BADARG);

    *ppContour = NULL;
    *ppContour = (awpContour*)malloc(sizeof(awpContour));
    if (*ppContour == NULL)
        _ERROR_EXIT_RES_(AWP_BADMEMORY);
    (*ppContour)->Points = NULL;
    (*ppContour)->Points = (awpPoint*)malloc(NumPoints*sizeof(awpPoint));
    if ((*ppContour)->Points == NULL)
        _ERROR_EXIT_RES_(AWP_BADMEMORY);
    memset((*ppContour)->Points, 0, NumPoints*sizeof(awpPoint));
    (*ppContour)->NumPoints = (DWORD)NumPoints;
    (*ppContour)->Direction = dir;

CLEANUP:
    if (res != AWP_OK)
    {
        /*cleanup variables*/
        *ppContour = NULL;
        NumPoints  = 0;
    }
    return res;
}

AWPRESULT awpFreeContour(awpContour** ppContour)
{
    AWPRESULT res = AWP_OK;
    if (ppContour == NULL)
        _ERROR_EXIT_RES_(AWP_BADARG);
    free((*ppContour)->Points);
    free(*ppContour);
    *ppContour = NULL;
CLEANUP:
    return res;
}


AWPRESULT awpGetContourRect(const awpContour* pContour, awpRect* rect)
{
    AWPRESULT res = AWP_OK;
    // инициализируем переменные
    WORD min_x = 0;
    WORD max_x = 0;
    WORD min_y = 0;
    WORD max_y = 0;
    DWORD i = 0;
    //проверим аргументы
    if (pContour == NULL)
        _ERROR_EXIT_RES_(AWP_BADARG);
    if (pContour->NumPoints <= 0)
        _ERROR_EXIT_RES_(AWP_BADARG);
    if (rect == NULL)
        _ERROR_EXIT_RES_(AWP_BADARG);
    min_x = pContour->Points[0].X;
    min_y = pContour->Points[0].Y;
    max_x = min_x;
    max_y = min_y;
    // выполним поиск
    for (i = 1; i < pContour->NumPoints; i++)
    {
        if (min_x > pContour->Points[i].X)
            min_x = pContour->Points[i].X;
        if (max_x < pContour->Points[i].X)
            max_x = pContour->Points[i].X;
        if (min_y > pContour->Points[i].Y)
            min_y = pContour->Points[i].Y;
        if (max_y < pContour->Points[i].Y)
            max_y = pContour->Points[i].Y;
    }
    // сохраним результат
    rect->left = min_x;
    rect->top  = min_y;
    rect->right = max_x;
    rect->bottom = max_y;
    
CLEANUP:
    return res;

}
static AWPINT mymax (AWPINT a, AWPINT b, AWPBOOL* Flag){
	*Flag = TRUE;
	if (a > b ) return a;
	*Flag = FALSE;
	return b;
}

static AWPINT mymin (AWPINT a, AWPINT b){
if (a < b) return a;
return b;
}
// функция принадлежности точки области  // не используется
static AWPBOOL IsPixelBelongArea(AWPINT NumPoints, awpPoint* P, AWPINT x, AWPINT y){
   AWPINT Count, i, j, m, x0;
   AWPBOOL F;
   AWPDOUBLE t;
   Count = 0;
   for (i = 0; i < NumPoints-1; i++)    {
      j =  (i+1);
      if (P[i].Y == P[j].Y) continue;
      if (P[i].Y > y && P[j].Y > y) continue;
      if (P[i].Y < y && P[j].Y < y) continue;

      m = mymax(P[i].Y,P[j].Y, &F);
      if (F) x0 = P[i].X; else x0 = P[j].X;
      if (m == y && x0 > x)
       Count = 1 - Count;
      else if (mymin(P[i].Y,P[j].Y)== y) continue;
      else{
      t = ((AWPDOUBLE)y - P[i].Y) / (P[j].Y - P[i].Y);
      if (t > 0 && t <1 && P[i].X + t*(P[j].X - P[i].X) >= x) Count = 1 - Count;
      }
   }
   return (Count == 0)?FALSE:TRUE;
}

AWPRESULT awpIsPointInContour(const awpContour* pContour, const awpPoint* p, AWPBOOL* result)
{
    AWPRESULT res = AWP_OK;
    if (pContour == NULL)
        _ERROR_EXIT_RES_(AWP_BADARG);
    if (pContour->NumPoints <= 0)
        _ERROR_EXIT_RES_(AWP_BADARG);
    if (p == NULL)
        _ERROR_EXIT_RES_(AWP_BADARG);
    if (result == NULL)
        _ERROR_EXIT_RES_(AWP_BADARG);
    /*важное замечание. Начальная и конечная точки контура должны совпадать*/
    *result = IsPixelBelongArea(pContour->NumPoints, pContour->Points, p->X, p->Y);
CLEANUP:
    return res;
}
