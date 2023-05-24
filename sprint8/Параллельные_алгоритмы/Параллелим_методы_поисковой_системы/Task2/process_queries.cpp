#include "process_queries.h"

#include <algorithm>
#include <execution>

std::vector<std::vector<Document>> ProcessQueries(
	const SearchServer& search_server,
	const std::vector<std::string>& queries) {
	std::vector<std::vector<Document>> ret_vector(queries.size());
	std::transform(
		std::execution::par,
		queries.begin(), queries.end(),
		ret_vector.begin(),
		[&search_server](const auto& str){return search_server.FindTopDocuments(str);}
	);
	return ret_vector;
}

std::vector<Document> ProcessQueriesJoined(
	const SearchServer& search_server,
	const std::vector<std::string>& queries) {
	std::vector<Document> documents;
	for (auto&& tmp = ProcessQueries(search_server, queries); const auto& document : tmp) {
		std::cerr << document.back() << std::endl;
		documents.insert(documents.end(), document.begin(), document.end());
	}
	return documents;
}