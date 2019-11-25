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

	std::vector < std::vector < int > > 
	make_initial_trie (size_t alpha_num){
		std::vector < std::vector < int > > trie;
		trie.push_back(std::vector < int > (alpha_num));
		for(size_t alpha = 0; alpha < alpha_num; alpha++){
			trie[0][alpha] = alpha + 1;
			trie.push_back(std::vector < int > (alpha_num, -1));
		}
		return trie;
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
		std::vector < std::vector < int > > trie
				= make_initial_trie(alpha_num);

		//[2] compression 
		size_t start_pos = 0;
		
		while(start_pos < input.length()){
			
			int node = root;
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
	
			if(right_pos == input.length()){
				compressed_vec.push_back(ic(node, '#'));
			}
			else{
				compressed_vec.push_back(ic(node, input[right_pos]));
				trie[node][encoding(input[right_pos])] = trie.size();
				trie.push_back(std::vector < int > (alpha_num, -1 ));
			}

			start_pos = right_pos;
		}

		return compressed_vec;
	}

	void make_encoding_file(){
		
	}

	std::string
	get_trie_path (
		const std::vector < std::vector < int > > &trie,
		const std::vector < int > parent,
		const std::vector < int > label,
		const std::function < char (int) > &decoding,
		int node,
		const int root = 0
	)
	{
		std::string path;
		while(node!=root){
			path += decoding(label[node]);
			node = parent[node];
		}
		std::reverse(path.begin(), path.end());
		return path;
	}

	std::string lzw_decode (
		std::vector < ic > &code,
		const std::function < int (char) > &encoding = default_encoding,	
		const std::function < char (int) > &decoding = default_decoding,
		int alpha_num = 26
	)
	{
		//[1]
		for(auto &p : code){
			int node = p.first;
			char next_char = p.second;
			std::cout << node << " " << next_char << '\n'; 
		}

		//[2]
		std::vector < std::vector < int > > trie;
		trie = make_initial_trie(alpha_num);

		std::vector < int > parent(alpha_num + 1);
		std::vector < int > label(alpha_num + 1);
		for(int i=0;i<=alpha_num;i++){
			label[i+1] = i;
		}

		std::string string_decoded;

		for(auto &p : code){
			int node = p.first;
			char next_char = p.second;
			
			//[1] : decode string
			auto s = get_trie_path(trie, parent, label, decoding, node);
			string_decoded += s;

			//[2] : insert new 
			int new_node = trie.size();

			parent.push_back(node);
			
			trie[node][encoding(next_char)] = new_node;
			trie.push_back(std::vector < int > (alpha_num, -1));
			
			label.push_back(encoding(next_char));
		}
		
		return string_decoded;
	}
}

#endif





