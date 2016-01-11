#include "xui_convas.h"
#include "xui_drawer.h"
#include "xui_propplus.h"
#include "xui_propview.h"
#include "xui_propctrl_expand.h"

//////////////////////////////////////////////////////////////////////////
//propctrl_expand
//////////////////////////////////////////////////////////////////////////
/*
//create
*/
xui_method_explain(xui_propctrl_expand,		 create,			xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_expand(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_expand)( xui_propdata* propdata )
: xui_propctrl()
{
	m_type	   += "propctrlexpand";
	m_border    = xui_rect2d<s32>(0, 6, 0, 6);
	m_backcolor = xui_colour(0.0f);

	//plus
	m_propplus = new xui_propplus();
	xui_method_ptrcall(m_propplus, set_parent	)(this);
	m_widgetvec.push_back(m_propplus);

	//name
	m_namectrl = new xui_drawer("", xui_rect2d<s32>(0, 0,128, 20));
	xui_method_ptrcall(m_namectrl, set_parent	)(this);
	xui_method_ptrcall(m_namectrl, set_backcolor)(xui_colour(0.0f));
	xui_method_ptrcall(m_namectrl, set_sidecolor)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(m_namectrl, set_font		)(xui_family("Arial", 16, false));
	xui_method_ptrcall(m_namectrl, set_textcolor)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(m_namectrl, set_textalign)(TA_LC);
	m_widgetvec.push_back(m_namectrl);

	//prop
	xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(propdata);
	const xui_propdata_vec& vec = dataexpand->get_subprop();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_prop_newctrl  func = vec[i]->get_func();
		xui_propctrl* propctrl = (*func)(vec[i]);
		propctrl->refresh();
		propctrl->set_parent(this);
		m_widgetvec.push_back(propctrl);
		m_propctrlvec.push_back(propctrl);
	}
}

/*
//method
*/
xui_method_explain(xui_propctrl_expand,		 was_expanded,		bool			)( void ) const
{
	return m_propplus->was_expanded();
}
xui_method_explain(xui_propctrl_expand,		 set_expanded,		void			)( bool flag )
{
	xui_propdata_expand* dataexpand = (xui_propdata_expand*)m_propdata;
	if (dataexpand->can_subfold())
	{
		m_propplus->set_expanded(flag);
	}
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_expand,		 on_linkpropdata,	void			)( void )
{
	xui_propdata_expand* dataexpand = (xui_propdata_expand*)m_propdata;
	xui_method_ptrcall(m_namectrl, set_text		)(dataexpand->get_name());
	xui_method_ptrcall(m_propplus, set_visible	)(dataexpand->can_subfold());

	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		xui_propdata_vec propdataall = get_propdataall(i);
		xui_propctrl* propctrl = m_propctrlvec[i];
		propctrl->set_propdata(propdataall);

		for (xui_propdata_vec::iterator itor = propdataall.begin(); itor != propdataall.end(); ++itor)
		{
			(*itor)->set_ctrl(propctrl);
		}
	}
}
xui_method_explain(xui_propctrl_expand,		 on_editvalue,		void			)( xui_propedit* sender )
{}

/*
//override
*/
xui_method_explain(xui_propctrl_expand,		 on_invalid,		void			)( xui_method_args& args )
{
	xui_propctrl::on_invalid(args);

	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		m_propctrlvec[i]->set_visible(false);
	}

	xui_propview* propview = get_propview();
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::LINE_HEIGHT + m_border.ay + m_border.by;

	if (m_propdata && m_propplus->was_expanded())
	{
		xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdata);
		const xui_propdata_vec& vec = dataexpand->get_subprop();
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
			xui_method_ptrcall(propctrl, set_enable )(vec[i]->can_edit());
			xui_method_ptrcall(propctrl, set_visible)(vec[i]->can_show());
			if (vec[i]->can_show() == false)
				continue;

			sz.h += propctrl->get_renderh();
		}
	}

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(xui_propctrl_expand,		 on_perform,		void			)( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 height = xui_propview::LINE_HEIGHT;
	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	//name
	pt.x = 0;
	pt.y = rt.ay;
	sz.w = rt.get_sz().w;
	sz.h = height;
	m_namectrl->set_rendersz(sz, false);
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
	//plus
	pt.x = (indent-16);
	pt.y = rt.ay + (height-12)/2;
	sz.w = 12;
	sz.h = 12;
	m_propplus->set_renderpt(pt, false);
	m_propplus->set_rendersz(sz, false);
	if (m_propplus->was_expanded())
	{
		pt.x = 0;
		pt.y = rt.ay + height;
		sz.w = rt.get_sz().w;
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
			if (propctrl->was_visible() == false)
				continue;

			sz.h  = propctrl->get_renderh();
			propctrl->set_renderpt(pt, false);
			propctrl->set_rendersz(sz, false);
			pt.y += sz.h;
		}
	}
}

/*
//method
*/
xui_method_explain(xui_propctrl_expand,		 get_propdataall,	xui_propdata_vec)( u32 index )
{
	xui_propdata_vec result;
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_expandbase* dataexpand = dynamic_cast<xui_expandbase*>(m_propdatavec[i]);
		const xui_propdata_vec& vec = dataexpand->get_subprop();
		result.push_back(vec[index]);
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
//propctrl_expandbool
//////////////////////////////////////////////////////////////////////////
/*
//create
*/
xui_method_explain(xui_propctrl_expand_bool, create,			xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_expand_bool(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_expand_bool)( xui_propdata* propdata )
: xui_propctrl_expand(propdata)
{
	xui_propedit_bool* editbool = new xui_propedit_bool(this);

	xui_drawer*  namectrl = editbool->get_namectrl();
	xui_control* textctrl = editbool->get_editctrl();
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(textctrl);

	m_propedit = editbool;
	m_namectrl->set_visible(false);
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_expand_bool)( void )
{
	delete m_propedit;
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_expand_bool, on_linkpropdata,	void			)( void )
{
	xui_propctrl_expand::on_linkpropdata();
	m_propedit->reset();
	xui_drawer* namectrl = m_propedit->get_namectrl();
	namectrl->set_text(m_propdata->get_name());

	bool same = true;
	xui_propdata_bool* databool = (xui_propdata_bool*)m_propdata;
	bool value = databool->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_bool* data = (xui_propdata_bool*)m_propdatavec[i];
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		m_propedit->set_value(value);
	}
}
xui_method_explain(xui_propctrl_expand_bool, on_editvalue,		void			)( xui_propedit* sender )
{
	bool value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_bool* data = (xui_propdata_bool*)m_propdatavec[i];
		data->set_value(value);
	}

	refresh();
}

/*
//override
*/
xui_method_explain(xui_propctrl_expand_bool, on_perform,		void			)( xui_method_args& args )
{
	xui_propctrl_expand::on_perform(args);
	s32 height = xui_propview::LINE_HEIGHT;

	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* boolctrl = m_propedit->get_editctrl();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	//boolctrl
	pt.x = rt.get_sz().w/2;
	pt.y = rt.ay + height/2 - boolctrl->get_renderh()/2;
	boolctrl->set_renderpt(pt, false);
	//namectrl
	pt.x = 0;
	pt.y = rt.ay;
	sz.w = rt.get_sz().w/2;
	sz.h = height;
	namectrl->set_renderpt(pt, false);
	namectrl->set_rendersz(sz, false);

	s32 indent = get_indent();
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}

//////////////////////////////////////////////////////////////////////////
//propctrl_expandenum
//////////////////////////////////////////////////////////////////////////
/*
//create
*/
xui_method_explain(xui_propctrl_expand_enum, create,			xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_expand_enum(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_expand_enum)( xui_propdata* propdata )
: xui_propctrl_expand(propdata)
{
	xui_propdata_enum* dataenum = (xui_propdata_enum*)propdata;
	xui_propedit_enum* editenum = new xui_propedit_enum(this, dataenum->get_textmap());

	xui_drawer*  namectrl = editenum->get_namectrl();
	xui_control* textctrl = editenum->get_editctrl();
	namectrl->set_parent(this);
	textctrl->set_parent(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(textctrl);

	m_propedit = editenum;
	m_namectrl->set_visible(false);
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_expand_enum)( void )
{
	delete m_propedit;
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_expand_enum, on_linkpropdata,	void			)( void )
{
	xui_propctrl_expand::on_linkpropdata();
	m_propedit->reset();
	xui_drawer* namectrl = m_propedit->get_namectrl();
	namectrl->set_text(m_propdata->get_name());

	bool same = true;
	xui_propdata_enum* dataenum = (xui_propdata_enum*)m_propdata;
	u32 value = dataenum->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_enum* data = (xui_propdata_enum*)m_propdatavec[i];
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		m_propedit->set_value(value);
	}
}
xui_method_explain(xui_propctrl_expand_enum, on_editvalue,		void			)( xui_propedit* sender )
{
	u32 value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_enum* data = (xui_propdata_enum*)m_propdatavec[i];
		data->set_value(value);
	}

	refresh();
}

/*
//override
*/
xui_method_explain(xui_propctrl_expand_enum, on_perform,		void			)( xui_method_args& args )
{
	xui_propctrl_expand::on_perform(args);
	s32 height = xui_propview::LINE_HEIGHT;

	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* enumctrl = m_propedit->get_editctrl();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	//enumctrl
	pt.x = rt.get_sz().w/2;
	pt.y = rt.ay + height/2 - enumctrl->get_renderh()/2;
	sz.w = rt.get_sz().w/2;
	sz.h = enumctrl->get_renderh();
	enumctrl->set_renderpt(pt, false);
	enumctrl->set_rendersz(sz, false);
	//namectrl
	pt.x = 0;
	pt.y = rt.ay;
	sz.w = rt.get_sz().w/2;
	sz.h = height;
	namectrl->set_renderpt(pt, false);
	namectrl->set_rendersz(sz, false);

	s32 indent = get_indent();
	namectrl->set_textoffset(xui_vector<s32>(indent, 0));
}