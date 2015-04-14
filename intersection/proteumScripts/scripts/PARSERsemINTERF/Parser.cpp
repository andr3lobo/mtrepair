#include "Parser.h"

int main(int argc, char *argv[]) {
	Parser *parser = new Parser();
	string filename, gcno, gcda;
	
	if (argc > 1) {
		filename = argv[1];
		
		/* Executando os métodos para a leitura dos arquivos .gcno e
		.gcda gerados pelo GCC e gerando um grafo a partir das
		informações obtidas. */
		gcno = filename.substr(0,filename.size())+".gcno";
		gcda = filename.substr(0,filename.size())+".gcda";
		parser->parseGcno(gcno);
		parser->parseGcda(gcda);
		parser->setBlockRuns();
		parser->makeGraph(filename);
		
		/* Liberando a memória alocada anteriormente. */
		parser->clearBlocks();
		delete parser;
	} else
		cout << "Argumentos Insuficientes.";
	
	return 0;
}

Parser::Parser() {
	lastFunction = -1;	// Definindo o atributo com um identificador de função inválido.
	status = 0;
}

Parser::~Parser() {
	
}

void Parser::parseGcno(string gcnoFilename) {
	char buffer[4];	// Tamanho de cada dado a ser lido no arquivo .gcno (32 bits).
	unsigned int magic, count, stringCount, flag, length, block, functionId, arc;
	string function, filename, reversed;
	
	ifstream gcnoFile(gcnoFilename.c_str(), ios::in | ios::binary);
	if (gcnoFile.is_open()) {
		cout << "Lendo Arquivo GCNO..." << endl;		
		gcnoFile.read(buffer,4); // Valor inicial dos arquivos .gcno (0x67636e6f ou 0x6f6e6367).
		magic = *((unsigned int*)buffer);
		gcnoFile.read(buffer,4); // Versão do GCC utilizada na geração do arquivo.
		cout << "Version: " << buffer[3] << buffer[2] << buffer[1] << buffer[0] << endl;
		gcnoFile.read(buffer,4); // Valor para sincronização dos arquivos .gcno e .gcda.
		cout << "Stamp: " << hex << *((unsigned int*)buffer) << endl;
		
		if (magic == 0x67636e6f) {
			while (gcnoFile.read(buffer,4)) {
			
				/* Blocos com dados sobre a função. */
				if (*((unsigned int*)buffer) == 0x01000000) {
					gcnoFile.read(buffer,4); // Tamanho do bloco.
					length = *((unsigned int*)buffer);
					gcnoFile.read(buffer,4); // Identificador da função.
					functionId = *((unsigned int*)buffer);
					gcnoFile.read(buffer,4); // Checksum da função.
					cout << "Checksum: " << *((unsigned int*)buffer) << endl;
					gcnoFile.read(buffer,4); // Tamanho da string seguinte.
					function.clear();
					for (count = *((unsigned int*)buffer); count > 0; count--) {
						gcnoFile.read(buffer,4); // String com o nome da função.
						for (stringCount = 0; stringCount < 4 && buffer[stringCount] != 0; stringCount++)
							function += buffer[stringCount];
					}
					gcnoFile.read(buffer,4); // Tamanho da string seguinte.
					cout << "File Name: ";
					for (count = *((unsigned int*)buffer); count > 0; count--) {
						gcnoFile.read(buffer,4); // String com o nome do arquivo.
						cout << buffer[0] << buffer[1] << buffer[2] << buffer[3];
					}
					cout << endl;
					gcnoFile.read(buffer,4); // Número da linha inicial da função.
					cout << "Line Number: " << *((unsigned int*)buffer) << endl;
					
				/* Blocos com dados sobre os arcos. */
				} else if (*((unsigned int*)buffer) == 0x01430000) {
					gcnoFile.read(buffer,4); // Tamanho do bloco.
					length = *((unsigned int*)buffer);
					gcnoFile.read(buffer,4); // Identificador do bloco.
					block = *((unsigned int*)buffer);
					addBlock(block,functionId,function); // Adicionando o bloco de origem caso não exista.
					
					/* Adicionando os arcos com os valores armazenados no restante do bloco. */
					for (count = length-1; count > 0; count -= 2) {
						gcnoFile.read(buffer,4); // Bloco de destino do arco.
						arc = *((unsigned int*)buffer);
						addBlock(arc,functionId,function); // Adicionando o bloco de destino caso não exista.
						gcnoFile.read(buffer,4); // Flag do arco.
						getBlock(block,functionId)->addArc(arc,*((unsigned int*)buffer));
					}
					
				/* Blocos com dados sobre as linhas. */
				} else if (*((unsigned int*)buffer) == 0x01450000) {
					gcnoFile.read(buffer,4); // Tamanho do bloco.
					length = *((unsigned int*)buffer);
					gcnoFile.read(buffer,4); // Identificador do bloco.
					block = *((unsigned int*)buffer);
					addBlock(block,functionId,function);
					
					
					flag = 1;
					while (flag) {
						gcnoFile.read(buffer,4);
						if (*((unsigned int*)buffer) == 0) {
							gcnoFile.read(buffer,4);
							if (*((unsigned int*)buffer) == 0) // 2 valores 0 sinalizam o final do bloco.
								flag = 0;
							else {
								filename.clear(); // 1 valor 0 e outro diferente sinalizam uma string com o tamanho do segundo valor.
								for (count = *((unsigned int*)buffer); count > 0; count--) {
									gcnoFile.read(buffer,4); // String correspondente ao nome do arquivo.
									for (stringCount = 0; stringCount < 4 && buffer[stringCount] != 0; stringCount++)
										filename += buffer[stringCount]; // Concatenando os valores da string.
								}
							}
						} else // Um valor diferente de 0 sinaliza o número de uma linha.
							getBlock(block,functionId)->addLine(*((unsigned int*)buffer),filename);
					}
					
				/* Blocos não utilizados. */
				} else if (*((unsigned int*)buffer) == 0x01410000) {
					gcnoFile.read(buffer,4);
					length = *((unsigned int*)buffer);
					for (count = length; count > 0; count--)
						gcnoFile.read(buffer,4);
				
				/* Blocos desconhecidos. */
				} else {
					cout << "Bloco desconhecido: " << hex << *((unsigned int*)buffer) << endl;
					gcnoFile.read(buffer,4);
					length = *((unsigned int*)buffer);
					for (count = length; count > 0; count--)
						gcnoFile.read(buffer,4);
				}
			}
			status = status | (1 << 0);
		} else if (magic != 0x6f6e6367)
			cout << "Arquivo GCNO invalido." << endl;
	} else
		cout << "Erro na leitura do arquivo " << gcnoFilename << endl;
	gcnoFile.close();
	
	if (magic == 0x6f6e6367) { // Small-endian.
		reversed = reverseFile(gcnoFilename);
		if (reversed == "")
			cout << "Erro ao reverter arquivo GCNO." << endl;
		else
			parseGcno(reversed);
	}
}

void Parser::parseGcda(string gcdaFilename) {
	char buffer[4];	// Tamanho de cada dado a ser lido no arquivo .gcda (32 bits).
	unsigned int magic, count, length, temp, functionId;
	unsigned long long value64;
	string reversed;
	
	ifstream gcdaFile(gcdaFilename.c_str(), ios::in | ios::binary);
	if (gcdaFile.is_open()) {
		cout << "Lendo Arquivo GCDA..." << endl;
		gcdaFile.read(buffer,4); // Valor inicial dos arquivos .gcda (0x67636461 ou 0x61646367).
		magic = *((unsigned int*)buffer);
		gcdaFile.read(buffer,4); // Versão do GCC utilizada na geração do arquivo.
		cout << "Version: " << buffer[3] << buffer[2] << buffer[1] << buffer[0] << endl;
		gcdaFile.read(buffer,4); // Valor para sincronização dos arquivos .gcno e .gcda.
		cout << "Stamp: " << hex << *((unsigned int*)buffer) << endl;
		
		if (magic == 0x67636461) {
			while (gcdaFile.read(buffer,4)) {
			
				/* Blocos com dados sobre a função. */
				if (*((unsigned int*)buffer) == 0x01000000) {
					gcdaFile.read(buffer,4); // Tamanho do bloco.
					length = *((unsigned int*)buffer);
					gcdaFile.read(buffer,4); // Identificador da função.
					functionId = *((unsigned int*)buffer);
					gcdaFile.read(buffer,4); // Checksum da função.
					cout << "Checksum: " << *((unsigned int*)buffer) << endl;
					
				/* Blocos com dados sobre a execução dos arcos. */
				} else if (*((unsigned int*)buffer) == 0x01a10000) {
					gcdaFile.read(buffer,4); // Tamanho do bloco.
					length = *((unsigned int*)buffer);
					for (count = length; count > 0; count -= 2) { // Leitura da contagem dos arcos (64 bits).
						gcdaFile.read(buffer,4);
						temp = *((unsigned int*)buffer);
						gcdaFile.read(buffer,4);
						value64 = (*((unsigned int*)buffer))*0x100000000LLU+temp;
						setArcRuns(functionId,value64);
					}
					
				/* Blocos com resumos. */
				} else if (*((unsigned int*)buffer) == 0xa1000000 || *((unsigned int*)buffer) == 0xa3000000) {
					if (*((unsigned int*)buffer) == 0xa1000000)
						cout << endl << "Resumo do objeto:" << endl;
					else
						cout << endl << "Resumo do programa:" << endl;
					gcdaFile.read(buffer,4); // Tamanho do bloco.
					length = *((unsigned int*)buffer);
					gcdaFile.read(buffer,4);
					cout << "Checksum: " << hex << *((unsigned int*)buffer) << endl;
					gcdaFile.read(buffer,4); // Número de arcos com um número definido de execuções.
					cout << "Num: " << dec << *((unsigned int*)buffer) << endl;
					gcdaFile.read(buffer,4); // Número total de execuções.
					cout << "Runs: " << *((unsigned int*)buffer) << endl;
					gcdaFile.read(buffer,4); 
					temp = *((unsigned int*)buffer);
					gcdaFile.read(buffer,4);  // Soma das execuções de todos os arcos em todas as execuções.
					cout << "Sum: " << (*((unsigned int*)buffer))*0x100000000LLU+temp << endl;
					gcdaFile.read(buffer,4);
					temp = *((unsigned int*)buffer);
					gcdaFile.read(buffer,4); // Número máximo de execuções de um arco em uma execução.
					cout << "Max: " << (*((unsigned int*)buffer))*0x100000000LLU+temp << endl;
					gcdaFile.read(buffer,4);
					temp = *((unsigned int*)buffer);
					gcdaFile.read(buffer,4); // Número máximo de execuções de um arco em todas as execuções do programa.
					cout << "Sum Max: " << (*((unsigned int*)buffer))*0x100000000LLU+temp << endl;
				
				/* Blocos desconhecidos. */
				} else {
					cout << "Bloco desconhecido: " << hex << *((unsigned int*)buffer) << endl;
					gcdaFile.read(buffer,4);
					length = *((unsigned int*)buffer);
					for (count = length; count > 0; count--)
						gcdaFile.read(buffer,4);
				}
			}
			status = status | (1 << 1);
		} else if (magic != 0x61646367)
			cout << "Arquivo GCDA invalido." << endl;
	} else
		cout << "Erro na leitura do arquivo " << gcdaFilename << endl;
	gcdaFile.close();
	
	if (magic == 0x61646367) { // Small-endian.
		reversed = reverseFile(gcdaFilename);
		if (reversed == "")
			cout << "Erro ao reverter arquivo GCDA." << endl;
		else
			parseGcda(reversed);
	}
}

/* Atribuindo o número de execuções para cada arco.
Os dados devem ser enviados de acordo com sua ordem no arquivo .gcda. */
void Parser::setArcRuns(unsigned int functionId, unsigned int arcCount) {
	unsigned int count = 0, flag = 0, subcount;
	int arcs = -1;
	
	if (lastFunction != (int)functionId) {
		lastFunction = (int)functionId;
		lastArc = -1; // Reiniciando o contador de arcos caso uma nova função seja iniciada.
	}
	
	while (getBlock(count,functionId) != NULL && !flag) {
		for (subcount = 0; subcount < getBlock(count,functionId)->getArcNumber() && !flag; subcount++)
			
			/* O número de execuções é atribuído apenas para arquivos sem a flag GCOV_ARC_ON_TREE. */
			if ((getBlock(count,functionId)->getFlag(subcount) & 1) == 0){
				arcs++;
				if (arcs > lastArc) {
					getBlock(count,functionId)->setArcRuns(subcount,arcCount);
					lastArc++;
					flag = 1;
				}
			}
		count++;
	}
}

/* Transforma um arquivo de small-endian para big-endian. */
string Parser::reverseFile(string filename) {
	unsigned int temp;
	char buffer[4];
	string newFilename = filename+"_reversed";
	
	ifstream oldFile(filename.c_str(), ios::in | ios::binary); // Arquivo original small-endian.
	ofstream newFile(newFilename.c_str(), ios::out | ios::binary); // Novo arquivo big-endian.
	
	if (oldFile.is_open() && newFile.is_open())
		while (oldFile.read(buffer,4)) {
			temp = buffer[0];
			buffer[0] = buffer[3];
			buffer[3] = temp;
			temp = buffer[1];
			buffer[1] = buffer[2];
			buffer[2] = temp;
			newFile.write(buffer,4);
		}
	else // Erro na leitura/escrita de um dos arquivos.
		newFilename = "";
	
	return newFilename;
}

/* Adiciona um bloco ao vetor caso não exista outro com o mesmo identificador e a mesma função. */
void Parser::addBlock(unsigned int block_no, unsigned int functionId, string function) {
	unsigned int count, flag = 0;
	
	for (count=0; count<blockVector.size() && !flag; count++) // Verificando se o bloco já existe.
		if (blockVector[count]->getId() == block_no && blockVector[count]->getFunctionId() == functionId)
			flag = 1;
	if (!flag)
		blockVector.push_back(new Block(block_no,functionId,function));
}

/* Retorna o bloco com o identificador e a função escolhidos, NULL caso não exista. */
Block* Parser::getBlock(unsigned int block_no, unsigned int functionId) {
	unsigned int count, flag = 0;
	Block *temp = NULL;
	
	for (count = 0; count < blockVector.size() && !flag; count++) {
		if (blockVector[count]->getId() == block_no && blockVector[count]->getFunctionId() == functionId) {
			temp = blockVector[count];
			flag = 1;
		}
	}
	return temp;
}

/* Limpa o vetor com os blocos. */
void Parser::clearBlocks() {
	unsigned int count;
	
	for (count = 0; count<blockVector.size(); count++) {
		blockVector[count]->clearArcs(); // Limpando os vetores de arcos de cada bloco.
		delete blockVector[count];
	}
	blockVector.clear();
}

void Parser::setBlockRuns() {
	unsigned int flag = 0, count, subCount, arcCount, subArcCount,
		function, first, last, lastId;
	int noFallthrough;

	for (count = 0; count<blockVector.size(); count++)
		for (arcCount = 0; arcCount<blockVector[count]->getArcNumber(); arcCount++) {
			if (!(blockVector[count]->getFlag(arcCount) & (1<<0))) { // Arco com contador definido.
				blockVector[count]->setFlag(arcCount, (blockVector[count]->getFlag(arcCount) | (1<<3)) );
			}
			if (blockVector[count]->getFlag(arcCount) & (1<<1)) { // Arco falso.
				blockVector[count]->removeArc(arcCount);
				arcCount--;
			}
		}
			
	for (count = 0; count<blockVector.size(); count++) { // Adicionando um arco do último ao primeiro bloco.
		if (function != blockVector[count]->getFunctionId()) {
			if (flag)
				blockVector[last]->addArc(first,(1<<1));
			else
				flag++;
			function = blockVector[count]->getFunctionId();
			first = blockVector[count]->getId();
			last = count;
			lastId = blockVector[count]->getId();
		}
		if (blockVector[count]->getId() > lastId) {
			last = count;
			lastId = blockVector[count]->getId();
		}
		if (blockVector[count]->getId() < first)
			first = blockVector[count]->getId();
	}
	blockVector[last]->addArc(first,(1<<1));
	
	flag = 0;
	while (!flag) {
		flag = 1;
		for (count = 0; count<blockVector.size(); count++)
			if (blockVector[count]->getRuns() == -1) {
				flag = 0;
				noFallthrough = -1;
				for (arcCount = 0; arcCount<blockVector[count]->getArcNumber(); arcCount++) {
					if (!(blockVector[count]->getFlag(arcCount) & (1<<2))) { // Buscando arcos sem fallthrough.
						noFallthrough = arcCount;
					}
				}
				if (noFallthrough != -1) { // Para arcos sem fallthrough encontrados.
					if (blockVector[count]->getFlag(noFallthrough) & (1<<3)) {
						blockVector[count]->setRuns(blockVector[count]->getArcRuns(noFallthrough));
						for (subCount = 0; subCount<blockVector.size(); subCount++)
							if (blockVector[subCount]->getFunctionId() == blockVector[count]->getFunctionId())
								for (subArcCount = 0; subArcCount<blockVector[subCount]->getArcNumber(); subArcCount++)
									if ( (blockVector[subCount]->getArc(subArcCount) == blockVector[count]->getId())
										&& !((blockVector[subCount]->getFlag(subArcCount)) & (1<<3)) ) {
										blockVector[subCount]->setArcRuns(subArcCount,blockVector[count]->getRuns());
										blockVector[subCount]->setFlag(subArcCount, blockVector[subCount]->getFlag(subArcCount) | (1<<3) );
									}
					}
				} else { // Para blocos apenas com arcos com fallthrough.
					noFallthrough = 0;
					if (blockVector[count]->getFlag(noFallthrough) & (1<<3)) {
						blockVector[count]->setRuns(blockVector[count]->getArcRuns(noFallthrough));
						for (subCount = 0; subCount<blockVector.size(); subCount++)
							if (blockVector[subCount]->getFunctionId() == blockVector[count]->getFunctionId())
								for (subArcCount = 0; subArcCount<blockVector[subCount]->getArcNumber(); subArcCount++)
									if ( (blockVector[subCount]->getArc(subArcCount) == blockVector[count]->getId())
										&& !((blockVector[subCount]->getFlag(subArcCount)) & (1<<3)) ) {
										blockVector[subCount]->setArcRuns(subArcCount,blockVector[count]->getRuns());
										blockVector[subCount]->setFlag(subArcCount, blockVector[subCount]->getFlag(subArcCount) | (1<<3) );
									}
					}
				}
			}
	}
}

/* Gera o grafo do programa através dos dados obtidos nos arquivos .gcno e .gcda com o Graphviz. */
void Parser::makeGraph(string filename) {
	ofstream graphFile;
	string textFilename, imageFilename, dot;
	int functionId = -1;
	unsigned int count, subCount, arcCount, lineCount, subLineCount, temp, flag;
	
	cout << endl << "Escrevendo arquivos..." << endl;
	if (status & 1) { // Sucesso no parsing do arquivo .gcno.
		for (count = 0; count<blockVector.size(); count++) {
			if (functionId != (int)blockVector[count]->getFunctionId()) { // Encerrando os últimos arquivos e criando novos arquivos caso outra função seja iniciada.
				functionId = (int)blockVector[count]->getFunctionId();
				if (graphFile.is_open()) {
					graphFile << "}" << endl;
					graphFile.close(); // Fechando o último arquivo de texto.
					dot = "dot -T png -o "+imageFilename+" "+textFilename; // Definindo o texto da linha de comando.
					system(dot.c_str()); // Gerando a imagem do grafo com o Graphviz.
				}
				textFilename = (filename.substr(0,filename.size()))+"_"+(blockVector[count]->getFunction())+".txt"; // Definindo o nome do arquivo de texto a partir do nome do arquivo do código fonte.
				imageFilename = (textFilename.substr(0,textFilename.size()-4))+".png"; // Definindo o nome do arquivo de imagem a partir do nome do arquivo de texto.
				graphFile.open(textFilename.c_str()); // Abrindo o arquivo de texto a ser utilizado.
				graphFile << "digraph {" << endl;
				graphFile << blockVector[count]->getId() << " [fontsize=12.0]" << endl; // Adicionando os blocos ao arquivo de texto e definindo o tamanho da fonte.
				for (arcCount = 0; arcCount<blockVector[count]->getArcNumber(); arcCount++) // Adicionando os arcos ao arquivo de texto.
					if (!(blockVector[count]->getFlag(arcCount) & (1<<1)))
						graphFile << blockVector[count]->getId() << "->" << blockVector[count]->getArc(arcCount) << endl;
			} else {

				if (blockVector[count]->getRuns() == 0) // Se bloco nao for acessado, sera colorido com vermelho.
				graphFile << blockVector[count]->getId() << " [color=red, fontsize=12.0]" << endl;

				else	graphFile << blockVector[count]->getId() << " [fontsize=12.0]" << endl;

				for (arcCount = 0; arcCount<blockVector[count]->getArcNumber(); arcCount++) // Adicionando os arcos ao arquivo de texto. 
					if (!(blockVector[count]->getFlag(arcCount) & (1<<1)))
						graphFile << blockVector[count]->getId() << "->" << blockVector[count]->getArc(arcCount) << endl;
			}
		}
		graphFile << "}" << endl;
		dot = "dot -T png -o "+imageFilename+" "+textFilename; // Definindo o texto da linha de comando.
		system(dot.c_str()); // Gerando a imagem do grafo com o Graphviz.
		graphFile.close();
		functionId = -1; // Reiniciar variavel (alterado!).
		
		for (count = 0; count<blockVector.size(); count++) { // Gerando os grafos com contadores de arcos.
			if (functionId != (int)blockVector[count]->getFunctionId()) { // Encerrando os últimos arquivos e criando novos arquivos caso outra função seja iniciada.
				functionId = (int)blockVector[count]->getFunctionId();
				if (graphFile.is_open()) {
					graphFile << "}" << endl;
					graphFile.close(); // Fechando o último arquivo de texto.
					dot = "dot -T png -o "+imageFilename+" "+textFilename; // Definindo o texto da linha de comando.
					system(dot.c_str()); // Gerando a imagem do grafo com o Graphviz.
				}
				textFilename = (filename.substr(0,filename.size()))+"_"+(blockVector[count]->getFunction())+"_arcs.txt"; // Definindo o nome do arquivo de texto a partir do nome do arquivo do código fonte.
				imageFilename = (textFilename.substr(0,textFilename.size()-4))+".png"; // Definindo o nome do arquivo de imagem a partir do nome do arquivo de texto.
				graphFile.open(textFilename.c_str()); // Abrindo o arquivo de texto a ser utilizado.
				graphFile << "digraph {" << endl;
				graphFile << blockVector[count]->getId() << " [fontsize=12.0]" << endl; // Adicionando os blocos ao arquivo de texto e definindo o tamanho da fonte.
				for (arcCount = 0; arcCount<blockVector[count]->getArcNumber(); arcCount++) // Adicionando os arcos ao arquivo de texto.
					if (!(blockVector[count]->getFlag(arcCount) & (1<<1)))
						if (blockVector[count]->getArcRuns(arcCount)!=0)//if (!(blockVector[count]->getFlag(arcCount) & (1<<0)))
							graphFile << blockVector[count]->getId() << "->" << blockVector[count]->getArc(arcCount) << " [label=" << blockVector[count]->getArcRuns(arcCount) << ",fontsize=12.0]" << endl;
						else
							graphFile << blockVector[count]->getId() << "->" << blockVector[count]->getArc(arcCount) << endl;
			} else {
				if (blockVector[count]->getRuns() == 0) // Se bloco nao for acessado, sera colorido com vermelho.
				graphFile << blockVector[count]->getId() << " [color=red, fontsize=12.0]" << endl;
				
				else		graphFile << blockVector[count]->getId() << " [fontsize=12.0]" << endl;

				for (arcCount = 0; arcCount<blockVector[count]->getArcNumber(); arcCount++) // Adicionando os arcos ao arquivo de texto.
					if (!(blockVector[count]->getFlag(arcCount) & (1<<1)))
						if (blockVector[count]->getArcRuns(arcCount)!=0)//if (!(blockVector[count]->getFlag(arcCount) & (1<<0)))
							graphFile << blockVector[count]->getId() << "->" << blockVector[count]->getArc(arcCount) << " [label=" << blockVector[count]->getArcRuns(arcCount) << ",fontsize=12.0]" << endl;
						else
							graphFile << blockVector[count]->getId() << "->" << blockVector[count]->getArc(arcCount) << endl;
			}
		}
		graphFile << "}" << endl;
		dot = "dot -T png -o "+imageFilename+" "+textFilename; // Definindo o texto da linha de comando.
		system(dot.c_str()); // Gerando a imagem do grafo com contadores de arcos com o Graphviz.
		graphFile.close();
				
		functionId = -1;
		for (count = 0; count<blockVector.size(); count++) { // Imprimindo as linhas de cada bloco de cada função no arquivo .lines.
			if (functionId != (int)blockVector[count]->getFunctionId()) { // Encerrando os últimos arquivos e criando novos arquivos caso outra função seja iniciada.
				functionId = (int)blockVector[count]->getFunctionId();
				if (graphFile.is_open())
					graphFile.close();
				textFilename = (filename.substr(0,filename.size()))+"_"+(blockVector[count]->getFunction())+".lines";
				graphFile.open(textFilename.c_str());
			}
			graphFile << blockVector[count]->getId() << endl;
			for (lineCount = 0; lineCount<blockVector[count]->getLineNumber(); lineCount++)
				graphFile << " " << blockVector[count]->getLine(lineCount) << endl;
		}
		
		functionId = -1;
		for (count = 0; count<blockVector.size(); count++) { // Imprimindo as linhas de cada bloco de cada função no arquivo .lines.
			if (functionId != (int)blockVector[count]->getFunctionId()) { // Encerrando os últimos arquivos e criando novos arquivos caso outra função seja iniciada.
				functionId = (int)blockVector[count]->getFunctionId();
				if (graphFile.is_open())
					graphFile.close();
				textFilename = (filename.substr(0,filename.size()))+"_"+(blockVector[count]->getFunction())+".runs";
				graphFile.open(textFilename.c_str());
			}
			for (lineCount = 0; lineCount<blockVector[count]->getLineNumber(); lineCount++) {
				flag = 0;
				temp = blockVector[count]->getRuns();
				for (subCount = 0; subCount<blockVector.size(); subCount++)
					for (subLineCount = 0; subLineCount<blockVector[subCount]->getLineNumber(); subLineCount++)
						if ( (blockVector[count]->getLine(lineCount) == blockVector[subCount]->getLine(subLineCount)) && (subCount!=count) )
							if (subCount > count)
								flag = 1;
							else
								temp += blockVector[subCount]->getRuns();
				if (!flag)
					graphFile << blockVector[count]->getLine(lineCount) << " - " << temp << endl;
			}
		}
		graphFile.close();
	}
}
