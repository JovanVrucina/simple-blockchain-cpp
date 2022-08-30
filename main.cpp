#include<iostream>
#include<string>
#include<ctime>
#include<vector>
using namespace std;

/*************************************CLASSES DECLARATION******************************************************/
//transaction data (struct)
class TxData{
	public:
		string from;
		string to;
		string msg; //transaction message could be anything form hello world to functoin args(future implementations)
		double amount;
		time_t timestamp;
		//double gas (future implementation)
};

//block class
class Block{


	private:
		friend class Blockchain;
		unsigned int index;
		unsigned int TxNum; //number of Tx in the block
		unsigned int nonce; //nonce for checking the validity of Tx
		size_t blockHash;
		size_t prevHash;
		size_t validateHash(); //proof-of-work hash generation
		Block(int idx, TxData transacData[16], size_t _prevHash, int txNum);
	
	public:
		//constructor
		//	Block(int idx, TxData transacData[16], size_t _prevHash, int txNum);

			size_t getHash(); //hash getter function
			size_t generateHash(int nonce); //generating a hash with nonce for validity purposes
			size_t getPrevHash();//getter for previous block hash
			int getIndex(); //getter for index
			int getNonce(); //getter for nonce

			TxData *data; //transaction array for a single block (max 16 tx)

			bool isHashValid(); //function to check validity of the block
};

class Blockchain{
	private:
		Block createGenesis(); //creating a genesis block

	public:
		vector<Block> chain; //chain of blocks - vector 

		Blockchain(); //constructor for the blockchain (calls createGenesis(); which calls addBlock();)

		void addBlock(TxData transacData[16], unsigned int txNum); //function for adding a block to the blockchain
		bool isValid(); //function for checking the validity of the whole blockchain

		Block *getLatestBlock(); //FOR DEMO PURPOSES ONLY!!!!! GETTING THE POINTER TO THE LATEST BLOCK TO DEMONSTRATE AN ATTACK ON THE NETWORK
};

void newTx(TxData *_mempool, unsigned short *_txNum);

/********************************************BLOCK CLASS MEMBERS*********************************************************************/
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
	
	cout<<"\nBlock info:\n"<<"Hash: "<<vrucinaChain.chain.back().getHash()<<"\nBlock index: "<<vrucinaChain.chain.back().getIndex()<<"\nBlock nonce: "<<vrucinaChain.chain.back().getNonce()<<endl;

	if(vrucinaChain.chain.back().isHashValid())
		cout<<"\nThe data in the blockchain is valid!"<<endl;
	else
		cout<<"\nThe data in the blockchain is invalid"<<endl;

	return EXIT_SUCCESS;	
}

/*************************************FUNCTIONS****************************************************************/
void newTx(TxData *_mempool, unsigned short *_txNum){
	system("clear");
	if(*_txNum>=16){
		cout<<"Mempool full, please write Txs to chain"<<endl;
		return;
	}

	cout<<"From: ";
	cin>>_mempool[*_txNum].from;
	cout<<"To: ";
	cin>>_mempool[*_txNum].to;
	cout<<"Amount: ";
	cin>>_mempool[*_txNum].amount;
	cout<<"Transaction message: ";
	cin>>_mempool[*_txNum].msg;

	(*_txNum)++;
}

/*************************************CLASS MEMBER FUNCTIONS***************************************************/

/*************************************BLOCK CLASS MEMBERS******************************************************/
Block::Block(int idx, TxData transacData[16], size_t _prevHash, int txNum){
	index = idx;
	TxNum = txNum;

	data = new TxData[TxNum];
	for(int i=0; i<TxNum; i++){
		data[i] = transacData[i];
		data[i].timestamp = time(NULL);
	}

	prevHash = _prevHash;
	blockHash = validateHash();
}

size_t Block::validateHash(){
	nonce = 0;
	size_t fHash;
	do{	
		fHash = generateHash(nonce++);
	}while(fHash/10000000000000!=0);

	nonce--;
	return fHash;
		
}

size_t Block::generateHash(int _nonce){
	hash<string> hash1;
	hash<size_t> finalHash;

	//initialising hash
	string toHash = "\0";
	for(int i=0; i<TxNum;i++){
		string bufferHash = to_string(data[i].amount)+data[i].from+data[i].to+to_string(data[i].timestamp)+data[i].msg;
		toHash += bufferHash;
	}
	toHash += to_string(_nonce);

	return finalHash(hash1(toHash)+prevHash);

}

//block hash getter function
size_t Block::getHash(){
	return blockHash;
}

//block previous hash getter function
size_t Block::getPrevHash(){
	return prevHash;
}

int Block::getIndex(){
	return index;
}

int Block::getNonce(){
	return nonce;
}

//block function isHashValid
bool Block::isHashValid(){
	return generateHash(nonce) == blockHash;
}



/*************************************BLOCKCHAIN CLASS MEMBERS******************************************************/

Blockchain::Blockchain(){
		Block genesis = createGenesis();
		chain.push_back(genesis);
}

//function that creates genesis block
Block Blockchain::createGenesis(){
	TxData dat[1];
	dat[0].amount = 0;
	dat[0].from = "NIL";
	dat[0].to = "NIL";
	dat[0].msg = "NULL";
	dat[0].timestamp = time(NULL);

	hash<int> hash1;
	Block genesis(0,dat, hash1(0), 1);

	return genesis;
}

//FOR DEMO PURPOSES ONLY!!!!!!!!!!!
Block *Blockchain::getLatestBlock(){
	return &chain.back();
}

void Blockchain::addBlock(TxData transacData[16], unsigned int txNum){
	int index = chain.size();

	for(int i=0; i<txNum; i++){
		transacData[i].timestamp = time(NULL);
	}
	Block newBlock(index, transacData, chain.back().getHash(), txNum);
	chain.push_back(newBlock);
}

bool Blockchain::isValid(){
	vector<Block>::iterator it;
	size_t chainLen = chain.size();

	for(it=chain.begin(); it!=chain.end(); it++){
		Block currentBlock = *it;
		if(!currentBlock.isHashValid())
			return false;

		if(chainLen>1){
			Block prevBlock = *(it-1);
			if(currentBlock.getPrevHash()!=prevBlock.getHash())
				return false;
		}
	}

	return true;
}