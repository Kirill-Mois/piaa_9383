#include "lab5_1.h"


Bor* CreateBor(const std::vector<std::pair<std::string, int>>& patterns) { //�������� ����
	std::cout << "\n_������ ���������� ����_";
	Bor* bor = new Bor();	// ������ ����
	for (auto& pt : patterns) {
		int NumPattern = find(patterns.begin(), patterns.end(), pt) - patterns.begin();
		std::cout << "\n��������������� " << NumPattern + 1 << " ������: " << pt.first << '\n';
		Bor* current = bor;	// ����� ����, ������� �� �����
		for (auto& c : pt.first) {
			if (current->next.find(c) == current->next.end()) { //���� ������ ����� ���, �� �����������
				current->next.insert({ c, new Bor(current, c) });
				std::cout << "� ��� ��������� �������, ���� ���� ������ (" << c << ")\n";
			}
			else {
				std::cout << "����� (" << c << ") ��� �������� ������� ��� ����������. ���������� ��������.\n";
			}
			current = current->next[c]; // ������� �� ������� �����
			current->NumPattern.push_back(NumPattern);
		}
		current->TermNumPattern = NumPattern; // ��� �������� �������
		current->IsTerminal = true;
		std::cout << "������� �������� ������������, ��������� ���������� ����� ����.\n";
	}
	return bor;
}


void Automaton(Bor* bor) {  //���������� ���. � �������� ������ (���������� ��������)
	std::cout << "\n\n_���������� ���������� � �������� ������_\n";
	std::queue<Bor*> front({ bor });  // ������� ������ ������ � ����
	while (!front.empty()) {
		Bor* current = front.front();
		front.pop();
		Bor* CurrentLink = current->parent->SufLink; // ������������ ������ ����� �������
		const char& key = current->ToParent;	// ����������� ������, ��� �������� ������ ������
		bool IsFound = true;	
		while (CurrentLink->next.find(key) == CurrentLink->next.end()) { 
			if (CurrentLink == bor) {
				std::cout << "���������� ������ �� �������, ������ ����������� �� ������.\n";
				current->SufLink = bor;	// ���� �� ����� ��� ����, �� ������ ��������������� � ������
				IsFound = false;  //������, �� ������ �����, �� �������
				break;
			}
			CurrentLink = CurrentLink->SufLink;
		}
		if (IsFound) {
			CurrentLink = CurrentLink->next.at(key); 
			if (current->parent == bor) {
				std::cout << "������� ������� - ������ �����, ������� ������ ����������� �� ������.\n";
				current->SufLink = bor;	// ��� ������ ������� ������ ������ ����� � ������ (����� ��� ������� ����� ��������� �� ����)
			}
			else {
				current->SufLink = CurrentLink;
				Bor* CurEndLink = current->SufLink;
				while (CurEndLink != bor) {	// ����� �������� ������. ���� ����� �� �����, ������ � ���
					if (CurEndLink->IsTerminal) {
						current->EndLink = CurEndLink;
						break;
					}
					CurEndLink = CurEndLink->SufLink;
				}
			}
		}
		if (!current->next.empty()) {	// ����������� ����� ������� � �������
			for (auto& i : current->next) {
				front.push(i.second);
			}
		}
	}
}


std::pair<int, int> LongestChain(Bor* bor, Bor* root, int& depth) {  //���������� ���� ���������� ������� �� ���. � �������� ������
	std::pair<int, int> longest = { 0, 0 };
	Bor* current = bor;
	while (current->SufLink != root) {
		longest.first++;
		current = current->SufLink;
	}
	longest.first++;
	current = bor;
	while (current->EndLink != nullptr) {
		longest.second++;
		current = current->EndLink;
	}
	for (auto& i : bor->next) {
		std::pair<int, int> next = LongestChain(i.second, root, ++depth);
		if (next.first > longest.first) {
			longest.first = next.first;
		}
		if (next.second > longest.second) {
			longest.second = next.second;
		}
	}
	depth--;
	return longest;
}


void AhoCorasick(const std::string& t, const std::vector<std::pair<std::string, int>>& patterns, std::vector<std::pair<int, int>>& result) {
	int length = 0;
	Bor* bor = CreateBor(patterns);
	Automaton(bor);
	std::cout << "\n\n_����� ����� ������� ������� �� ���������� � �������� ������_\n";
	int depth = 0;
	std::pair<int, int> longest = LongestChain(bor, bor, depth);
	std::cout << "����� ���������� ������� �� ���������� ������: " << longest.first << '\n';
	std::cout << "����� ���������� ������� �� �������� ������: " << longest.second << '\n';
	Bor* current = bor;
	for (int i = 0; i < t.length(); i++) {
		current = current->GetLink(t.at(i));  // �������� ������ ��� �������� 
		Bor* EndLink = current->EndLink;
		while (EndLink != nullptr) {  // ���� � ���� ������� ���� �������� ������, ������ ������� ������
			result.push_back({ i - patterns.at(EndLink->TermNumPattern).first.length() + 2, EndLink->TermNumPattern + 1 });
			EndLink = EndLink->EndLink;	// � ���, ���� ������� �� �������� ������ �� ��������
		}
		if (current->IsTerminal)  // ���� ������� ������������ - ������ ������
			result.push_back({ i - patterns.at(current->TermNumPattern).first.length() + 2, current->TermNumPattern + 1 });
	}
	delete bor;
}

