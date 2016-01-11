#include "xui_textbox.h"
#include "xui_slider.h"
#include "xui_scrollthumb.h"
#include "xui_propctrl.h"
#include "xui_propedit_slider.h"

/*
//constructor
*/
xui_create_explain(xui_propedit_slider)( xui_propctrl* propctrl, f64 interval, f64 minvalue, f64 maxvalue )
: xui_propedit(propctrl)
{
	m_minvalue = minvalue;
	m_maxvalue = maxvalue;
	m_editnumb = new xui_propedit_number(propctrl, interval);
	xui_drawer*  namectrl = m_editnumb->get_namectrl();
	xui_textbox* textctrl = (xui_textbox*)m_editnumb->get_editctrl();
	namectrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlnonfocus);
	namectrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlgetfocus);
	namectrl->xm_mouserise		+= new xui_method_member<xui_method_mouse, xui_propedit_slider>(this, &xui_propedit_slider::on_namectrlmouserise);
	textctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlnonfocus);
	textctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlgetfocus);
	textctrl->xm_textchanged	+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_textctrltextchanged);
	textctrl->xm_textenter		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_textctrltextenter);

	m_spinctrl = new xui_slider("", xui_rect2d<s32>(0, 0, 32, 20), FLOWSTYLE_H);
	xui_method_ptrcall(m_spinctrl, set_backcolor)(xui_colour(0.0f));
	m_spinctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_base>(m_editnumb, &xui_propedit_base::on_editctrlnonfocus);
	m_spinctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_base>(m_editnumb, &xui_propedit_base::on_editctrlgetfocus);
	m_spinctrl->xm_nonfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlnonfocus);
	m_spinctrl->xm_getfocus		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlgetfocus);
	m_spinctrl->xm_scroll		+= new xui_method_member<xui_method_args,  xui_propedit_slider>(this, &xui_propedit_slider::on_spinctrlscroll);

	xui_scrollthumb* thumb = m_spinctrl->get_thumb();
	thumb->set_sidestyle(SIDESTYLE_N);
	thumb->set_sidecolor(xui_colour(1.0f, 0.2f, 0.3f, 0.9f));
}

/*
//destructor
*/
xui_delete_explain(xui_propedit_slider)( void )
{
	delete m_editnumb;
}

/*
//method
*/
xui_method_explain(xui_propedit_slider,	get_editnumb,			xui_propedit_number*)( void ) const
{
	return m_editnumb;
}
xui_method_explain(xui_propedit_slider, get_spinctrl,			xui_slider*			)( void ) const
{
	return m_spinctrl;
}

xui_method_explain(xui_propedit_slider, get_value,				f64					)( void ) const
{
	return m_editnumb->get_value();
}
xui_method_explain(xui_propedit_slider, set_value,				void				)( f64 value )
{
	m_editnumb->set_value(value);

	f64 interval = m_editnumb->get_interval();
	s32 scroll_range = (s32)((m_maxvalue - m_minvalue) / interval);
	s32 scroll_value = (s32)((value      - m_minvalue) / interval);
	m_spinctrl->ini_scroll(scroll_range, scroll_value);
}

/*
//override
*/
xui_method_explain(xui_propedit_slider, reset,					void				)( void )
{
	m_editnumb->reset();

	xui_scrollthumb* thumb = m_spinctrl->get_thumb();
	thumb->set_sidestyle(SIDESTYLE_N);
	s32 scroll_range = m_spinctrl->get_range();
	s32 scroll_value = 0;
	m_spinctrl->ini_scroll(scroll_range, scroll_value);
}

/*
//event
*/
xui_method_explain(xui_propedit_slider, on_spinctrlgetfocus,	void				)( xui_componet* sender, xui_method_args&  args )
{
	xui_scrollthumb* thumb = m_spinctrl->get_thumb();
	thumb->set_sidestyle(SIDESTYLE_S);
}
xui_method_explain(xui_propedit_slider, on_spinctrlnonfocus,	void				)( xui_componet* sender, xui_method_args&  args )
{
	xui_scrollthumb* thumb = m_spinctrl->get_thumb();
	thumb->set_sidestyle(SIDESTYLE_N);
	if (sender == m_editnumb->get_editctrl())
	{
		check_value();
	}
}
xui_method_explain(xui_propedit_slider, on_namectrlmouserise,	void				)( xui_componet* sender, xui_method_mouse& args )
{
	check_value();
}
xui_method_explain(xui_propedit_slider, on_textctrltextchanged, void				)( xui_componet* sender, xui_method_args&  args )
{
	f64 interval = m_editnumb->get_interval();
	s32 scroll_range = (s32)((m_maxvalue - m_minvalue) / interval);
	s32 scroll_value = (s32)((get_value()- m_minvalue) / interval);
	m_spinctrl->ini_scroll(scroll_range, scroll_value);
}
xui_method_explain(xui_propedit_slider, on_textctrltextenter,	void				)( xui_componet* sender, xui_method_args&  args )
{
	check_value();
}
xui_method_explain(xui_propedit_slider, on_spinctrlscroll,		void				)( xui_componet* sender, xui_method_args&  args )
{
	f64 interval = m_editnumb->get_interval();
	f64 value    = m_minvalue + (f64)m_spinctrl->get_value() * interval;
	m_editnumb->set_value(value);
	m_propctrl->on_editvalue(this);
}

/*
//method
*/
xui_method_explain(xui_propedit_slider, check_value,			void				)( void )
{
	f64 value = get_value();
	if (value < m_minvalue || value > m_maxvalue)
	{
		value = xui_max(value, m_minvalue);
		value = xui_min(value, m_maxvalue);

		std::wstringstream tmp;
		tmp << value;
		xui_textbox* textctrl = (xui_textbox*)m_editnumb->get_editctrl();
		textctrl->set_text(tmp.str());
	}
}