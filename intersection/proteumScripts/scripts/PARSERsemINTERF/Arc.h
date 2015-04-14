#include <vector>

class Arc {
	private:
		unsigned int block; // Bloco de destino do arco.
		unsigned int flag; // Flag do arco.
		unsigned long long runs; // Número de execuções do arco.
	public:
		Arc(unsigned int block, unsigned int flag);
		~Arc();
		void setFlag(unsigned int flag);
		void setRuns(unsigned long long runs);
		unsigned int getBlock();
		unsigned int getFlag();
		unsigned long long getRuns();
};
