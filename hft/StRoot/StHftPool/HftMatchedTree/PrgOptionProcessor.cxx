#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/regex.hpp>

#include "TFile.h"

#include "PrgOptionProcessor.h"


PrgOptionProcessor::PrgOptionProcessor() : TObject(),
   fOptions("Program options", 120), fOptionsValues(), fHftreeFile(), fVolumeListFile(),
   fVolumePattern(),
   fVolumeList(), fMaxEventsUser(0), fSparsity(1)
{
   InitOptions();
}


PrgOptionProcessor::PrgOptionProcessor(int argc, char **argv) : TObject(),
   fOptions("Program options", 120), fOptionsValues(), fHftreeFile(), fVolumeListFile(),
   fVolumePattern(),
   fVolumeList(), fMaxEventsUser(0), fSparsity(1)
{
   InitOptions();
   ProcessOptions(argc, argv);
}


void PrgOptionProcessor::InitOptions()
{
   // Declare supported options
   fOptions.add_options()
      ("help,h",              "Print help message")
      ("hftree-file,f",       po::value<std::string>(&fHftreeFile), "Full path to a ROOT file containing a 'hftree' TTree " \
       "OR a text file with a list of such ROOT files")
      ("volume-pattern,p",    po::value<std::string>(&fVolumePattern)->implicit_value("process_all_volumes"), "A regex pattern Sti/TGeo volume names")
      ("volume-pattern-flist,l",   po::value<std::string>(&fVolumeListFile), "Full path to a text file with Sti/TGeo volume names")
      ("max-events,n",        po::value<unsigned int>(&fMaxEventsUser)->default_value(0), "Maximum number of events to process")
      ("sparsity,s",          po::value<float>(&fSparsity)->default_value(1), "Approximate fraction of events to read and process")
   ;

   // Set default values for Sti volume name patterns. These are used if the user does not use any
   // of the options to modify them
   fVolumeList.insert("^.*IDSM_1/IBMO_1/IBAM_[\\d]+/ILSB.*$");
   fVolumeList.insert("^.*IDSM_1/PXMO_1/PXLA_[\\d]+/LADR_\\d/PXSI_[\\d]+/PLAC.*$");
}


/**
 * Takes the standard command line arguments and parses them with the boost
 * program_options utility. Additional checks are implemented to verify the
 * validity of the supplied arguments.
 */
void PrgOptionProcessor::ProcessOptions(int argc, char **argv)
{
   using namespace std;

   po::store(po::parse_command_line(argc, argv, fOptions), fOptionsValues);
   po::notify(fOptionsValues);

   if (fOptionsValues.count("help"))
   {
      std::cout << fOptions << std::endl;
      exit(EXIT_FAILURE);
   }

   Info("ProcessOptions", "User provided options:");


   if (fOptionsValues.count("hftree-file"))
   {
      std::string hftreeFile = boost::any_cast<std::string>(fOptionsValues["hftree-file"].value());

      std::cout << "fHftreeFile: " << hftreeFile << std::endl;
      std::ifstream tmpFileCheck(hftreeFile.c_str());
      if (!tmpFileCheck.good()) {
         Error("ProcessOptions", "File \"%s\" does not exist", hftreeFile.c_str());
         tmpFileCheck.close();
         exit(EXIT_FAILURE);
      }
   } else {
      Error("ProcessOptions", "Input file not set");
      exit(EXIT_FAILURE);
   }

   if (fOptionsValues.count("volume-pattern-flist"))
   {
      std::cout << "fVolumeListFile: " << fVolumeListFile << std::endl;
      std::ifstream volListFile(fVolumeListFile.c_str());

      if (!volListFile.good()) {
         Error("ProcessOptions", "File \"%s\" does not exist", fVolumeListFile.c_str());
         volListFile.close();
         exit(EXIT_FAILURE);
      }

      fVolumeList.clear();
      std::string pattern;

      while ( volListFile.good() )
      {
         volListFile >> pattern;

         try {
            boost::regex re(pattern);
         }
         catch (boost::regex_error& e) {
            Error("ProcessOptions", "Provided regex \"%s\" is not valid", pattern.c_str());
            exit(EXIT_FAILURE);
         }

         if (volListFile.eof()) break;

         fVolumeList.insert(pattern);
      }

      Info("ProcessOptions", "User patterns (fVolumeList) are:");
      copy(fVolumeList.begin(), fVolumeList.end(), ostream_iterator<string>(std::cout, "\n"));
   }


   if (fOptionsValues.count("volume-pattern"))
   {
      // Process implicit value first
      if (fVolumePattern.compare("process_all_volumes") == 0) {
         fVolumeList.clear();
      }
      else {

         try {
            boost::regex re(fVolumePattern);
         }
         catch (boost::regex_error& e) {
            Error("ProcessOptions", "Provided regex \"%s\" is not valid", fVolumePattern.c_str());
            exit(EXIT_FAILURE);
         }

         fVolumeList.clear();
         fVolumeList.insert(fVolumePattern);
      }

      Info("ProcessOptions", "User patterns (fVolumeList) are:");
      copy(fVolumeList.begin(), fVolumeList.end(), ostream_iterator<string>(std::cout, "\n"));
   }


   if (fOptionsValues.count("max-events"))
   {
      std::cout << "max-events: " << fMaxEventsUser << std::endl;
   }

   if (fOptionsValues.count("sparsity"))
   {
      if (fSparsity > 1 || fSparsity <= 0) {
         Warning("ProcessOptions", "Sparsity specified value outside allowed limits. Set to 1");
         fSparsity = 1;
      }
      std::cout << "sparsity: " << fSparsity << std::endl;
   }
}


bool PrgOptionProcessor::MatchedVolName(std::string & volName) const
{
   if (volName.empty() || fVolumeList.empty())
      return false;

   std::set<std::string>::const_iterator iPattern = fVolumeList.begin();

   for( ; iPattern != fVolumeList.end(); ++iPattern )
   {
      boost::regex r(*iPattern);

      if ( boost::regex_match(volName, r) )
         return true;
   }

   return false;
}


TChain* PrgOptionProcessor::BuildHftreeChain(std::string name)
{
   TChain *chain = new TChain(name.c_str(), "READ");

   TFile file( fHftreeFile.c_str() );

   if ( file.IsZombie() )
   {
      Warning("BuildHftreeChain", "Not a root file: %s", fHftreeFile.c_str());

      std::ifstream hftreeListFile(fHftreeFile.c_str());
      std::string hftreeFile;

      while ( hftreeListFile.good() )
      {
         hftreeListFile >> hftreeFile;
         if (hftreeListFile.eof()) break;

         chain->AddFile( hftreeFile.c_str() );
      }
   } else
   {
      Info("BuildHftreeChain", "Good root file: %s", fHftreeFile.c_str());
      chain->AddFile( fHftreeFile.c_str() );
   }

   return chain;
}