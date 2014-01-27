/*!
 * \class StPxlClusterCollection
 * \author Qiu Hao, March 2013
 * \Initial Revision.
 */
/***************************************************************************
 *
 * $Id$
 *
 * Author: Qiu Hao, March 2013
 ***************************************************************************
 *
 * Description:
 * pxl cluster collection
 * More information at
 * https://www.star.bnl.gov/protected/heavy/qiuh/HFT/software/PXL_software.pdf
 *
 ***************************************************************************
 *
 * $Log$
 * Revision 1.2  2014/01/27 02:37:02  qiuh
 * *** empty log message ***
 *
 *
 **************************************************************************/

#include "StPxlClusterCollection.h"

ClassImp(StPxlClusterCollection)

StPxlClusterCollection::StPxlClusterCollection() : StObject()
{
}

void StPxlClusterCollection::addCluster(Int_t sector, Int_t ladder, Int_t sensor, const StPxlCluster &cluster)
{
   mClusterVec[sector - 1][ladder - 1][sensor - 1].push_back(cluster);
}

Int_t StPxlClusterCollection::numberOfClusters(Int_t sector, Int_t ladder, Int_t sensor) const
{
   return mClusterVec[sector - 1][ladder - 1][sensor - 1].size();
}

const StPxlCluster *StPxlClusterCollection::cluster(Int_t sector, Int_t ladder, Int_t sensor, Int_t clusterIndex) const
{
   return &mClusterVec[sector - 1][ladder - 1][sensor - 1][clusterIndex];
}
