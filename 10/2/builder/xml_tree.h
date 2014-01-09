/*
 * File:    xml_tree.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   xml tree
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

#include "typedef.h"
#include "xml_builder.h"

#ifndef XML_TREE_H
#define XML_TREE_H

typedef enum _XmlNodeType
{
	XML_NODE_NONE,
	XML_NODE_PI,
	XML_NODE_TEXT,
	XML_NODE_NORMAL,
	XML_NODE_COMMENT
}XmlNodeType;

typedef struct _XmlAttr
{
	char* name;
	char* value;

	struct _XmlAttr* next;
}XmlAttr;

typedef struct _XmlNodePi
{
	char* name;	
	XmlAttr* first_attr;
}XmlNodePi;

typedef struct _XmlNodeNormal
{
	char* name;
	XmlAttr* first_attr;
}XmlNodeNormal;

typedef struct _XmlNode
{
	XmlNodeType type;
	union
	{
		char* text;
		char* comment;
		XmlNodePi pi;
		XmlNodeNormal normal;
	}u;
	struct _XmlNode* parent;
	struct _XmlNode* children;
	struct _XmlNode* sibling;
}XmlNode;

typedef Ret (*XmlNodeVisitFunc)(void* ctx, int level, XmlNode* node);

XmlNode* xml_node_create_text(const char* text);
XmlNode* xml_node_create_comment(const char* comment);
XmlNode* xml_node_create_pi(const char* name, const char** attrs);
XmlNode* xml_node_create_normal(const char* name, const char** attrs); 
XmlNode* xml_node_append_sibling(XmlNode* node, XmlNode* sibling);
XmlNode* xml_node_append_child(XmlNode* node, XmlNode* child);

void xml_node_visit(XmlNode* node, XmlBuilder* builder);
void xml_node_print(XmlNode* node);

void xml_node_destroy(XmlNode* node);

#endif/*XML_TREE_H*/

