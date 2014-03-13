/***************************************************************************
*
* $Id$
*
* Author: Yaping Wang, March 2013
****************************************************************************
* Description:
* Data structure for individual IST hit (a 1D cluster).
****************************************************************************
*
* $Log$
* Revision 1.9  2014/03/13 22:10:30  smirnovd
* Expand tabs and trim trailing whitespace
*
* Revision 1.8  2014/03/13 22:10:21  smirnovd
* Fixed constructor's initialization list
*
* Revision 1.7  2014/03/13 22:10:12  smirnovd
* Move some constants from StIstUtil/StIstConsts.h to StEvent/StEnumerations.h to avoid external dependance of StEvent on StIstUtil
*
* Revision 1.6  2014/02/26 21:18:08  smirnovd
* Style corrected with astyle -s3 -p -H -A3 -k3 -O -o -y -Y -f
*
* Revision 1.5  2014/02/26 01:35:36  ypwang
* get rid of meanColumn/meanRow/Apv transformations and local position uncertainties to avoid external constants access
*
* Revision 1.4  2014/02/25 17:04:50  ypwang
* get rid of mClusteringType and its accessory/modifier functions
*
*
****************************************************************************
* StIstHit.h,v 1.0
* Revision 1.0 2013/11/04 15:25:30 Yaping
* Initial version
****************************************************************************/

#ifndef StIstHit_hh
#define StIstHit_hh

#include "StHit.h"
#include "StEvent/StEnumerations.h"

using namespace StIstConsts;

class StIstHit : public StHit
{
public:
   StIstHit(unsigned char ladder = -1, unsigned char sensor = -1, float Charge = 0., float ChargeErr = 0.,
      unsigned char maxTB = 0, unsigned char nRawHits = 1, unsigned char nRawHitsZ = 0,
      unsigned char nRawHitsRPhi = 0);
   StIstHit(const StThreeVectorF &position, const StThreeVectorF &error, unsigned int hwPosition, float charge,
      unsigned char trackRefCount = 0);

   virtual StDetectorId detector() const;
   void    setDetectorId(StDetectorId);

   //accessories
   unsigned char        getLadder() const;
   unsigned char        getSensor() const;
   unsigned char        getMaxTimeBin() const;
   float                getChargeErr() const;
   unsigned char        getNRawHits() const;
   unsigned char        getNRawHitsZ() const;
   unsigned char        getNRawHitsRPhi() const;
   float                localPosition(unsigned int ) const;

   //modifiers
   void    setMaxTimeBin(unsigned char tb);
   void    setChargeErr(float chargeErr);
   void    setNRawHits(unsigned char nRawHits);//cluster size
   void    setNRawHitsZ(unsigned char nRawHitsZ);
   void    setNRawHitsRPhi(unsigned char nRawHitsRPhi);
   void    setLocalPosition(float, float, float);

protected:
   UChar_t mMaxTimeBin;        // max charge time bin
   Float_t mChargeErr;         // charge uncertainty
   UChar_t mNRawHits;          // nRawHits: cluster size
   UChar_t mNRawHitsZ;         // cluster size in Z direction
   UChar_t mNRawHitsRPhi;      // cluster size in r-phi direction

   Float_t mLocalPosition[3];  //local position of hit inside the sensor

   StDetectorId mDetectorId;

   ClassDef(StIstHit, 1)
};

ostream &operator<<(ostream &, const StIstHit &);

////////////////////////
inline unsigned char StIstHit::getLadder() const          { return 1 + (mHardwarePosition - 1) / kIstNumSensorsPerLadder;};
inline unsigned char StIstHit::getSensor() const          { return 1 + (mHardwarePosition - 1) % kIstNumSensorsPerLadder;};
inline unsigned char StIstHit::getMaxTimeBin() const      { return mMaxTimeBin;         };
inline float StIstHit::getChargeErr()    const            { return mChargeErr;          };
inline unsigned char StIstHit::getNRawHits() const        { return mNRawHits;           };
inline unsigned char StIstHit::getNRawHitsZ() const       { return mNRawHitsZ;          };
inline unsigned char StIstHit::getNRawHitsRPhi() const    { return mNRawHitsRPhi;       };

inline void StIstHit::setMaxTimeBin(unsigned char tb)                   { mMaxTimeBin = tb;             };
inline void StIstHit::setChargeErr(float chargeErr)                     { mChargeErr = chargeErr;       };
inline void StIstHit::setNRawHits(unsigned char nRawHits)               { mNRawHits = nRawHits;         };
inline void StIstHit::setNRawHitsZ(unsigned char nRawHitsZ)             { mNRawHitsZ = nRawHitsZ;       };
inline void StIstHit::setNRawHitsRPhi(unsigned char nRawHitsRPhi)       { mNRawHitsRPhi = nRawHitsRPhi; };

#endif
