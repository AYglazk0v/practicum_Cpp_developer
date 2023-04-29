#include <iostream>
#include <vector>

struct Document {
	int id;
	int relevance;
};

bool  HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
		return lhs.relevance > rhs.relevance;
}

int main() {
	std::vector<Document> v_d{{1,3}, {1,1}};
	std::sort(v_d.begin(), v_d.end(), HasDocumentGreaterRelevance);
	for (auto& [id, rel] : v_d) {
		std::cout << "{id=" << id << " rel=" << rel << "}\n"; 
	}
    return 0;
}