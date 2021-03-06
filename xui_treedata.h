#ifndef __xui_treedata_h__
#define __xui_treedata_h__

#include "xui_family.h"
#include "xui_family_render.h"
#include "xui_component.h"

class xui_treedata
{
	friend class xui_treenode;

public:
	xui_treedata( void );
	xui_treedata( const std::wstring& text );
	xui_treedata( const std::wstring& text, xui_bitmap* icon );
	xui_treedata( const std::wstring& text, xui_bitmap* icon, bool flag );
	virtual ~xui_treedata( void ){};

	xui_treenode*				get_node		( void );

	virtual bool				get_flag		( u32 index );
	virtual xui_bitmap*			get_icon		( u32 index );
	virtual std::wstring		get_text		( u32 index );
	virtual xui_family			get_textfont	( u32 index );
	virtual xui_family_render	get_textdraw	( u32 index );
	virtual void				set_text		( u32 index, const std::wstring& text );
	virtual void				set_flag		( u32 index, bool flag );

protected:
	void						set_node		( xui_treenode* node );

	xui_treenode*				m_node;
	xui_bitmap*					m_icon;
	std::wstring				m_text;
	bool						m_flag;
};

#endif//__xui_treedata_h__