#ifndef __onity_renderdata_h__
#define __onity_renderdata_h__

#include "onity_treedata.h"

class onity_renderdata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_renderdata( xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_renderdata( void );

	/*
	//override
	*/
	virtual std::wstring		get_text			( u32 index );
	virtual bool				get_flag			( u32 index );
	virtual void				set_flag			( u32 index, bool flag );
};

#endif//__onity_renderdata_h__