#ifndef __xui_timehead_h__
#define __xui_timehead_h__

#include "xui_control.h"

class xui_timehead : public xui_control
{
	xui_declare_rtti

public:
	xui_timehead( xui_timeview* timeview );

protected:
	virtual void				on_mousedown		( xui_method_mouse& args );
	virtual void				on_mousemove		( xui_method_mouse& args );
	virtual void				on_mouserise		( xui_method_mouse& args );
	virtual void				on_renderself		( xui_method_args&  args );

	s32							hit_frame			( s32 x );

	xui_vector<s32>				m_downrela;
	xui_vector<s32>				m_currrela;
};

#endif//__xui_timehead_h__