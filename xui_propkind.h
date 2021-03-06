#ifndef __xui_propkind_h__
#define __xui_propkind_h__

#include "xui_family.h"
#include "xui_family_render.h"
#include "xui_propdata.h"

typedef xui_kindctrl* (*xui_kind_newctrl)( xui_propkind* propkind );
typedef std::vector<xui_propkind*> xui_propkind_vec;
class xui_propkind
{
public:
	xui_propkind( xui_proproot* root, const std::wstring& name, const std::string& type, xui_kind_newctrl func, xui_bitmap* icon, bool flag, bool headshow = true, bool tail = false, bool plusshow = false, const xui_family& textfont = xui_family(12), const xui_family_render& textdraw = xui_family_render::k_default);
	virtual ~xui_propkind( void );

	xui_proproot*					get_root	( void ) const;
	xui_kind_newctrl				get_func	( void ) const;
	const std::string&				get_type	( void ) const;
	bool							was_tail	( void ) const;
	bool							was_headshow( void ) const;
	bool							was_plusshow( void ) const;
	const xui_family&				get_textfont( void ) const;
	const xui_family_render&		get_textdraw( void ) const;
	const xui_propdata_vec&			get_propdata( void ) const;
	xui_propdata*					get_propdata( const std::wstring& name );
	void							add_propdata( xui_propdata* propdata );
	std::wstring					get_proppath( xui_propdata* propdata );
	const std::wstring&				get_name	( void ) const;
	void							set_name	( const std::wstring& name );
	xui_bitmap*						get_icon	( void ) const;
	void							set_icon	( xui_bitmap* icon );
	bool							get_flag	( void ) const;
	void							set_flag	( bool flag );
	void							non_ctrl	( void );
	xui_kindctrl*					get_ctrl	( void ) const;
	void							set_ctrl	( xui_kindctrl* ctrl );
	bool							can_show	( void ) const;
	void							set_show	( bool flag );
	bool							can_edit	( void ) const;
	void							set_edit	( bool flag );

	xui_method<xui_method_propdata>	xm_propchanged;
	xui_method<xui_method_args>		xm_namechanged;
	xui_method<xui_method_args>		xm_flagchanged;

protected:
	xui_proproot*					m_root;
	xui_kind_newctrl				m_func;
	std::string						m_type;
	bool							m_show;
	bool							m_edit;
	xui_kindctrl*					m_ctrl;
	std::wstring					m_name;
	xui_bitmap*						m_icon;
	bool							m_flag;
	bool							m_tail;
	bool							m_headshow;
	bool							m_plusshow;
	xui_family						m_textfont;
	xui_family_render				m_textdraw;
	xui_propdata_vec				m_propdata;
};

#endif//__xui_propkind_h__