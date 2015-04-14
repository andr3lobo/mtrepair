#include "Block.h"

#include <iostream>
using namespace std;

Block::Block(unsigned int block, unsigned int functionId, string function) {
	block_no = block;
	this->functionId = functionId;
	this->function = function;
	runs = -1;
}

Block::~Block() {
	
}

/* Retorna o identificador do bloco. */
unsigned int Block::getId() {
	return block_no;
}

/* Retorna o identificador da função. */
unsigned int Block::getFunctionId() {
	return functionId;
}

/* Retorna a string da função. */
string Block::getFunction() {
	return function;
}

/* Retorna o número de execuções do bloco. */
int Block::getRuns() {
	return runs;
}

/* Retorna o número de linhas. */
unsigned int Block::getLineNumber() {
	return lineno.size();
}

/* Retorna o número de arcos. */
unsigned int Block::getArcNumber() {
	return arcs.size();
}

/* Retorna a linha da posição escolhida. */
unsigned int Block::getLine(unsigned int pos) {
	if (pos < lineno.size())
		return lineno[pos];
	else
		return 0;
}

/* Retorna o nome do arquivo da linha da posição escolhida. */
string Block::getLineFilename(unsigned int pos) {
	if (pos < lineFilename.size())
		return lineFilename[pos];
	else
		return 0;
}

/* Retorna o arco da posição escolhida. */
unsigned int Block::getArc(unsigned int pos) {
	if (pos < arcs.size())
		return arcs[pos]->getBlock();
	else
		return 0;
}

/* Retorna a flag do arco da posição escolhida. */
unsigned int Block::getFlag(unsigned int pos) {
	if (pos < arcs.size())
		return arcs[pos]->getFlag();
	else
		return 0;
}

/* Retorna o número de execuções do arco da posição escolhida. */
unsigned long long Block::getArcRuns(unsigned int pos) {
	if (pos < arcs.size())
		return arcs[pos]->getRuns();
	else
		return 0;
}

/* Define a flag de um arco. */
void Block::setFlag(unsigned int pos, unsigned int flag) {
	arcs[pos]->setFlag(flag);
}

/* Define o número de execuções do bloco. */
void Block::setRuns(int runs) {
	this->runs = runs;
}

/* Define o número de execuções do arco da posição escolhida. */
void Block::setArcRuns(unsigned int pos, unsigned long long count) {
	arcs[pos]->setRuns(count);
}

/* Adiciona uma linha ao bloco. */
void Block::addLine(unsigned int line, string filename) {
	lineno.push_back(line);
	lineFilename.push_back(filename);
}

/* Adiciona um arco ao bloco. */
void Block::addArc(unsigned int arc, unsigned int flag) {
	this->arcs.push_back(new Arc(arc,flag));
}

void Block::removeArc(unsigned int arc) {
	unsigned int count;
	
	vector<Arc*>::iterator it = arcs.begin();
	for(count = 0; it != arcs.end(); it++, count++) {
		if(count == arc) {
			arcs.erase(it);
			break;
		}
	}
}

/* Limpa o vetor de arcos do bloco. */
void Block::clearArcs() {
	unsigned int count;
	
	for (count = 0; count<arcs.size(); count++)
		delete arcs[count];
	arcs.clear();
}
