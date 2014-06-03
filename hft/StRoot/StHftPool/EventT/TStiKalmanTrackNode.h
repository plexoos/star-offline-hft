#ifndef TStiKalmanTrackNode_h
#define TStiKalmanTrackNode_h

#include "TObject.h"
#include "TVector3.h"

#include "Sti/StiKalmanTrackNode.h"


class TStiKalmanTrackNode : public TObject
{
public:

   TStiKalmanTrackNode();
   TStiKalmanTrackNode(const StiKalmanTrackNode &stiKTN);
   TStiKalmanTrackNode & operator=(const StiKalmanTrackNode &stiKTN);
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);

protected:

   TVector3    fPosition;          ///< Coordinates of the track state/node position
   TVector3    fTrackP;            ///< Local track momentum vector
   double      fEnergyLosses;      ///< Energy lost in the volume
   double      fNodeRadius;        ///< The nominal radius of the Sti volume associated with this node
   std::string fStiDetectorName;

   ClassDef(TStiKalmanTrackNode, 1)
};


bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);


#endif
