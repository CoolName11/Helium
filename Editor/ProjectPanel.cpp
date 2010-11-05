#include "Precompile.h"

#include "ProjectPanel.h"
#include "ArtProvider.h"

#include "Pipeline/Asset/AssetClass.h"

#include "Editor/FileDialog.h"
#include "Editor/Controls/MenuButton.h"

using namespace Helium;
using namespace Helium::Editor;

ProjectPanel::ProjectPanel( wxWindow *parent, DocumentManager* documentManager )
: ProjectPanelGenerated( parent )
, m_DocumentManager( documentManager )
, m_Project( NULL )
, m_Model( NULL )
, m_OptionsMenu( NULL )
, m_DropTarget( NULL )
{
#pragma TODO( "Remove this block of code if/when wxFormBuilder supports wxArtProvider" )
    {
        Freeze();

        m_AddFileButton->SetBitmap( wxArtProvider::GetBitmap( ArtIDs::Actions::FileAdd ) );
        m_AddFileButton->Enable( false );
        //m_AddFileButton->Hide();

        m_DeleteFileButton->SetBitmap( wxArtProvider::GetBitmap( ArtIDs::Actions::FileDelete ) );
        m_DeleteFileButton->Enable( false );
        //m_DeleteFileButton->Hide();

        m_OptionsButton->SetBitmap( wxArtProvider::GetBitmap( ArtIDs::Actions::Options, wxART_OTHER, wxSize(16, 16) ) );
        m_OptionsButton->SetMargins( 3, 3 );
        m_OptionsButton->Hide();

        m_ProjectManagementPanel->Layout();

        Layout();
        Thaw();
    }

    SetHelpText( TXT( "This is the project outliner.  Manage what's included in your project here." ) );

    m_OptionsMenu = new wxMenu();
    {
        //wxMenuItem* detailsMenuItem = new wxMenuItem(
        //    m_OptionsMenu,
        //    VaultMenu::ViewResultDetails,
        //    VaultMenu::Label( VaultMenu::ViewResultDetails ),
        //    VaultMenu::Label( VaultMenu::ViewResultDetails ).c_str(),
        //    wxITEM_RADIO );
        //m_OptionsMenu->Append( detailsMenuItem );
        //Connect( VaultMenu::ViewResultDetails, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ProjectPanel::OnOptionsMenuSelect ), NULL, this );
    }
    m_OptionsButton->SetContextMenu( m_OptionsMenu );
    m_OptionsButton->SetHoldDelay( 0.0f );
    m_OptionsButton->Connect( wxEVT_MENU_OPEN, wxMenuEventHandler( ProjectPanel::OnOptionsMenuOpen ), NULL, this );
    m_OptionsButton->Connect( wxEVT_MENU_CLOSE, wxMenuEventHandler( ProjectPanel::OnOptionsMenuClose ), NULL, this );
    m_OptionsButton->Enable( true );
    m_OptionsButton->Hide();    

    m_DataViewCtrl->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( ProjectPanel::OnSelectionChanged ), NULL, this );

    std::set< tstring > extension;
    Asset::AssetClass::GetExtensions( extension );

#pragma TODO("Why isn't hrb part of the AssetClass extensions?")
    extension.insert( TXT( "hrb" ) );
    extension.insert( TXT( "obj" ) );

    m_DataViewCtrl->EnableDropTarget( wxDF_FILENAME );
    m_DropTarget = new FileDropTarget( extension );
    //m_DropTarget->AddDragOverListener( FileDragEnterSignature::Delegate( this, &ProjectPanel::DragEnter ) );
    m_DropTarget->AddDragOverListener( FileDragOverSignature::Delegate( this, &ProjectPanel::OnDragOver ) );
    //m_DropTarget->AddDragLeaveListener( FileDragLeaveSignature::Delegate( this, &ProjectPanel::DragLeave ) );
    m_DropTarget->AddDroppedListener( FileDroppedSignature::Delegate( this, &ProjectPanel::OnDroppedFiles ) );
    m_DataViewCtrl->GetMainWindow()->SetDropTarget( m_DropTarget );
}

ProjectPanel::~ProjectPanel()
{
    m_OptionsButton->Disconnect( wxEVT_MENU_OPEN, wxMenuEventHandler( ProjectPanel::OnOptionsMenuOpen ), NULL, this );
    m_OptionsButton->Disconnect( wxEVT_MENU_CLOSE, wxMenuEventHandler( ProjectPanel::OnOptionsMenuClose ), NULL, this );
}

void ProjectPanel::SetProject( Project* project, const Document* document )
{
#pragma TODO( " Rachel WIP: "__FUNCTION__" - Clean up m_Project beforing setting new one" )
    if ( m_Project )
    {
        if ( m_Model )
        {
            m_DocumentManager->e_DocumentAdded.RemoveMethod( m_Model.get(), &ProjectViewModel::OnDocumentAdded );
            m_DocumentManager->e_DocumentRemoved.RemoveMethod( m_Model.get(), &ProjectViewModel::OnDocumentRemoved );

            m_Model = NULL;
        }

        m_Project = NULL;
    }

    m_AddFileButton->Enable( false );
    m_DeleteFileButton->Enable( false );

    m_Project = project;
    if ( m_Project )
    {
        m_Model = new ProjectViewModel( m_DocumentManager );
        m_Model->SetProject( project, document );

        m_DocumentManager->e_DocumentAdded.AddMethod( m_Model.get(), &ProjectViewModel::OnDocumentAdded );
        m_DocumentManager->e_DocumentRemoved.AddMethod( m_Model.get(), &ProjectViewModel::OnDocumentRemoved );

        m_Model->ResetColumns();
        m_DataViewCtrl->AppendColumn( m_Model->CreateColumn( ProjectModelColumns::Name ) );
        m_DataViewCtrl->AppendColumn( m_Model->CreateColumn( ProjectModelColumns::FileSize ) );

        // the ctrl will now hold ownership via reference count
        m_DataViewCtrl->AssociateModel( m_Model.get() );

        m_AddFileButton->Enable( true );
    }

#pragma TODO( " Rachel WIP: "__FUNCTION__" - Do we actually need to refresh?" )
    Refresh();
}

void ProjectPanel::OnAddFile( wxCommandEvent& event )
{
    if ( m_Project )
    {
        FileDialog openDlg( this, TXT( "Open" ), m_Project->a_Path.Get().Directory().c_str() );
#pragma TODO("Set file dialog filters from Asset::AssetClass::GetExtensions")
        //openDlg.AddFilters( ...

        if ( openDlg.ShowModal() == wxID_OK )
        {
            Path path( (const wxChar*)openDlg.GetPath().c_str() );

            Asset::AssetClassPtr asset;
            if ( _tcsicmp( path.Extension().c_str(), TXT( "hrb" ) ) == 0 )
            {
                asset = Asset::AssetClass::LoadAssetClass( path );
            }
            else
            {
                asset = Asset::AssetClass::Create( path );
            }

            if ( asset.ReferencesObject() )
            {
                m_Project->AddPath( asset->GetSourcePath() );
            }
        }
    }
}

void ProjectPanel::OnDeleteFile( wxCommandEvent& event )
{
    if ( m_Project )
    {
        wxDataViewItemArray selection;
        int numSeleted = m_DataViewCtrl->GetSelections( selection );

        for( int index = 0; index < numSeleted; ++index )
        {
            if ( selection[index].IsOk() )
            {
                m_Model->RemoveItem( selection[index] );
            }
        }
    }
}

void ProjectPanel::OnOptionsMenuOpen( wxMenuEvent& event )
{
    event.Skip();
    //if ( event.GetMenu() == m_OptionsMenu )
    //{
    //    // refresh menu's view toggles
    //}
}

void ProjectPanel::OnOptionsMenuClose( wxMenuEvent& event )
{
    m_DataViewCtrl->SetFocus();
    event.Skip();
}

void ProjectPanel::OnOptionsMenuSelect( wxCommandEvent& event )
{
    event.Skip();

    //switch( event.GetId() )
    //{
    //default:
    //    break;
    //};
}

void ProjectPanel::OnSelectionChanged( wxDataViewEvent& event )
{
    wxDataViewItemArray selection;
    int numSeleted = m_DataViewCtrl->GetSelections( selection );
    m_DeleteFileButton->Enable( numSeleted > 0 ? true : false );
}

///////////////////////////////////////////////////////////////////////////////
void ProjectPanel::OnDragOver( FileDroppedArgs& args )
{
    //Path path( args.m_Path );

    //// it's a project file
    //if ( _tcsicmp( path.FullExtension().c_str(), TXT( "project.hrb" ) ) == 0 ) 
    //{
    //    // allow user to drop a project in
    //}
    //else if ( m_Project )
    //{
        wxDataViewItem item;
        wxDataViewColumn* column;
        m_DataViewCtrl->HitTest( wxPoint( args.m_X, args.m_Y ), item, column );

        if ( item.IsOk() && !m_Model->IsDropPossible( item ) )
        {
            args.m_DragResult = wxDragNone;
        }    
    //}
    //else
    //{
    //    args.m_DragResult = wxDragNone;
    //}
}

void ProjectPanel::OnDroppedFiles( const FileDroppedArgs& args )
{
    Path path( args.m_Path );

    // it's a project file
    if ( _tcsicmp( path.FullExtension().c_str(), TXT( "project.hrb" ) ) == 0 ) 
    {
#pragma TODO( " Rachel WIP: "__FUNCTION__" - Close the current project and open the dropped one")
        //(MainFrame*)GetParent()->OpenProject( path );
    }
    else if ( m_Project )
    {
        Asset::AssetClassPtr asset;
        if ( _tcsicmp( path.Extension().c_str(), TXT( "hrb" ) ) == 0 )
        {
            asset = Asset::AssetClass::LoadAssetClass( path );
        }
        else
        {
            asset = Asset::AssetClass::Create( path );
        }

        if ( asset.ReferencesObject() )
        {
            m_Project->AddPath( asset->GetSourcePath() );
        }
    }
}

