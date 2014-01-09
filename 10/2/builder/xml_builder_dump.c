/*
 * File:    xml_builder_dump.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   xml builder dump implementation.
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

#include "xml_builder_dump.h"

typedef struct _PrivInfo
{
	FILE* fp;
}PrivInfo;

static void xml_builder_dump_on_start_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	int i = 0;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	fprintf(priv->fp, "<%s", tag);

	for(i = 0; attrs != NULL && attrs[i] != NULL && attrs[i + 1] != NULL; i += 2)
	{
		fprintf(priv->fp, " %s=\"%s\"", attrs[i], attrs[i + 1]);
	}
	fprintf(priv->fp, ">");

	return;
}

static void xml_builder_dump_on_end_element(XmlBuilder* thiz, const char* tag)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	fprintf(priv->fp, "</%s>\n", tag);

	return;
}

static void xml_builder_dump_on_text(XmlBuilder* thiz, const char* text, size_t length)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	fwrite(text, length, 1, priv->fp);

	return;
}

static void xml_builder_dump_on_comment(XmlBuilder* thiz, const char* text, size_t length)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	fprintf(priv->fp, "<!--");
	fwrite(text, length, 1, priv->fp);
	fprintf(priv->fp, "-->\n");

	return;
}

static void xml_builder_dump_on_pi_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	int i = 0;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	fprintf(priv->fp, "<?%s", tag);

	for(i = 0; attrs != NULL && attrs[i] != NULL && attrs[i + 1] != NULL; i += 2)
	{
		fprintf(priv->fp, " %s=\"%s\"", attrs[i], attrs[i + 1]);
	}
	fprintf(priv->fp, "?>\n");

	return;
}

static void xml_builder_dump_on_error(XmlBuilder* thiz, int line, int row, const char* message)
{
	fprintf(stderr, "(%d,%d) %s\n", line, row, message);

	return;
}

static void xml_builder_dump_destroy(XmlBuilder* thiz)
{
	if(thiz != NULL)
	{
		free(thiz);
	}

	return;
}

XmlBuilder* xml_builder_dump_create(FILE* fp)
{
	XmlBuilder* thiz = (XmlBuilder*)calloc(1, sizeof(XmlBuilder));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->on_start_element  = xml_builder_dump_on_start_element;
		thiz->on_end_element    = xml_builder_dump_on_end_element;
		thiz->on_text           = xml_builder_dump_on_text;
		thiz->on_comment        = xml_builder_dump_on_comment;
		thiz->on_pi_element     = xml_builder_dump_on_pi_element;
		thiz->on_error          = xml_builder_dump_on_error;
		thiz->destroy           = xml_builder_dump_destroy;

		priv->fp = fp != NULL ? fp : stdout;
	}

	return thiz;
}

#ifdef XML_BUILDER_DUMP_TEST
int main(int argc, char* argv[])
{
	const char* pi_attrs[] = {"version", "1.0", "encoding", "utf-8", NULL};
	const char* root_attrs[] = {"name", "lixianjing", "blog", "http://www.limodev.cn/blog",NULL};

	XmlBuilder* thiz = xml_builder_dump_create(stdout);

	xml_builder_on_pi_element(thiz, "xml", pi_attrs);
	xml_builder_on_comment(thiz,"comment", 6);
	xml_builder_on_start_element(thiz, "programmer", root_attrs);
	xml_builder_on_text(thiz,"text", 4);
	xml_builder_on_end_element(thiz, "programmer");

	xml_builder_destroy(thiz);

	return 0;
}
#endif/*XML_BUILDER_DUMP_TEST*/

