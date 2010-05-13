#pragma once

#include "API.h"

#include "PreferencesBase.h"

namespace Luna
{
  /////////////////////////////////////////////////////////////////////////////
  // Choice of how file paths should be displayed in the UI.
  // 
  namespace FilePathOptions
  {
    enum FilePathOption
    {
      FileName,
      FileNamePlusExt,
      PartialPath,
      FullPath
    };

    static void FilePathOptionEnumerateEnumeration( Reflect::Enumeration* info )
    {
      info->AddElement(FileName, "FileName");
      info->AddElement(FileNamePlusExt, "FileNamePlusExt");
      info->AddElement(PartialPath, "PartialPath");
      info->AddElement(FullPath, "FullPath");
    }
  };
  typedef FilePathOptions::FilePathOption FilePathOption;

  // Get the UI label for a file path based upon the specified FilePathOption
  LUNA_EDITOR_API std::string TuidToLabel( const tuid& fileID, const FilePathOption filePathOption );

  /////////////////////////////////////////////////////////////////////////////
  // Base class for preferenced within Luna.  Provides convenience functions
  // for saving and loading.
  // 
  class LUNA_EDITOR_API Preferences NOC_ABSTRACT : public PreferencesBase
  {
  public:
    static const char* s_ResetPreferences;
    static const char* s_ResetPreferencesLong;

  public:
    static void InitializeType();
    static void CleanupType();

    Preferences();

    virtual std::string GetPreferencesPath() const = 0;
    void SavePreferences();
    void LoadPreferences();

  public:
    REFLECT_DECLARE_ABSTRACT( Preferences, PreferencesBase );
    static void EnumerateClass( Reflect::Compositor<Preferences>& comp );
  };
  typedef Nocturnal::SmartPtr< Preferences > PreferencesPtr;
}