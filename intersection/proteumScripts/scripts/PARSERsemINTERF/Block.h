#include <cstdlib>
#include <string>
#include <vector>

#include "Arc.h"

using namespace std;

class Block {
	private:
		unsigned int block_no; // Identificador do bloco.
		unsigned int functionId; // Identificador da função.
		int runs;
		string function; // String da função.
		vector<unsigned int> lineno; // Vetor de linhas do código-fonte referentes ao bloco.
		vector<string> lineFilename; // Vetor com o nome do arquivo de cada linha.
		vector<Arc*> arcs; // Vetor de arcos com origem no bloco.
	public:
		Block(unsigned int block, unsigned int functionId, string function);
		~Block();
		unsigned int getId();
		unsigned int getFunctionId();
		string getFunction();
		int getRuns();
		unsigned int getLineNumber();
		unsigned int getArcNumber();
		unsigned int getLine(unsigned int pos);
		string getLineFilename(unsigned int pos);
		unsigned int getArc(unsigned int pos);
		unsigned int getFlag(unsigned int pos);
		unsigned long long getArcRuns(unsigned int pos);
		void setFlag(unsigned int pos, unsigned int flag);
		void setRuns(int runs);
		void setArcRuns(unsigned int pos, unsigned long long count);
		void addLine(unsigned int line, string filename);
		void addArc(unsigned int arc, unsigned int flag);
		void removeArc(unsigned int arc);
		void clearArcs();
};
