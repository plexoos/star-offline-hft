#ifndef HftMatchedTree_h
#define HftMatchedTree_h

#include "StMaker.h"
#include "TFile.h"
#include "TTree.h"

class EventT;


class HftMatchedTree : public StMaker
{
public:

   HftMatchedTree(const Char_t *name = "HftMatTree");
   virtual ~HftMatchedTree();
   virtual Int_t Init();
   virtual Int_t InitRun(Int_t runnumber);
   virtual Int_t Make();
   virtual Int_t Finish();

   void SetMinNoHits(Int_t minNoHits = 0) { fMinNoHits = minNoHits; }
   void SetpCut(Double_t pCut = 0.0) { fpCut = pCut; }

protected:

   TTree    *fTree;
   EventT   *fEvent;

private:

   void SetTree();

   TFile    *fFile;
   Int_t     fMinNoHits;
   Double_t  fpCut;

   ClassDef(HftMatchedTree, 0)
};

#endif
