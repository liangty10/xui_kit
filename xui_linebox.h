#ifndef __xui_linebox_h__
#define __xui_linebox_h__

#include "xui_control.h"

class xui_linebox : public xui_control
{
	xui_declare_rtti

public:
	static xui_linebox*		create				( void );

	xui_linebox( const xui_vector<s32>& size, u08 flow = k_flowstyle_h );

	u08						get_flow			( void ) const;
	void					set_flow			( u08 flow );
	void					add_linectrl		( xui_control* ctrl );
	void					del_linectrl		( xui_control* ctrl );
	virtual xui_rect2d<s32>	get_cornerrt		( xui_component* component ) const;
	virtual void			render_else			( void );

protected:
	virtual void			on_invalid			( xui_method_args& args );
	virtual void			on_perform			( xui_method_args& args );

	u08						m_flow;
};

#endif//__xui_linebox_h__