#ifndef TStiKalmanTrackNode_h
#define TStiKalmanTrackNode_h

#include <set>
#include <string>

#include "TObject.h"
#include "TVector3.h"

#include "Sti/StiKalmanTrackNode.h"


class TStiKalmanTrackNode : public TObject
{
public:

   TStiKalmanTrackNode();
   TStiKalmanTrackNode(const StiKalmanTrackNode &stiKTN);
   TStiKalmanTrackNode & operator=(const StiKalmanTrackNode &stiKTN);
   const TVector3& GetTrackP() const { return fTrackP; }
   const TVector3& GetPosition() const { return fPosition; }
   float GetEnergyLosses() const { return -fEnergyLosses; }
   float GetNodeRadius() const { return fNodeRadius; }
   float GetNodeCenterRefAngle() const { return fNodeCenterRefAngle; }
   std::string GetVolumeName() const { return fVolumeName; }
   bool   MatchedVolName(const std::string & pattern) const;
   bool   MatchedVolName(const std::set<std::string> & patterns) const;
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);

protected:

   TVector3    fPosition;          ///< Coordinates of the track state/node position
   TVector3    fTrackP;            ///< Track momentum vector in global CS
   float       fEnergyLosses;      ///< Energy lost in the volume
   float       fNodeRadius;        ///< The nominal radius of the Sti volume associated with this node
   float       fNodeCenterRefAngle;   ///< Angle to the center of the Sti volume associated with this node
   std::string fVolumeName;           ///< Name of Sti volume

   ClassDef(TStiKalmanTrackNode, 1)
};


bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs);


#endif