#ifndef __cocos_tileview_h__
#define __cocos_tileview_h__

#include "xui_control.h"

namespace cocos2d
{
	class Sprite;
	class SpriteFrame;
	class DrawNode;
}

class cocos_filedata;
class cocos_propfile;
class cocos_renderview;
class cocos_tileview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const s32							name_size = 20;
	static const s32							vert_grap = 10;

	/*
	//constructor
	*/
	cocos_tileview( void );

	/*
	//destructor
	*/
	virtual ~cocos_tileview( void );

	/*
	//method
	*/
	xui_treeview*								get_lineview				( void );
	cocos_renderview*							get_drawview				( void );
	xui_scroll*									get_viewroll				( void );
	xui_treenode*								get_viewfile				( void );
	s32											get_tilesize				( void ) const;
	void										set_viewfile				( xui_treenode* viewfile, const std::wstring& viewtext );
	void										set_viewtext				( const std::wstring& viewtext );
	void										set_tilesize				( s32 size );
	void										get_tileinfo				( s32& s, s32& c, s32& g, s32& w, s32& h );
	xui_treenode*								get_tilenode				( const xui_vector<s32>& mouse, xui_rect2d<s32>& tilert );
	void										set_tilevisible				( xui_treenode* node );
	std::vector<xui_treenode*>					get_shownodes				( void );

protected:
	/*
	//event
	*/
	void										on_drawviewinvalid			( xui_component* sender, xui_method_args&	  args );
	void										on_drawviewrenderself		( xui_component* sender, xui_method_args&	  args );
	void										on_drawviewkeybddown		( xui_component* sender, xui_method_keybd&	  args );
	void										on_drawviewmousedown		( xui_component* sender, xui_method_mouse&	  args );
	void										on_drawviewmouseclick		( xui_component* sender, xui_method_mouse&	  args );
	void										on_drawviewmousewheel		( xui_component* sender, xui_method_mouse&	  args );
	void										on_drawviewmousedoubleclick	( xui_component* sender, xui_method_mouse&	  args );
	void										on_drawviewmousedragitem	( xui_component* sender, xui_method_dragdrop& args );
	void										on_editctrlnonfocus			( xui_component* sender, xui_method_args&     args );
	void										on_editctrltextenter		( xui_component* sender, xui_method_args&	  args );

	/*
	//method
	*/
	void										draw_file					( const xui_rect2d<s32>& rt, xui_treenode* node );
	void										draw_icon					( const xui_rect2d<s32>& rt, xui_treenode* node );
	void										draw_name					( const xui_rect2d<s32>& rt, xui_treenode* node );
	void										draw_leaf					( const xui_rect2d<s32>& rt, xui_treenode* node );
	void										draw_texture				( const xui_rect2d<s32>& rt, xui_treenode* node );
	void										draw_frame					( const xui_rect2d<s32>& rt, cocos2d::SpriteFrame* frame );
	void										draw_node					( s32 ic, s32 ir, s32 x, s32 y, s32 w, s32 s, xui_treenode* node );
	void										draw_background				( s32 ic, s32 ir, s32 c, s32 g, s32 w, s32 h, u32 count );
	cocos2d::Sprite*							push_2dsprite				( void );

	/*
	//member
	*/
	xui_treenode*								m_viewfile;
	std::wstring								m_viewtext;
	s32											m_tilesize;
	cocos_renderview*							m_drawview;
	xui_scroll*									m_viewroll;
	xui_textbox*								m_editctrl;
	cocos2d::DrawNode*							m_drawnode;
	u32											m_curindex;
	std::vector<cocos2d::Sprite*>				m_2dsprite;
	std::map<xui_treenode*, xui_rect2d<s32> >	m_plusnode;
};

#endif//__cocos_tileview_h__