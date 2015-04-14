#include <iostream>
#include <fstream>

#include "Block.h"

using namespace std;

class Parser {
	private:
		vector<Block*> blockVector; // Vetor para todos os n�s do grafo.
		int lastArc; // �ltimo arco usado pela fun��o de defini��o de execu��o dos arcos.
		int lastFunction; // �ltima fun��o usada pela fun��o de defini��o de execu��o dos arcos.
		unsigned int status; // Vari�vel que armazena se houve ou n�o sucesso no parsing dos arquivos.
		void setArcRuns(unsigned int functionId, unsigned int arcCount);
	public:
		Parser();
		~Parser();
		int main(int argc, char *argv[]);
		void parseGcno(string gcnoFilename);
		void parseGcda(string gcdaFilename);
		string reverseFile(string filename);
		void addBlock(unsigned int block_no, unsigned int functionId, string function);
		Block* getBlock(unsigned int block_no, unsigned int functionId);
		void clearBlocks();
		void setBlockRuns();
		void makeGraph(string filename);
};
