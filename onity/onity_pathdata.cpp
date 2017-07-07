#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_treenode.h"
#include "xui_proproot.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "onity_project.h"
#include "onity_mainform.h"
#include "onity_resource.h"
#include "onity_proppath.h"
#include "onity_pathdata.h"

/*
//constructor
*/
xui_create_explain(onity_pathdata)( xui_bitmap* icon, const std::wstring& full, xui_proproot* prop )
: onity_filedata(icon, full, prop)
{}

/*
//virtual
*/
xui_method_explain(onity_pathdata, ntf_rename,		void					)( const std::wstring& last, const std::wstring& curr )
{
	onity_filedata::ntf_rename(last, curr);

	onity_proppath* proppath = dynamic_cast<onity_proppath*>(m_prop);
	const xui_proproot_vec& vec = proppath->get_fileprop();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_propfile* propfile = dynamic_cast<onity_propfile*>(vec[i]);
		if (propfile)
			propfile->ntf_rename(last, curr);
	}
}

/*
//override
*/
xui_method_explain(onity_pathdata, get_text,		std::wstring			)( u32 index )
{
	if (m_text.length() == 0)
	{
		std::wstring full = xui_global::get_workpath();
		if (full.back() == L'/' ||
			full.back() == L'\\')
			full.erase(full.length()-1);

		return get_file(full);
	}

	return get_file();
}
xui_method_explain(onity_pathdata, set_text,		void					)( u32 index, const std::wstring& text )
{
	if (m_text.length() == 0)
		return;

	onity_project*	project = onity_mainform::get_ptr()->get_project();
	onity_proppath* prop	= dynamic_cast<onity_proppath*>(m_prop);
	onity_treedata* data	= (onity_treedata*)prop->get_linkdata(project->get_pathview());
	if (data)
	{
		std::wstring work = xui_global::get_workpath();
		std::wstring last = m_text;
		std::wstring curr = get_path()+text;

		xui_global::set_fwatchclose();
		if (xui_global::rna_file(last, curr))
		{
			std::vector<xui_treenode*> nodevec;
			data->get_node()->get_leafnodetotal(nodevec);
			for (u32 i = 0; i < nodevec.size(); ++i)
			{
				onity_pathdata* pathdata = (onity_pathdata*)nodevec[i]->get_linkdata();
				pathdata->ntf_rename(last, curr);
			}

			project->ntf_rename(last, curr);
		}
		xui_global::set_fwatchstart(work);
	}
}
