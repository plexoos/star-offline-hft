#include <cmath>

#include "StiScanRootFile.h"

#include "TCanvas.h"
#include "TH2S.h"
#include "TProfile2D.h"
#include "TSystem.h"

#include "StHftPool/EventT/StiScanHistContainer.h"
#include "StHftPool/EventT/StiScanHistsByVolume.h"


StiScanRootFile::StiScanRootFile() : TFile(), mDirs()
{
   BookHists();
}


StiScanRootFile::StiScanRootFile(const char *fname, Option_t *option, const char *ftitle, Int_t compress) :
   TFile(fname, option, ftitle, compress), mDirs()
{
   printf("Created ROOT file: %s\n", GetName());

   BookHists();
}


void StiScanRootFile::BookHists()
{
   mDirs["sti_vol"] = new StiScanHistsByVolume("sti_vol", "sti_vol", "", this);
   mDirs["sti_trk"] = new StiScanHistContainer("sti_trk", "sti_trk", "", this);
   mDirs["gea"] = new StiScanHistContainer("gea", "gea", "", this);
}


void StiScanRootFile::FillHists(const EventT &eventT, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container;

   container = static_cast<StiScanHistContainer*> (mDirs["sti_vol"]);
   container->FillHists(eventT, volumeList);

   container = static_cast<StiScanHistContainer*> (mDirs["sti_trk"]);
   container->FillHists(eventT, volumeList);
}


void StiScanRootFile::FillHists(const EventG &eventG, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container = static_cast<StiScanHistContainer*> (mDirs["gea"]);
   container->FillHists(eventG, volumeList);
}


void StiScanRootFile::FillDerivedHists()
{
   for (TDirMapConstIter iDir=mDirs.begin() ; iDir!=mDirs.end(); ++iDir)
   {
      std::string  dirName = iDir->first;
      StiScanHistContainer *container = (StiScanHistContainer*) iDir->second;

      if (!container) {
         Error("FillDerivedHists", "No container/directory found for key %s. Skipping...", dirName.c_str());
         continue;
      }

      container->FillDerivedHists();
   }
}


Int_t StiScanRootFile::Write(const char* name, Int_t opt, Int_t bufsiz)
{
   Info("Write", "%s", GetName());

   // Set histogram axis limits to the same value calculated in sti_trk container
   StiScanHistContainer* sti_trk = (StiScanHistContainer*) mDirs["sti_trk"];
   StiScanHistContainer* sti_vol = (StiScanHistContainer*) mDirs["sti_vol"];
   StiScanHistContainer* gea = (StiScanHistContainer*) mDirs["gea"];

   sti_vol->SetZRange(sti_trk->GetZMin(), sti_trk->GetZMax());
   gea->SetZRange(sti_trk->GetZMin(), sti_trk->GetZMax());

   for (TDirMapConstIter iDir=mDirs.begin() ; iDir!=mDirs.end(); ++iDir)
   {
      StiScanHistContainer *container = static_cast<StiScanHistContainer*> (iDir->second);
      if (!container) continue;
      container->PrettifyHists();
   }

   return TFile::Write(name, opt, bufsiz);
}


Int_t StiScanRootFile::Write(const char* name, Int_t opt, Int_t bufsiz) const
{
   return TFile::Write(name, opt, bufsiz);
}


void StiScanRootFile::Close(Option_t *option)
{
   TFile::Close(option);
}


void StiScanRootFile::SaveAllAs(std::string prefix)
{
   if (gSystem->mkdir(prefix.c_str()) < 0)
      Warning("SaveAllAs", "Perhaps dir already exists: %s", prefix.c_str());
   else
      Info("SaveAllAs", "Created dir: %s", prefix.c_str());

   for (TDirMapConstIter iDir=mDirs.begin() ; iDir!=mDirs.end(); ++iDir)
   {
      std::string  dirName = iDir->first;
      StiScanHistContainer *container = static_cast<StiScanHistContainer*> (iDir->second);

      if (!container) {
         Error("SaveAllAs", "No container/directory found for key %s. Skipping...", dirName.c_str());
         continue;
      }

      std::string path = prefix + "/" + dirName;

      if (gSystem->mkdir(path.c_str()) < 0)
         Warning("SaveAllAs", "Perhaps dir already exists: %s", path.c_str());
      else
         Info("SaveAllAs", "Created dir: %s", path.c_str());

      container->SaveAllAs(path);
   }
}