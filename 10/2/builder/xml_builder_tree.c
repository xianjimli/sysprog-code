/*
 * File:    xml_builder_tree.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   xml builder tree implementation.
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
 * 2009-06-22 Li XianJing <xianjimli@hotmail.com> created.
 *
 */

#include "xml_builder_tree.h"

typedef struct _PrivInfo
{
	XmlNode* root;
	XmlNode* current;
}PrivInfo;

static void xml_builder_tree_on_start_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	XmlNode* new_node = NULL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	new_node = xml_node_create_normal(tag, attrs);
	xml_node_append_child(priv->current, new_node);
	priv->current = new_node;

	return;
}

static void xml_builder_tree_on_end_element(XmlBuilder* thiz, const char* tag)
{ 
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	priv->current = priv->current->parent;
	assert(priv->current != NULL);

	return;
}

static void xml_builder_tree_on_text(XmlBuilder* thiz, const char* text, size_t length)
{
	XmlNode* new_node = NULL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	new_node = xml_node_create_text(text);
	xml_node_append_child(priv->current, new_node);

	return;
}

static void xml_builder_tree_on_comment(XmlBuilder* thiz, const char* text, size_t length)
{
	XmlNode* new_node = NULL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	new_node = xml_node_create_comment(text);
	xml_node_append_child(priv->current, new_node);

	return;
}

static void xml_builder_tree_on_pi_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	XmlNode* new_node = NULL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	new_node = xml_node_create_pi(tag, attrs);
	xml_node_append_child(priv->current, new_node);

	return;
}

static void xml_builder_tree_on_error(XmlBuilder* thiz, int line, int row, const char* message)
{
	fprintf(stderr, "(%d,%d) %s\n", line, row, message);

	return;
}

static void xml_builder_tree_destroy(XmlBuilder* thiz)
{
	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;
		xml_node_destroy(priv->root);

		free(thiz);
	}

	return;
}

XmlBuilder* xml_builder_tree_create(void)
{
	XmlBuilder* thiz = (XmlBuilder*)calloc(1, sizeof(XmlBuilder));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->on_start_element  = xml_builder_tree_on_start_element;
		thiz->on_end_element    = xml_builder_tree_on_end_element;
		thiz->on_text           = xml_builder_tree_on_text;
		thiz->on_comment        = xml_builder_tree_on_comment;
		thiz->on_pi_element     = xml_builder_tree_on_pi_element;
		thiz->on_error          = xml_builder_tree_on_error;
		thiz->destroy           = xml_builder_tree_destroy;
		
		priv->root = xml_node_create_normal("__root__", NULL);
		priv->current = priv->root;
	}

	return thiz;
}

XmlNode*    xml_builder_get_tree(XmlBuilder* thiz)
{
	XmlNode* tree = NULL;

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;
		tree = priv->root->children;
		priv->root->children = NULL;
	}

	return tree;
}

#ifdef XML_BUILDER_TREE_TEST
#include "xml_builder_dump.h"

void dump_tree(XmlNode* tree)
{
	XmlBuilder* dump_builder = xml_builder_dump_create(stdout);
	xml_node_visit(tree, dump_builder);
	xml_builder_destroy(dump_builder);

	return;
}

int main(int argc, char* argv[])
{
	const char* pi_attrs[] = {"version", "1.0", "encoding", "utf-8", NULL};
	const char* root_attrs[] = {"name", "lixianjing", "blog", "http://www.limodev.cn/blog",NULL};

	XmlBuilder* thiz = xml_builder_tree_create();

	xml_builder_on_pi_element(thiz, "xml", pi_attrs);
	xml_builder_on_comment(thiz,"comment", 6);
	xml_builder_on_start_element(thiz, "programmer", root_attrs);
	xml_builder_on_text(thiz,"text", 4);
	xml_builder_on_end_element(thiz, "programmer");
	XmlNode* tree = xml_builder_get_tree(thiz);
	xml_builder_destroy(thiz);

	dump_tree(tree);
	xml_node_destroy(tree);

	return 0;
}
#endif/*XML_BUILDER_TREE_TEST*/

