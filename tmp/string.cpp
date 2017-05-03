#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
	string str,strdirfile,strext;
	str = argv[1];
	cout << str << endl;
	size_t founddirfile=str.find_last_of("/");
	size_t foundext=str.find_last_of(".");
	strdirfile=str.substr(0,foundext);
	strext=str.substr(foundext+1);
	cout << strdirfile << endl;
	cout << strext << endl;
	return 0;
}

