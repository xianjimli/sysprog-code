#include "xml_parser.h"
#include "xml_builder_dump.h"
#include "xml_builder_tree.h"

#define XML "<?xml version=\"1.0\" encoding=\"utf-8\"?> \
<!--comment--> <br/> <p>ppp</p> <br />\
<programmer name=\"lixianjing\" blog=\"http://www.limodev.cn/blog\">text</programmer>"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "xml_builder_dump.h"

char* read_file(const char* file_name)
{
	char* buffer = NULL;
	FILE* fp = fopen(file_name, "r");

	if(fp != NULL)
	{
		struct stat st = {0};
		if(stat(file_name, &st) == 0)
		{
			buffer = malloc(st.st_size + 1);
			fread(buffer, st.st_size, 1, fp);
			buffer[st.st_size] = '\0';
		}
	}

	return buffer;
}

void dump_tree(XmlNode* tree)
{
	XmlBuilder* dump_builder = xml_builder_dump_create(stdout);
	xml_node_visit(tree, dump_builder);
	xml_builder_destroy(dump_builder);

	return;
}

int main(int argc, char* argv[])
{
	XmlParser* thiz = xml_parser_create();
	XmlBuilder* builder = xml_builder_tree_create();

	xml_parser_set_builder(thiz, builder);

	if(argc > 1)
	{
		char* buffer = read_file(argv[1]);
		xml_parser_parse(thiz, buffer);
		free(buffer);
	}

	XmlNode* tree = xml_builder_get_tree(builder);
	dump_tree(tree);
	xml_node_destroy(tree);

	xml_builder_destroy(builder);
	xml_parser_destroy(thiz);

	return 0;
}

