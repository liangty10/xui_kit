#include "xui_linebox.h"

xui_implement_rtti(xui_linebox, xui_control);

/*
//constructor
*/
xui_create_explain(xui_linebox)( const xui_vector<s32>& size, xui_component* parent )
: xui_control(size, parent)
{
	m_flow = FLOWSTYLE_H;
}

/*
//flow
*/
xui_method_explain(xui_linebox, get_flow,		u08				)( void ) const
{
	return m_flow;
}
xui_method_explain(xui_linebox, set_flow,		void			)( u08 flow )
{
	if (m_flow != flow)
	{
		m_flow  = flow;
		invalid();
	}
}

/*
//line ctrl
*/
xui_method_explain(xui_linebox, add_linectrl,	void			)( xui_component* component )
{
	if (component->get_parent())
		return;

	component->set_parent(this);
	m_widgetvec.push_back(component);
	invalid();
}
xui_method_explain(xui_linebox, del_linectrl,	void			)( xui_component* component )
{
	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		component);

	if (itor == m_widgetvec.end())
		return;

	component->set_parent(NULL);
	delete component;
	m_widgetvec.erase(itor);
	invalid();
}
xui_method_explain(xui_linebox, get_cornerrt,	xui_rect2d<s32>	)( xui_component* component ) const
{
	xui_rect2d<s32> corner(0);
	if (m_widgetvec.size())
	{
		if (m_widgetvec.front() == component)
		{
			switch (m_flow)
			{
			case FLOWSTYLE_H:
				corner.value[0] = m_corner;
				corner.value[3] = m_corner;
				break;
			case FLOWSTYLE_V:
				corner.value[0] = m_corner;
				corner.value[1] = m_corner;
				break;
			}
		}

		if (m_widgetvec.back () == component)
		{
			switch (m_flow)
			{
			case FLOWSTYLE_H:
				corner.value[1] = m_corner;
				corner.value[2] = m_corner;
				break;
			case FLOWSTYLE_V:
				corner.value[2] = m_corner;
				corner.value[3] = m_corner;
				break;
			}
		}
	}

	return corner;
}

/*
//callback
*/
xui_method_explain(xui_linebox, on_invalid,		void			)( xui_method_args& args )
{
	if (m_widgetvec.size() > 0)
	{
		//计算大小
		xui_vector<s32> sz = (m_flow == FLOWSTYLE_H)
			? xui_vector<s32>(0, get_renderh())
			: xui_vector<s32>(get_renderw(), 0);

		for (u32 i = 0;  i < m_widgetvec.size(); ++i)
		{
			switch (m_flow)
			{
			case FLOWSTYLE_H:
				sz.w += m_widgetvec[i]->get_renderw();
				break;
			case FLOWSTYLE_V:
				sz.h += m_widgetvec[i]->get_renderh();
				break;
			}
		}

		//布局
		if (get_rendersz() != sz)
		{
			set_rendersz(sz);
		}
		else
		{
			perform();
		}
	}
}
xui_method_explain(xui_linebox, on_perform,		void			)( xui_method_args& args )
{
	xui_vector<s32> pt(0);
	xui_vecptr_addloop(m_widgetvec)
	{
		xui_component* comp = m_widgetvec[i];
		comp->on_perform_pt(pt);

		switch (m_flow)
		{
		case FLOWSTYLE_H:
			comp->on_perform_h(get_renderh());
			pt.x += comp->get_renderw();
			break;
		case FLOWSTYLE_V:
			comp->on_perform_w(get_renderw());
			pt.y += comp->get_renderh();
			break;
		}
	}
}