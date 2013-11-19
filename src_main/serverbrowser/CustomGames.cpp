//====== Copyright © 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "pch_serverbrowser.h"
#include <vgui_controls/HTML.h>
#include <vgui_controls/MessageDialog.h>

using namespace vgui;

#define NUM_COMMON_TAGS			20

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
TagMenuButton::TagMenuButton(Panel *parent, const char *panelName, const char *text) : BaseClass(parent,panelName,text)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void TagMenuButton::OnShowMenu( vgui::Menu *menu )
{
	PostActionSignal(new KeyValues("TagMenuButtonOpened"));
	BaseClass::OnShowMenu(menu);
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CCustomServerInfoURLQuery : public vgui::QueryBox
{
	DECLARE_CLASS_SIMPLE( CCustomServerInfoURLQuery, vgui::QueryBox );
public:
	CCustomServerInfoURLQuery(const char *title, const char *queryText,vgui::Panel *parent) : BaseClass( title, queryText, parent )
	{
		SetOKButtonText( "#ServerBrowser_CustomServerURLButton" );
	}
};

DECLARE_BUILD_FACTORY( TagInfoLabel );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
TagInfoLabel::TagInfoLabel(Panel *parent, const char *panelName) : BaseClass(parent,panelName, (const char *)NULL, NULL)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
TagInfoLabel::TagInfoLabel(Panel *parent, const char *panelName, const char *text, const char *pszURL) : BaseClass(parent,panelName,text,pszURL)
{
}

//-----------------------------------------------------------------------------
// Purpose: If we were left clicked on, launch the URL
//-----------------------------------------------------------------------------
void TagInfoLabel::OnMousePressed(MouseCode code)
{
	if (code == MOUSE_LEFT)
	{
		if ( GetURL() )
		{
			// Pop up the dialog with the url in it
			CCustomServerInfoURLQuery *qb = new CCustomServerInfoURLQuery( "#ServerBrowser_CustomServerURLWarning", "#ServerBrowser_CustomServerURLOpen", this );
			if (qb != NULL)
			{
				qb->SetOKCommand( new KeyValues("DoOpenCustomServerInfoURL") );
				qb->AddActionSignalTarget(this);
				qb->MoveToFront();
				qb->DoModal();
			}
		}
	} 
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void TagInfoLabel::DoOpenCustomServerInfoURL( void )
{
	if ( GetURL() )
	{
		system()->ShellExecute("open", GetURL() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CCustomGames::CCustomGames(vgui::Panel *parent) : 
	BaseClass(parent, "CustomGames", eInternetServer )
{
	m_pGameList->AddColumnHeader(9, "Tags", "#ServerBrowser_Tags", 200);
	m_pGameList->SetSortFunc(9, TagsCompare);
	m_pGameList->SetSortColumn(9);

	if ( !IsSteamGameServerBrowsingEnabled() )
	{
		m_pGameList->SetEmptyListText("#ServerBrowser_OfflineMode");
		m_pConnect->SetEnabled( false );
		m_pRefreshAll->SetEnabled( false );
		m_pRefreshQuick->SetEnabled( false );
		m_pAddServer->SetEnabled( false );
		m_pFilter->SetEnabled( false );
	}

	m_szTagFilter[0] = 0;

	m_pTagFilter = new TextEntry(this, "TagFilter");
	m_pTagFilter->SetEnabled( false );
	m_pTagFilter->SetMaximumCharCount( MAX_TAG_CHARACTERS );

	m_pAddTagList = new TagMenuButton( this, "AddTagList", "#ServerBrowser_AddCommonTags" );
	m_pTagListMenu = new Menu( m_pAddTagList, "TagList" );
	m_pAddTagList->SetMenu( m_pTagListMenu );
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CCustomGames::~CCustomGames()
{
}		

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCustomGames::UpdateDerivedLayouts( void )
{
	if ( m_pFilter->IsSelected() )
	{
		LoadControlSettings( "servers/CustomGamesPage_Filters.res" );
	}
	else
	{
		LoadControlSettings( "servers/CustomGamesPage.res" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCustomGames::OnLoadFilter(KeyValues *filter)
{
	BaseClass::OnLoadFilter( filter );

	Q_strncpy(m_szTagFilter, filter->GetString("gametype"), sizeof(m_szTagFilter));

	if ( m_pTagFilter )
	{
		m_pTagFilter->SetText(m_szTagFilter);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CCustomGames::CheckTagFilter( gameserveritem_t &server )
{
	// Servers without tags go in the official games, servers with tags go in custom games
	bool bOfficialServer = !( server.m_szGameTags && server.m_szGameTags[0] );
	if ( bOfficialServer )
		return false;

	// Custom games substring matches tags with the server's tags
	int count = Q_strlen( m_szTagFilter );
	if ( count )
	{
		CUtlVector<char*> TagList;
		V_SplitString( m_szTagFilter, ",", TagList );
		for ( int i = 0; i < TagList.Count(); i++ )
		{
			if ( !Q_strnistr( server.m_szGameTags, TagList[i], MAX_TAG_CHARACTERS ) )
				return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: gets filter settings from controls
//-----------------------------------------------------------------------------
void CCustomGames::OnSaveFilter(KeyValues *filter)
{
	BaseClass::OnSaveFilter( filter );

	if ( m_pTagFilter )
	{
		// tags
		m_pTagFilter->GetText(m_szTagFilter, sizeof(m_szTagFilter) - 1);
	}

	if ( m_szTagFilter[0] )
	{
		Q_strlower(m_szTagFilter);
	}

	// Need to remove the basegamepage's "notags"
	for ( int i = 0; i < m_vecServerFilters.Count(); i++ )
	{
		if ( !Q_strnicmp( m_vecServerFilters[i].m_szKey, "gametype", 8 ) )
		{
			m_vecServerFilters.Remove(i);
			break;
		}
	}
	m_vecServerFilters.AddToTail( MatchMakingKeyValuePair_t( "gametype", m_szTagFilter ) );
	filter->SetString("gametype", m_szTagFilter);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCustomGames::SetRefreshing(bool state)
{
	if ( state )
	{
		m_pAddTagList->SetEnabled( false );
	}

	BaseClass::SetRefreshing( state );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCustomGames::ServerResponded( int iServer, gameserveritem_t *pServerItem )
{
	CBaseGamesPage::ServerResponded( iServer, pServerItem );

	// If we've found a server with some tags, enable the add tag button
	if ( pServerItem->m_szGameTags[0] )
	{
		m_pAddTagList->SetEnabled( true );
	}
}

struct tagentry_t
{
	const char *pszTag;
	int iCount;
};
int __cdecl SortTagsInUse( const tagentry_t *pTag1, const tagentry_t *pTag2 )
{
	return (pTag1->iCount < pTag2->iCount);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCustomGames::RecalculateCommonTags( void )
{
	// Regenerate our tag list
	m_pTagListMenu->DeleteAllItems();

	// Loop through our servers, and build a list of all the tags
	CUtlVector<tagentry_t> aTagsInUse;

	int iCount = m_pGameList->GetItemCount();
	for ( int i = 0; i < iCount; i++ )
	{
		int serverID = m_pGameList->GetItemUserData( i );
		gameserveritem_t *pServer = GetServer( serverID ); 
		if ( pServer && pServer->m_szGameTags && pServer->m_szGameTags[0] )
		{
			CUtlVector<char*> TagList;
			V_SplitString( pServer->m_szGameTags, ",", TagList );

			for ( int iTag = 0; iTag < TagList.Count(); iTag++ )
			{
				// First make sure it's not already in our list
				bool bFound = false;
				for ( int iCheck = 0; iCheck < aTagsInUse.Count(); iCheck++ )
				{
					if ( !Q_strnicmp(TagList[iTag], aTagsInUse[iCheck].pszTag, MAX_TAG_CHARACTERS ) )
					{
						aTagsInUse[iCheck].iCount++;
						bFound = true;
					}
				}

				if ( !bFound )
				{
					int iIdx = aTagsInUse.AddToTail();
					aTagsInUse[iIdx].pszTag = TagList[iTag];
					aTagsInUse[iIdx].iCount = 0;
				}
			}
		}
	}

	aTagsInUse.Sort( SortTagsInUse );

	int iTagsToAdd = min( aTagsInUse.Count(), NUM_COMMON_TAGS );
	for ( int i = 0; i < iTagsToAdd; i++ )
	{
		const char *pszTag = aTagsInUse[i].pszTag;
		m_pTagListMenu->AddMenuItem( pszTag, new KeyValues("AddTag", "tag", pszTag), this, new KeyValues( "data", "tag", pszTag ) );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCustomGames::OnTagMenuButtonOpened( void )
{
	RecalculateCommonTags();
}

//-----------------------------------------------------------------------------
// Purpose: Sets the text from the message
//-----------------------------------------------------------------------------
void CCustomGames::OnAddTag(KeyValues *params)
{
	KeyValues *pkvText = params->FindKey("tag", false);
	if (!pkvText)
		return;

	AddTagToFilterList( pkvText->GetString() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCustomGames::AddTagToFilterList( const char *pszTag )
{
	char txt[ 128 ];
	m_pTagFilter->GetText( txt, sizeof( txt ) );

	if ( txt[0] )
	{
		CUtlVector<char*> TagList;
		V_SplitString( txt, ",", TagList );
		for ( int i = 0; i < TagList.Count(); i++ )
		{
			// Already in the tag list?
			if ( !Q_stricmp(TagList[i],pszTag) )
				return;
		}
	}

	// Append it
	char tmptags[MAX_TAG_CHARACTERS];
	tmptags[0] = '\0';
	if ( txt[0] )
	{
		Q_strncpy( tmptags, txt, MAX_TAG_CHARACTERS );
		Q_strncat( tmptags, ",", MAX_TAG_CHARACTERS );
	}
	Q_strncat( tmptags, pszTag, MAX_TAG_CHARACTERS );
	m_pTagFilter->SetText(tmptags);

	// Update & apply filters now that the tag list has changed
	UpdateFilterSettings();
	ApplyGameFilters();
}