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
	//Starting kureChain by initialising a new instance of a blockchain
	system("clear");
	cout<<"Starting..."<<endl;
	Blockchain kureChain;
	TxData mempool[16];
	unsigned short txNum = 0;
	char choice = '\0';

	cout<<"Welcome"<<endl;

	while (true)
	{
		cout<<"\nMenu:\n0-Exit\t1-New Tx\t2-Write Tx to chain"<<endl;
		cin>>choice;

		switch(choice){
			case '0':
				return EXIT_SUCCESS;
				break;
			case '1':
				newTx(mempool, &txNum);
				break;
			case '2':
				kureChain.addBlock(mempool, txNum);
				break;
			default:
				system("clear");
				cout<<"Invalid input, try again!"<<endl;
				break;
		}
	}
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
	}while(fHash/100000000000000!=0);

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