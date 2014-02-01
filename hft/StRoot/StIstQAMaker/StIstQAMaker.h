/***************************************************************************
*
* $Id$
*
* Author: Yaping Wang, October 2013
****************************************************************************
* Description: 
* Generate several histograms and trees for IST raw hits and hits.
****************************************************************************
*
* $Log$
* Revision 1.2  2014/01/29 18:25:02  ypwang
* updating scripts
*
*
****************************************************************************
* StIstQAMaker.h,v 1.0
* Revision 1.0 2013/11/04 16:20:30 Yaping
* Initial version
****************************************************************************/

#ifndef StIstQAMaker_hh     
#define StIstQAMaker_hh

#include <string>

#include "StMaker.h"
#include "StRoot/StIstUtil/StIstConsts.h"
#include "TString.h"
#include "TNtuple.h"
#include "TH2F.h"
#include "TProfile.h"

class StEvent;
class StTrack;

class StIstQAMaker : public StMaker {
public:
  StIstQAMaker(const char *name="ist_Qa");     // constructor
  ~StIstQAMaker();                          	// destructor
  Int_t Init();
  Int_t  Make();                      		// invoked for every event
  Int_t  Finish();                    		// called once at the end
  virtual const char *GetCVS() const {
    static const char cvs[]="Tag $Name$ $Id$ built "__DATE__" "__TIME__ ; 
    return cvs;
  };

protected:
  TFile* myRootFile;
  TTree *istRawHitTree;
  TTree *istHitTree;

  struct rawHitInfo {
	int channelId, geoId, ladder, sensor, column, row, maxTimeBin, idTruth, EventId;
	float charge, chargeErr;
  } istRawHit;
  
  struct hitInfo {
	int hitId, ladder, sensor, idTruth, EventId, maxTimeBin, clusteringType, nRawHits, nRawHitsZ, nRawHitsRPhi;
	float meanColumn, meanRow, localX, localY, localZ, x, y, z, charge, chargeErr;
  } istHit;

  //position
  TH2F* rawHitMap[kIstNumSensors]; // raw hit column vs row per sensor
  TH2F* hitMap[kIstNumSensors];    // hit mean column vs mean row per sensor
  TH2F* hitMapOfIST;		   // hit map of IST layer in r-phi vs. Z area
  TH2F* hitGlobalXY;		   // hit global x vs y
  TH2F* hitGlobalPhiZ;		   // hit global z
  //Charge
  TH2F* rawHitCharge_TimeBin[kIstNumTimeBins]; // Charge (ADC) vs channel ID over all time bins
  TH2F* rawHitChargeErr;  // RMS noise vs channel ID
  TH2F* hitCharge_SensorId;         // Charge vs sensorID
  TH2F* hitChargeErr_SensorId;      // Charge uncertainty vs sensorID
  TH2F* maxTimeBin_SensorId;	    // max ADC time bin vs sensorID
  //hit or raw hit number
  TH2F* numOfRawHits_SensorId;      // number of raw hits vs sensor Id
  TH2F* numOfHits_SensorId;	    // number of hits vs sensor Id
  TProfile* numOfRawHits_EventId[kIstNumSensors];
  //cluster size
  TH2F* clusterSize_SensorId;	    // hit cluster size
  TH2F* clusterSizeZ_SensorId;	    // hit cluster size in Z direction
  TH2F* clusterSizeRPhi_SensorId;   // hit cluster size in R-Phi direction

  std::string mRootFilename;
  Int_t  mEventCounter;             // Event countter

private:
  ClassDef(StIstQAMaker,1);
};

// deconstructor
inline StIstQAMaker::~StIstQAMaker(){ /*    */ };

#endif