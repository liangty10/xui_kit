#ifndef __onity_preview_h__
#define __onity_preview_h__

#include "xui_control.h"
#include "xui_action_ctrl.h"

class NPNode;
class onity_renderview;
class onity_preview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_preview( void );

	/*
	//destructor
	*/
	virtual ~onity_preview( void );

	/*
	//method
	*/
	NPNode*					get_drawnode		( void );
	onity_renderview*		get_drawview		( void );
	void					set_toolshow		( bool flag );
	void					set_needshow		( void );
	void					set_viewprop		( xui_proproot* prop, bool play = true );
	void					set_drawrect		( const xui_rect2d<s32>& rect );

protected:
	/*
	//callback
	*/
	virtual void			on_updateself		( xui_method_update& args );

	/*
	//event
	*/
	void					on_buttonclick		( xui_component* sender, xui_method_args&	args );
	void					on_speedscroll		( xui_component* sender, xui_method_args&	args );
	void					on_headperform		( xui_component* sender, xui_method_args&	args );
	void					on_viewupdateself	( xui_component* sender, xui_method_update& args );
	void					on_viewrenderself	( xui_component* sender, xui_method_args&	args );
	void					on_viewmousewheel	( xui_component* sender, xui_method_mouse&	args );
	void					on_ctrltick			( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	f32						get_speed			( void );
	void					set_speed			( f32 speed );
	void					set_localtransform	( void );
	f32						get_incscale		( void );
	f32						get_decscale		( void );

	/*
	//member
	*/
	xui_panel*				m_head;
	xui_panel*				m_tool;
	onity_renderview*		m_view;
	xui_button*				m_play;
	xui_drawer*				m_text;
	xui_slider*				m_speed;
	xui_button*				m_large;
	xui_button*				m_small;
	xui_button*				m_reset;
	f32						m_scale;
	NPNode*					m_node;
	xui_rect2d<s32>			m_rect;
	xui_action_ctrl*		m_ctrl;
};

#endif//__onity_preview_h__