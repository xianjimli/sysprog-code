/*
 * File:    xml_builder_tree.h
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

#ifndef XML_BUILDER_TREE_H
#define XML_BUILDER_TREE_H
#include "xml_builder.h"
#include "xml_tree.h"

DECLS_BEGIN

XmlBuilder* xml_builder_tree_create(void);
XmlNode*    xml_builder_get_tree(XmlBuilder* thiz);

DECLS_END

#endif/*XML_BUILDER_TREE_H*/

