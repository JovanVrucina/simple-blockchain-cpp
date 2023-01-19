#include<iostream>
#include"Block.h"
#include"blockchain.h"
using namespace std;

int main(void){
	cout<<"Initialising and validating the genesis block"<<endl;

	Blockchain vrucinaChain;
	TxData mempool[16];

	mempool[0].amount = 500;
	mempool[0].from = "Test1";
	mempool[0].to = "Test2";
	mempool[0].msg = "Hello, World!";

	mempool[1].amount = 256;
	mempool[1].from = "Test2";
	mempool[1].to = "Test1";
	mempool[1].msg = "r";

	cout<<"Validating the first block"<<endl;
	vrucinaChain.addBlock(mempool, 2);

	for(int i=0; i<2; i++)
		cout<<"\nTransaction "<<i<<":\nAmount: "<<vrucinaChain.chain.back().data[i].amount<<"\nFrom: "<<vrucinaChain.chain.back().data[i].from<<"\nTo: "<<vrucinaChain.chain.back().data[i].to<<"\nMessage: "<<vrucinaChain.chain.back().data[i].msg<<endl;
	

	if(vrucinaChain.isValid())
		cout<<"\nThe data in the blockchain is valid!"<<endl;
	else
		cout<<"\nThe data in the blockchain is invalid"<<endl;

	return EXIT_SUCCESS;	
}
