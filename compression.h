#ifndef COMPRESSION
#define COMPRESSION

#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

namespace pattern {
	using ic = std::pair < int, char >; 	
	
	int default_encoding ( char c ){
		return c - 'a';
	}

	char default_decoding ( int z ){
		return 'a' + z;
	}

	std::vector < ic > lzw_encode ( 
		const std::string &input,
		const std::function < int (char) > &encoding = default_encoding,	
		const std::function < char (int) > &decoding = default_decoding,
		int alpha_num = 26
	)
	{
		/*
		std::cout << input << '\n';
		for(auto e:input){
				std::cout << encoding(e);
		}puts("");
		*/
		std::vector < ic > compressed_vec;

		//[1] input single alphabets in the trie
		const size_t root = 0;
		std::vector < std::vector < int > > trie;
		trie.push_back(std::vector < int > (alpha_num));
		for(size_t alpha = 0; alpha < alpha_num; alpha++){
			trie[0][alpha] = alpha + 1;
			trie.push_back(std::vector < int > (alpha_num, -1));
		}

		//[2] 
		size_t start_pos = 0;
		
		while(start_pos < input.length()){
			
			size_t node = root;
			size_t left_pos = start_pos;
			size_t right_pos = start_pos;
			
			while(
				right_pos < input.length() && 
				trie[node][encoding(input[right_pos])] != -1 
			)
			{
				node = trie[node][encoding(input[right_pos])];
				++right_pos;
			}
	
			//printf("%d %d\n", (int)left_pos, (int)right_pos);

			if(right_pos == input.length()){
				compressed_vec.push_back(ic(node, '#'));
			}
			else{
				compressed_vec.push_back(ic(node, encoding(input[right_pos])));
				trie[node][encoding(input[right_pos])] = trie.size();
				trie.push_back(std::vector < int > (alpha_num, -1 ));
			}

			start_pos = right_pos;
		}

		return compressed_vec;
	}

	void make_encoding_file(){

	}

	std::string lzw_decode (
		std::vector < ic > &code,
		const std::function < int (char) > &encoding = default_encoding,	
		const std::function < char (int) > &decoding = default_decoding,
		int alpha_num = 26
	)
	{
			
	}
}

#endif
























