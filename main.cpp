#include<iostream>
#include"Block.h"
#include"blockchain.h"
using namespace std;

int main(void){
	cout<<"Probably initialising and validating the genesis block"<<endl;

	Blockchain vrucinaChain;
	TxData mempool[16];

	mempool[0].amount = 6.9;
	mempool[0].from = "Vitalik Buterin";
	mempool[0].to = "JovanVrucina";
	mempool[0].msg = "This code could use some cleanup";

	mempool[1].amount = 4.2;
	mempool[1].from = "JovanVrucina";
	mempool[1].to = "Vitalik Buterin";
	mempool[1].msg = "I know";

	cout<<"Okay, now it's validating the first block"<<endl;
	vrucinaChain.addBlock(mempool, 2);

	for(int i=0; i<2; i++)
		cout<<"\nTransaction "<<i<<":\nAmount: "<<vrucinaChain.chain.back().data[i].amount<<"\nFrom: "<<vrucinaChain.chain.back().data[i].from<<"\nTo: "<<vrucinaChain.chain.back().data[i].to<<"\nMessage: "<<vrucinaChain.chain.back().data[i].msg<<endl;
	

	if(vrucinaChain.isValid())
		cout<<"\nThe data in the blockchain is valid!"<<endl;
	else
		cout<<"\nThe data in the blockchain is invalid"<<endl;

	return EXIT_SUCCESS;	
}
