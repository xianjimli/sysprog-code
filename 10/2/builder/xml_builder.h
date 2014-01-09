/*
 * File:    xml_builder.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   xml builder interface.
 *
 * Copyright (c) Li XianJing
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2009-05-30 Li XianJing <xianjimli@hotmail.com> created.
 *
 */

#include "typedef.h"

#ifndef XML_BUILDER_H
#define XML_BUILDER_H

DECLS_BEGIN

struct _XmlBuilder;
typedef struct _XmlBuilder XmlBuilder;

typedef void (*XmlBuilderOnStartElementFunc)(XmlBuilder* thiz, const char* tag, const char** attrs);
typedef void (*XmlBuilderOnEndElementFunc)(XmlBuilder* thiz, const char* tag);
typedef void (*XmlBuilderOnTextFunc)(XmlBuilder* thiz, const char* text, size_t length);
typedef void (*XmlBuilderOnCommentFunc)(XmlBuilder* thiz, const char* text, size_t length);
typedef void (*XmlBuilderOnPiElementFunc)(XmlBuilder* thiz, const char* tag, const char** attrs);
typedef void (*XmlBuilderOnErrorFunc)(XmlBuilder* thiz, int line, int row, const char* message);
typedef void (*XmlBuilderDestroyFunc)(XmlBuilder* thiz);

struct _XmlBuilder
{
	XmlBuilderOnStartElementFunc on_start_element;
	XmlBuilderOnEndElementFunc   on_end_element;
	XmlBuilderOnTextFunc         on_text;
	XmlBuilderOnCommentFunc      on_comment;
	XmlBuilderOnPiElementFunc    on_pi_element;
	XmlBuilderOnErrorFunc        on_error;
	XmlBuilderDestroyFunc        destroy;

	char priv[1];
};

static inline void xml_builder_on_start_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	return_if_fail(thiz != NULL && thiz->on_start_element != NULL);

	thiz->on_start_element(thiz, tag, attrs);

	return;
}

static inline void xml_builder_on_end_element(XmlBuilder* thiz, const char* tag)
{
	return_if_fail(thiz != NULL && thiz->on_end_element != NULL);

	thiz->on_end_element(thiz, tag);

	return;
}

static inline void xml_builder_on_text(XmlBuilder* thiz, const char* text, size_t length)
{
	return_if_fail(thiz != NULL && thiz->on_text != NULL);

	thiz->on_text(thiz, text, length);

	return;
}

static inline void xml_builder_on_comment(XmlBuilder* thiz, const char* text, size_t length)
{
	return_if_fail(thiz != NULL);

	if(thiz->on_comment != NULL)
	{
		thiz->on_comment(thiz, text, length);
	}

	return;
}

static inline void xml_builder_on_pi_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	return_if_fail(thiz != NULL);
	
	if(thiz->on_pi_element != NULL)
	{
		thiz->on_pi_element(thiz, tag, attrs);
	}

	return;
}

static inline void xml_builder_on_error(XmlBuilder* thiz, int line, int row, const char* message)
{
	return_if_fail(thiz != NULL);

	if(thiz->on_error != NULL)
	{
		thiz->on_error(thiz, line, row, message);
	}

	return;
}

static inline void xml_builder_destroy(XmlBuilder* thiz)
{
	if(thiz != NULL && thiz->destroy != NULL)
	{
		thiz->destroy(thiz);
	}

	return;
}

DECLS_END

#endif/*XML_BUILDER_H*/

