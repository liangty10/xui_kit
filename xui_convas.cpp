#include "xui_bitmap.h"
#include "xui_family_bitmap.h"
#include "xui_family_member.h"
#include "xui_convas.h"

xui_implement_instance_member(xui_convas);
xui_implement_instance_method(xui_convas);

/*
//constructor
*/
xui_convas::xui_convas( void )
{
	m_viewport = xui_rect2d<s32>(0);
	m_cliprect = xui_rect2d<s32>(0);
}

/*
//cliprect
//viewport
*/
xui_method_explain(xui_convas, get_cliprect,		const xui_rect2d<s32>&	)( void ) const
{
	return m_cliprect;
}

xui_method_explain(xui_convas, set_cliprect,		void					)( const xui_rect2d<s32>&	rt )
{
	if (m_cliprect != rt)
	{
		m_cliprect  = rt;
		if (m_cliprect.get_w() == 0 || 
			m_cliprect.get_h() == 0)
		{
			glDisable(GL_SCISSOR_TEST);
		}
		else
		{
			glEnable (GL_SCISSOR_TEST);
			glScissor(
				m_cliprect.ax, 
				m_viewport.get_h()-m_cliprect.by, 
				m_cliprect.get_w(), 
				m_cliprect.get_h());
		}
	}
}

xui_method_explain(xui_convas, get_viewport,		const xui_rect2d<s32>&	)( void ) const
{
	return m_viewport;
}
xui_method_explain(xui_convas, set_viewport,		void					)( const xui_rect2d<s32> rt )
{
	if (m_viewport != rt)
	{
		m_viewport  = rt;
		glViewport(
			m_viewport.ax,
			m_viewport.ay,
			m_viewport.get_w(),
			m_viewport.get_h());
	}
}

/*
//image
*/
xui_method_explain(xui_convas, draw_image,			void					)( xui_bitmap*				image,
																			   const xui_vector<s32>&	pt,
																			   const xui_colour&		color )
{
	draw_image(
		image,
		xui_rect2d<s32>(pt, image->get_size()),
		color);
}

xui_method_explain(xui_convas, draw_image,			void					)( xui_bitmap*				image,
																			   const xui_rect2d<s32>&	dst,
																			   const xui_colour&		color )
{
	draw_image(
		image,
		xui_rect2d<s32>(xui_vector<s32>(0), image->get_size()),
		dst,
		color);
}

xui_method_explain(xui_convas, draw_image,			void					)( xui_bitmap*				image,
																			   const xui_rect2d<s32>&	src,
																			   const xui_rect2d<s32>&	dst,
																			   const xui_colour&		color )
{
	image->set_bind();

	glDisable(GL_POLYGON_SMOOTH);
	glBegin(GL_QUADS);
	glColor4fv(color.value);

	f32 invw = 1.0f / (f32)image->get_size().w;
	f32 invh = 1.0f / (f32)image->get_size().h;

	glTexCoord2f((f32)src.ax*invw, (f32)src.ay*invh); glVertex2f((f32)dst.ax, (f32)dst.ay);	
	glTexCoord2f((f32)src.ax*invw, (f32)src.by*invh); glVertex2f((f32)dst.ax, (f32)dst.by); 
	glTexCoord2f((f32)src.bx*invw, (f32)src.by*invh); glVertex2f((f32)dst.bx, (f32)dst.by); 
	glTexCoord2f((f32)src.bx*invw, (f32)src.ay*invh); glVertex2f((f32)dst.bx, (f32)dst.ay); 

	glEnd();

	image->non_bind();
}

/*
//text
*/
xui_method_explain(xui_convas, calc_size,			xui_vector<s32>			)( const std::wstring&		text,
																			   const xui_family&		textfont,
																		       s32						maxwidth,
																			   bool						singleline )
{
	xui_vector<s32> size;
	size.w = 0;
	size.h = textfont.size + textfont.vert;

	if (singleline)
	{
		for (u32 i = 0; i < text.length(); ++i)
		{
			//非可见字符
			if (text[i] < 0x20)
				continue;

			xui_family_member* member = get_family_member(textfont, text[i]);
			size.w += member->rt.get_sz().w;
			size.w += textfont.horz;
		}
	}
	else
	{
		//绘制多行
		s32 textline = 0;
		s32 curwidth = 0;

		//single line buffer
		std::wstring buffer;

		wchar_t*p = (wchar_t*)text.c_str();
		while (*p)
		{
			if (*p == L'\n')
			{
				buffer.clear();

				curwidth = 0;
				++textline;
				++p;
			}
			else
			{
				std::wstring word;
				s32 sw  = calc_word(std::wstring(p), textfont, maxwidth, word);
				if (sw == 0 && word.empty())
					break;

				if (sw+curwidth > maxwidth)
				{
					//宽度太小
					if (curwidth == 0)
						break;

					buffer.clear();
					curwidth = 0;
					++textline;
				}
				else
				{
					curwidth += sw;
					buffer	 += word;
					p		 += word.length();
				}
			}
		}

		if (!buffer.empty())
			++textline;

		size.w  = textline > 1 ? maxwidth : curwidth;
		size.h *= textline;
	}

	return size;
}

xui_method_explain(xui_convas, calc_draw,			xui_rect2d<s32>			)( const std::wstring&		text,
																			   const xui_family&		textfont,
																			   const xui_rect2d<s32>&	calcrect,
																			   u08						textalign,
																			   bool						singleline )
{
	xui_rect2d<s32> drawrect = calcrect;
	xui_vector<s32> size     = calc_size(text, textfont, calcrect.get_w(), singleline);
	drawrect.set_sz(size);

	//水平方向对齐
	switch (textalign)
	{
	case TA_CT:	//CT
	case TA_CC:	//CC
	case TA_CB:	//CB
		drawrect.oft_x((calcrect.get_sz().w - size.w) / 2);
		break;
	case TA_RT:	//RT
	case TA_RC:	//RC
	case TA_RB:	//RB
		drawrect.oft_x((calcrect.get_sz().w - size.w));
		break;
	}

	//竖直方向对齐
	switch (textalign)
	{
	case TA_LC:	//LC
	case TA_CC:	//CC
	case TA_RC:	//RC
		drawrect.oft_y((calcrect.get_sz().h - size.h) / 2);
		break;
	case TA_LB:	//LB
	case TA_CB:	//CB
	case TA_RB:	//RB
		drawrect.oft_y((calcrect.get_sz().h - size.h));
		break;
	}

	return drawrect;
}

xui_method_explain(xui_convas, calc_text,			std::wstring			)( const std::wstring&		text,
																			   const xui_family&		textfont,
																			   s32						maxwidth )
{
	//绘制多行
	std::wstring temp;

	s32 curwidth = 0;
	wchar_t*p = (wchar_t*)text.c_str();
	while (*p)
	{
		if (*p == L'\n')
		{
			temp += *p;
			break;
		}
		else
		{
			std::wstring word;
			s32 sw =  calc_word(std::wstring(p), textfont, maxwidth, word);
			if (sw == 0 && word.empty())
				break;

			if (sw+curwidth > maxwidth)
			{
				break;
			}
			else
			{
				curwidth += sw;
				temp     += word;
				p	     += word.length();
			}
		}
	}

	return temp;
}

xui_method_explain(xui_convas, calc_char,			u32						)( const std::wstring&		text,
																			   const xui_family&		textfont,
																			   s32						position )
{
	if (position < 0)	
		return 0;

	s32 currx  = 0;
	u32 index  = 0;
	for (u32 i = 0; i < text.length(); ++i)
	{			
		if (text[i] == L'\n')
			continue;

		xui_family_member* member = get_family_member(textfont, text[i]);
		if (position < currx + member->rt.get_w()/2)
			break;

		currx += member->rt.get_w();
		currx += textfont.horz;
		++index;
	}

	return index;
}

xui_method_explain(xui_convas, calc_word,			u32						)( const std::wstring&		text,
																			   const xui_family&		textfont,
																			   s32						maxwidth,
																			   std::wstring&			word )
{
	word.clear();

	s32 curwidth = 0;
	for (u32 i = 0; i < text.length(); ++i)
	{
		//非可见字符
		if (text[i] < 0x20)
		{
			if (word.length())
				break;
				
			word += text[i];
			continue;
		}

		xui_family_member* member = get_family_member(textfont, text[i]);

		//单词字符
		if ((text[i] >= L'a' && text[i] <= L'z') ||
			(text[i] >= L'A' && text[i] <= L'Z'))
		{
			if (curwidth + member->rt.get_w() + textfont.horz > maxwidth)
				break;

			curwidth += member->rt.get_w();
			curwidth += textfont.horz;
			word     += text[i];
		}
		//空格中文
		else
		{
			if (word.length())
				break;

			if (curwidth + member->rt.get_sz().w + textfont.horz > maxwidth)
				break;

			curwidth += member->rt.get_sz().w;
			curwidth += textfont.horz;
			word     += text[i];
			break;
		}
	}

	return curwidth;
}

xui_method_explain(xui_convas, draw_text,			void					)( const std::wstring&		text,
																			   const xui_family&		textfont,
																			   const xui_rect2d<s32>&	drawrect,
																			   const xui_family_render&	textdraw )
{
	xui_vector<s32> pt = drawrect.get_pt();

	//绘制多行
	s32 curwidth = 0;
	s32 maxwidth = drawrect.get_w();
	std::wstring buffer;

	wchar_t*p = (wchar_t*)text.c_str();
	while (*p)
	{
		if(*p == L'\n')
		{
			draw_text(buffer, textfont, pt, textdraw);
			pt.y += textfont.size;
			pt.y += textfont.vert;
			buffer.clear();

			curwidth = 0;
			++p;
		}
		else
		{
			std::wstring word;
			s32 sw =  calc_word(std::wstring(p), textfont, maxwidth, word);
			if (sw == 0 && word.empty())
				break;

			if (sw+curwidth > maxwidth)
			{
				//宽度太小
				if (curwidth == 0)
					break;

				draw_text(buffer, textfont, pt, textdraw);
				pt.y += textfont.size;
				pt.y += textfont.vert;
				buffer.clear();

				curwidth  = 0;
			}
			else
			{
				curwidth += sw;
				buffer	 += word;
				p		 += word.length();
			}
		}
	}

	if (!buffer.empty())
		draw_text(buffer, textfont, pt, textdraw);
}

xui_method_explain(xui_convas, draw_text,			void					)( const std::wstring&		text,
																			   const xui_family&		textfont,
																			   const xui_vector<s32>&	screenpt,
																			   const xui_family_render&	textdraw )
{
	xui_rect2d<s32> dst;
	dst.set_pt(screenpt);

	for (u32 i = 0; i < text.length(); ++i)
	{
		//非可见字符
		if (text[i] < 0x20)
			continue;

		xui_family_member* member = get_family_member(textfont, text[i]);
		dst.set_sz(member->rt.get_sz());

		if (textdraw.renderstyle == TEXTDRAW_STROKE)
		{
			xui_vector<s32> offset;
			offset = xui_vector<s32>(-textdraw.strokewidth,   -textdraw.strokewidth);
			draw_image(member->bitmap, member->rt, dst+offset, textdraw.strokecolor);
			offset = xui_vector<s32>( 0,                      -textdraw.strokewidth);
			draw_image(member->bitmap, member->rt, dst+offset, textdraw.strokecolor);
			offset = xui_vector<s32>( textdraw.strokewidth,   -textdraw.strokewidth);
			draw_image(member->bitmap, member->rt, dst+offset, textdraw.strokecolor);
			offset = xui_vector<s32>(-textdraw.strokewidth,    0);
			draw_image(member->bitmap, member->rt, dst+offset, textdraw.strokecolor);
			offset = xui_vector<s32>( textdraw.strokewidth,    0);
			draw_image(member->bitmap, member->rt, dst+offset, textdraw.strokecolor);
			offset = xui_vector<s32>(-textdraw.strokewidth,    textdraw.strokewidth);
			draw_image(member->bitmap, member->rt, dst+offset, textdraw.strokecolor);
			offset = xui_vector<s32>( 0,                       textdraw.strokewidth);
			draw_image(member->bitmap, member->rt, dst+offset, textdraw.strokecolor);
			offset = xui_vector<s32>( textdraw.strokewidth,    textdraw.strokewidth);
			draw_image(member->bitmap, member->rt, dst+offset, textdraw.strokecolor);
		}
		if (textdraw.renderstyle == TEXTDRAW_SHADOW)
		{
			xui_vector<s32> offset;
			offset = xui_vector<s32>(1, 0);
			draw_image(member->bitmap, member->rt, dst+offset, xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
			offset = xui_vector<s32>(0, 1);
			draw_image(member->bitmap, member->rt, dst+offset, xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
			offset = xui_vector<s32>(1, 1);
			draw_image(member->bitmap, member->rt, dst+offset, xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
		}
		{
			draw_image(member->bitmap, member->rt, dst,        textdraw.normalcolor);
		}

		dst.oft_x(member->rt.get_w()+textfont.horz);
	}
}

/*
//line
*/
xui_method_explain(xui_convas, draw_line,			void					)( const xui_vector<s32>&	p1, 
																			   const xui_vector<s32>&	p2,
																			   const xui_colour&		color )
{
	glDisable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);
	glColor4fv(color.value);

	glVertex2f((f32)p1.x+0.375f, (f32)p1.y+0.375f);
	glVertex2f((f32)p2.x+0.375f, (f32)p2.y+0.375f);

	glEnd();
}

/*
//path
*/
xui_method_explain(xui_convas, draw_path,			void					)( xui_vector<s32>*			pt,
																			   u32						count,
																			   const xui_colour&		color )
{
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_STRIP);
	glColor4fv(color.value);

	for (u32 i = 0; i < count; ++i)
		glVertex2f((f32)pt[i].x+0.375f, (f32)pt[i].y+0.375f);

	glEnd();
}

xui_method_explain(xui_convas, fill_poly,			void					)( xui_vector<s32>*			pt,
																			   u32						count,
																			   const xui_colour&		color )
{
	glEnable(GL_POLYGON_SMOOTH);
	glBegin(GL_TRIANGLES);
	glColor4fv(color.value);

	for (u32 i = 0; i < count; ++i)
		glVertex2f((f32)pt[i].x, (f32)pt[i].y);

	glEnd();
}

/*
//rectangle
*/
xui_method_explain(xui_convas, draw_rectangle,		void					)( const xui_rect2d<s32>&	rt,
																			   const xui_colour&		color,
																			   s32						thick )
{
	draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.ay),	color);
	draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.ax, rt.by),	color);
	draw_line(xui_vector<s32>(rt.ax, rt.by), xui_vector<s32>(rt.bx, rt.by),	color);
	draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by),	color);
	if (thick > 1)
	{
		for (s32 i = 1; i < thick; ++i)
		{
			xui_rect2d<s32> temp;
			temp.ax = rt.ax+i;
			temp.ay = rt.ay+i;
			temp.bx = rt.bx-i;
			temp.by = rt.by-i;
			draw_rectangle(temp, color);
		}
	}
}

xui_method_explain(xui_convas, fill_rectangle,		void					)( const xui_rect2d<s32>&	rt,
																			   const xui_colour&		color )
{
	glDisable(GL_POLYGON_SMOOTH);
	glBegin(GL_QUADS);
	glColor4fv(color.value);

	glVertex2f((f32)rt.ax, (f32)rt.ay);
	glVertex2f((f32)rt.ax, (f32)rt.by);
	glVertex2f((f32)rt.bx, (f32)rt.by);
	glVertex2f((f32)rt.bx, (f32)rt.ay);

	glEnd();
}

/*
//round
*/
xui_method_explain(xui_convas, draw_round,			void					)( const xui_rect2d<s32>&	rt,
																			   const xui_colour&		color,
																			   s32						corner )
{
	draw_round(
		rt, 
		color, 
		xui_rect2d<s32>(corner));
}

xui_method_explain(xui_convas, draw_round,			void					)( const xui_rect2d<s32>&	rt,
																			   const xui_colour&		color,
																			   const xui_rect2d<s32>&   corner )
{
	//左上/右上/右下/左下
	s32 corner0 = corner.value[0];
	s32 corner1 = corner.value[1];
	s32 corner2 = corner.value[2];
	s32 corner3 = corner.value[3];
	if (corner0) 
		draw_arc(xui_rect2d<s32>(
			rt.ax,			 
			rt.ay,
			rt.ax+2*corner0, 
			rt.ay+2*corner0), color, 180, 90, 1);
	if (corner1) 
		draw_arc(xui_rect2d<s32>(
			rt.bx-2*corner1, 
			rt.ay,			
			rt.bx,		   
			rt.ay+2*corner1), color, 270, 90, 1);
	if (corner2) 
		draw_arc(xui_rect2d<s32>(
			rt.bx-2*corner2, 
			rt.by-2*corner2, 
			rt.bx,		   
			rt.by),			  color,   0, 90, 1);
	if (corner3) 
		draw_arc(xui_rect2d<s32>(
			rt.ax,			 
			rt.by-2*corner3, 
			rt.ax+2*corner3, 
			rt.by),			  color,  90, 90, 1);

	draw_line(xui_vector<s32>(rt.ax+corner0, rt.ay),		 xui_vector<s32>(rt.bx-corner1, rt.ay),			color);
	draw_line(xui_vector<s32>(rt.bx,		 rt.ay+corner1), xui_vector<s32>(rt.bx,		    rt.by-corner2),	color);
	draw_line(xui_vector<s32>(rt.bx-corner2, rt.by),		 xui_vector<s32>(rt.ax+corner3, rt.by),			color);
	draw_line(xui_vector<s32>(rt.ax,		 rt.by-corner3), xui_vector<s32>(rt.ax,		    rt.ay+corner0),	color);
}

xui_method_explain(xui_convas, fill_round,			void					)( const xui_rect2d<s32>&	rt,
																			   const xui_colour&		color,
																			   s32						corner )
{
	fill_round(
		rt,
		color,
		xui_rect2d<s32>(corner));
}

xui_method_explain(xui_convas, fill_round,			void					)( const xui_rect2d<s32>&	rt,
																			   const xui_colour&		color,
																			   const xui_rect2d<s32>&   corner )
{
	//左上/右上/右下/左下
	s32 corner0 = corner.value[0];
	s32 corner1 = corner.value[1];
	s32 corner2 = corner.value[2];
	s32 corner3 = corner.value[3];
	if (corner0) 
		fill_arc(xui_rect2d<s32>(
			rt.ax,
			rt.ay,
			rt.ax+2*corner0, 
			rt.ay+2*corner0), color, 180, 90, 1);
	if (corner1) 
		fill_arc(xui_rect2d<s32>(
			rt.bx-2*corner1,	
			rt.ay,
			rt.bx,
			rt.ay+2*corner1), color, 270, 90, 1);
	if (corner2) 
		fill_arc(xui_rect2d<s32>(
			rt.bx-2*corner2,	
			rt.by-2*corner2, 
			rt.bx,
			rt.by),			  color,   0, 90, 1);
	if (corner3) 
		fill_arc(xui_rect2d<s32>(
			rt.ax,
			rt.by-2*corner3, 
			rt.ax+2*corner3, 
			rt.by),			  color,  90, 90, 1);

	//上边
	fill_rectangle(xui_rect2d<s32>(
		rt.ax + corner0,	
		rt.ay,		 
		rt.bx - corner1, 
		rt.ay +(corner0|corner1)),	
		color);
	//右边
	fill_rectangle(xui_rect2d<s32>(
		rt.bx -(corner1|corner2),	
		rt.ay + corner1, 
		rt.bx,		  
		rt.by - corner2),	
		color);
	//下边
	fill_rectangle(xui_rect2d<s32>(
		rt.ax + corner3,	
		rt.by -(corner2|corner3), 
		rt.bx - corner2, 
		rt.by),			
		color);
	//左边
	fill_rectangle(xui_rect2d<s32>(
		rt.ax,			
		rt.ay + corner0, 
		rt.ax +(corner3|corner0), 
		rt.by - corner3),	
		color);
	//中间
	fill_rectangle(xui_rect2d<s32>(
		rt.ax +(corner3|corner0),	
		rt.ay +(corner0|corner1), 
		rt.bx -(corner1|corner2), 
		rt.by -(corner2|corner3)),	
		color);
}

/*
//circle
*/
xui_method_explain(xui_convas, draw_circle,			void					)( const xui_vector<s32>&	center,
																			   s32						radius,
																			   const xui_colour&		color,
																			   s32						start,
																			   s32						sweep )
{
	xui_rect2d<s32> rt;
	rt.ax = center.x - radius;
	rt.bx = center.x + radius;
	rt.ay = center.y - radius;
	rt.by = center.y + radius;
	draw_arc(rt, color, start, sweep, 1);
}

xui_method_explain(xui_convas, fill_circle,			void					)( const xui_vector<s32>&	center, 
																			   s32						radius,
																			   const xui_colour&		color, 
																			   s32						start, 
																			   s32						sweep )
{
	xui_rect2d<s32> rt;
	rt.ax = center.x - radius;
	rt.bx = center.x + radius;
	rt.ay = center.y - radius;
	rt.by = center.y + radius;
	fill_arc(rt, color, start, sweep, 1);
}

/*
//arc
*/
xui_method_explain(xui_convas, draw_arc,			void					)( const xui_rect2d<s32>&	rt,
																			   const xui_colour&		color,
																			   s32						start,
																			   s32						sweep,
																			   s32						precision )
{
	glDisable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_STRIP);
	glColor4fv(color.value);

	f32 halfw = rt.get_w() / 2.0f;
	f32 halfh = rt.get_h() / 2.0f;
	f32 lastx = -1.0f;
	f32 lasty = -1.0f;
	s32 count = sweep / precision;
	for (s32 i = 0; i <= count; ++i)
	{
		s32 d = start + precision *  i;
		f32 x = cos(d*xui_pi/180.0f) * halfw + halfw + (f32)rt.ax;
		f32 y = sin(d*xui_pi/180.0f) * halfh + halfh + (f32)rt.ay;
		if (xui_pixel_align(x) == lastx && 
			xui_pixel_align(y) == lasty)
			continue;

		glVertex2f(xui_pixel_align(x)+0.375f, xui_pixel_align(y)+0.375f);
		lastx = xui_pixel_align(x);
		lasty = xui_pixel_align(y);
	}

	glEnd();
}

xui_method_explain(xui_convas, fill_arc,			void					)( const xui_rect2d<s32>&	rt,
																			   const xui_colour&		color,
																			   s32						start,
																			   s32						sweep,
																			   s32						precision )
{
	glDisable(GL_POLYGON_SMOOTH);
	glBegin(GL_TRIANGLE_FAN);
	glColor4fv(color.value);

	f32 halfw = rt.get_w() / 2.0f;
	f32 halfh = rt.get_h() / 2.0f;
	f32 lastx = -1.0f;
	f32 lasty = -1.0f;
	s32 count = sweep / precision;

	glVertex2f(halfw + (f32)rt.ax, halfh + (f32)rt.ay);
	for (s32 i = 0; i <= count; ++i)
	{
		s32 d = start + precision *  i;
		f32 x = cos(d*xui_pi/180.0f) * halfw + halfw + (f32)rt.ax;
		f32 y = sin(d*xui_pi/180.0f) * halfh + halfh + (f32)rt.ay;
		if (xui_pixel_align(x) == lastx && 
			xui_pixel_align(y) == lasty)
			continue;

		glVertex2f(xui_pixel_align(x), xui_pixel_align(y));
		lastx = xui_pixel_align(x);
		lasty = xui_pixel_align(y);
	}

	glEnd();
}

xui_method_explain(xui_convas, get_family_bitmap,	xui_family_bitmap*		)( const xui_family&		family )
{
	return (xui_family_bitmap*)xui_bitmap::create(family);
}

xui_method_explain(xui_convas, get_family_member,	xui_family_member*		)( const xui_family&		family, u16 wc )
{
	return get_family_bitmap(family)->get_member(wc);
}