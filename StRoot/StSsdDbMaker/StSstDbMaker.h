#ifndef STSSTDBMAKER_H
#define STSSTDBMAKER_H

#ifndef StMaker_H
#include "StMaker.h"
#endif
class St_ssdDimensions;
class St_ssdWafersPosition;
class St_slsCtrl;
class slsCtrl_st;
class ssdConfiguration_st;
class ssdHotChips_st;
#include "StSsdUtil/StSsdBarrel.hh"
#include "THashList.h"

class StSstDbMaker : public StMaker
{
private:
   StSsdBarrel           *mySsd;
   St_ssdDimensions      *m_dimensions;//!
   St_ssdWafersPosition  *m_positions;//!
   ssdConfiguration_st   *m_config;//!
   slsCtrl_st            *m_ctrl;//!
   Int_t                   mode;//!
   EReturnCodes           mReady; ///< Status code returned by Make(). It is !kStOk if failed to access DB tables
   static THashList *fRotList;
   map<unsigned int,short> mMapHotChips; //!

public:
   StSstDbMaker(const char *name = "SstDb");
   virtual       ~StSstDbMaker();
   virtual Int_t  InitRun(Int_t runNumber);
   virtual Int_t  Make();
   virtual THashList *GetRotations() {return fRotList;}
   virtual St_ssdWafersPosition *CalculateWafersPosition();
   virtual StSsdBarrel  *GetSsd() {return mySsd;}
   virtual slsCtrl_st   *GetSlsCtrl() {return m_ctrl;}
   virtual Int_t        GetMode() {return mode;}
   virtual St_ssdWafersPosition *GetssdWafersPos() {return m_positions;}
   virtual St_ssdDimensions     *GetssdDimensions() {return m_dimensions;}

   static const TGeoHMatrix *getHMatrixSensorOnGlobal(int ladder, int sensor);

   Int_t chipHot(Int_t side, Int_t ladder, Int_t wafer, Int_t chip) const; 
   void setHotChips(ssdHotChips_st *hotChipsTable);
   
   virtual const char *GetCVS() const
   {static const char cvs[] = "Tag $Name:  $ $Id: StSstDbMaker.h,v 1.2 2015/02/10 17:05:12 bouchet Exp $ built "__DATE__" "__TIME__ ; return cvs;}
   ClassDef(StSstDbMaker, 0)  //StAF chain virtual base class for Makers
};
// Global pointers:
R__EXTERN StSstDbMaker *gStSstDbMaker;
#endif


