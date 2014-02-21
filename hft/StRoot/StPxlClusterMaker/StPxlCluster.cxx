/*!
 * \class StPxlCluster
 * \author Qiu Hao, Jan 2013, according codes from Xiangming Sun
 * \Initial Revision.
 */
/***************************************************************************
 *
 * $Id$
 *
 * Author: Qiu Hao, Jan 2013, according codes from Xiangming Sun
 ***************************************************************************
 *
 * Description:
 * a group of neighboring pixel raw hits
 * More information at
 * https://www.star.bnl.gov/protected/heavy/qiuh/HFT/software/PXL_software.pdf
 *
 ***************************************************************************
 *
 * $Log$
 * Revision 1.4  2014/02/21 21:11:06  smirnovd
 * Minor style and empty space adjustments
 *
 * Revision 1.3  2014/01/28 19:29:35  qiuh
 * *** empty log message ***
 *
 *
 **************************************************************************/

#include "StPxlCluster.h"
#include "StPxlRawHitMaker/StPxlRawHit.h"
#include <algorithm>
#include <map>

using namespace std;

ClassImp(StPxlCluster)


StPxlCluster::StPxlCluster()
{
   mColumnCenter = -9999;
   mRowCenter = -9999;
   mIdTruth = -9999;
}


Int_t StPxlCluster::nRawHits() const
{
   return mRawHitVec.size();
}


void StPxlCluster::addRawHit(const StPxlRawHit *rawHit)
{
   mRawHitVec.push_back(rawHit);
}


bool compareSecond(const pair<int, int> &pair1, const pair<int, int> &pair2)
{
   return pair1.second < pair2.second;
}


void StPxlCluster::summarize(int embeddingShortCut)
{
   // calculate average column and row
   float columnSum = 0;
   float rowSum    = 0;
   int   nRawHits_ = nRawHits();

   for (int i = 0; i < nRawHits_; i++) {
      columnSum += mRawHitVec[i]->column();
      rowSum += mRawHitVec[i]->row();
   }

   mColumnCenter = columnSum / float(nRawHits_);
   mRowCenter = rowSum / float(nRawHits_);

   // find the most frequent raw hit idTruth as cluster idTruth
   mIdTruth = 0;
   if (embeddingShortCut) {return;}
   map<int, int> idTruthMap;
   for (int i = 0; i < nRawHits_; i++) {
      if (mRawHitVec[i]->idTruth()) {idTruthMap[mRawHitVec[i]->idTruth()] ++;}
   }

   if (idTruthMap.size()) {
      mIdTruth = max_element(idTruthMap.begin(), idTruthMap.end(), compareSecond)->first;
   }
}
