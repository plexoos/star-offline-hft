#include <cmath>

#include "StiScanRootFile.h"

#include "TCanvas.h"
#include "TH2S.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TSystem.h"

#include "StHftPool/EventT/StiScanHistContainer.h"


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
   mDirs["sti"] = new StiScanHistContainer("sti", "sti", "", this);
   mDirs["gea"] = new StiScanHistContainer("gea", "gea", "", this);
}


void StiScanRootFile::FillHists(const EventT &eventT, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container = static_cast<StiScanHistContainer*> (mDirs["sti"]);
   container->FillHists(eventT, volumeList);
}


void StiScanRootFile::FillHists(const EventG &eventG)
{
   StiScanHistContainer* container = static_cast<StiScanHistContainer*> (mDirs["gea"]);
   container->FillHists(eventG);
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

   StiScanHistContainer* sti = (StiScanHistContainer*) mDirs["sti"];
   StiScanHistContainer* gea = (StiScanHistContainer*) mDirs["gea"];

   gea->SetZRange(sti->GetZMin(), sti->GetZMax());

   for (TDirMapConstIter iDir=mDirs.begin() ; iDir!=mDirs.end(); ++iDir)
   {
      StiScanHistContainer *container = static_cast<StiScanHistContainer*> (iDir->second);
      if (!container) continue;
      container->PrettifyHists();
   }

   TFile::Write(name, opt, bufsiz);
}


Int_t StiScanRootFile::Write(const char* name, Int_t opt, Int_t bufsiz) const
{
   TFile::Write(name, opt, bufsiz);
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
