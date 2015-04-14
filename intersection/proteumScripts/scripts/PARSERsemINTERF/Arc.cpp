#include "Arc.h"

Arc::Arc(unsigned int block, unsigned int flag) {
	this->block = block;
	this->flag = flag;
	runs = 0;
}

Arc::~Arc() {
	
}

/* Define a flag. */
void Arc::setFlag(unsigned int flag) {
	this->flag = flag;
}

/* Define o número de execuções. */
void Arc::setRuns(unsigned long long runs) {
	this->runs = runs;
}

/* Retorna o identificador do bloco de destino. */
unsigned int Arc::getBlock() {
	return block;
}

/* Retorna a flag. */
unsigned int Arc::getFlag() {
	return flag;
}

/* Retorna o número de execuções. */
unsigned long long Arc::getRuns() {
	return runs;
}
