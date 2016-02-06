#ifndef PRU_H_
#define PRU_H_

class PRU
{
public:
	static void Init();
	static void Execute(const char* file, int pruNum = 0);
	static void Exit();
	static void Write(int index, unsigned int value, int pruNum = 0);
	static unsigned int Read(int index, int pruNum = 0);
	
private:
	static unsigned int* pruDataMem[2];
};

#endif