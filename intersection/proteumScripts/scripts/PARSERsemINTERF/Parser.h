#include <iostream>
#include <fstream>

#include "Block.h"

using namespace std;

class Parser {
	private:
		vector<Block*> blockVector; // Vetor para todos os nós do grafo.
		int lastArc; // Último arco usado pela função de definição de execução dos arcos.
		int lastFunction; // Última função usada pela função de definição de execução dos arcos.
		unsigned int status; // Variável que armazena se houve ou não sucesso no parsing dos arquivos.
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
