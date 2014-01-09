all:
	gcc -g -Wall count_word.c -DCOUNT_WORD_TEST -o count_word_test
	gcc -g -Wall word_segmentation.c -DWORD_SEGMENTATION_TEST -o word_segmentation_test
	gcc -g -Wall parse_token.c -DPARSE_TOKEN_TEST -o parse_token_test
	gcc -g -Wall ini_parser.c -DINI_PARSER_TEST -o ini_parser_test

clean:
	rm -f *test
