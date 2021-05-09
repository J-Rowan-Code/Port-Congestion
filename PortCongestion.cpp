#include<cstdlib>
#include<iostream>
#include<fstream>
#include<locale>
#include<sstream>
#include<string>
using namespace std;
#define TIMEDELAY 3
#define N 128

struct Node{
	int data;
	Node *next;
};

class Queue{
	private:
		Node *front, *rear;
		int counter;
	public:
		Queue();
		~Queue();
		void join(int num1);
		void leave();
		int frontq();
		int length();
		bool isEmpty();
};

void init_sim();

//Global declarations here
int congestionSize[N], numberOfPorts=0, sumOfInputPackets=0;
Queue input[N], output[N];


int main(int argc, char **argv){
	string expression, tempstr;
	int portNumber=-1, destination=0;
	
	ifstream inputFile;
	if(argc!=2){
		cout << "Type a file name. " << endl << argv[1] << endl;
		exit(0);
		//Closing the program so the user can open again with the file name
	}
	inputFile.open(argv[1]);
	//inputFile.open("simulation4ports.txt");
	if(inputFile.is_open()==false){
		cout << "Could not read file: " << endl << argv[1] << endl;
		exit(1);
	}
	
	while(!inputFile.eof()){
		getline(inputFile, expression);
		stringstream line(expression); //Turns the thing into a line? adds \0?
		if(inputFile.eof()) break;
		if(expression[0] == '#') continue;
		if(expression[0] == 'P'){
			getline(line, tempstr, ' '); //Breaks string based off of the ' ' character (space)
			getline(line, tempstr, ' ');
			numberOfPorts = atoi(tempstr.c_str());
			continue;
		}
		portNumber++;
		while(getline(line, tempstr, ' ')){
			destination = atoi(tempstr.c_str());
			if(destination < 0 || destination > numberOfPorts || numberOfPorts < portNumber) {
				cout << "Error in the format of the text file" << endl;
				exit(3);
			}
			input[portNumber].join(destination);
			sumOfInputPackets++;
		}
	}
	
	//All inputs join and leave successfully.
	init_sim();
	int indx, eCount, temp, portSum=0, portMax=0;//, potato=0;
	unsigned long int clock=0;
	portNumber=0;
	
	while(true){
		eCount=0;
		portSum=0;
		for(indx=0; indx<numberOfPorts; indx++){
			if(input[indx].isEmpty()){
				eCount++;
			}
		}
		if(eCount==numberOfPorts){
			break;
		}
		if(input[portNumber].isEmpty()){
			portNumber++;
			clock++;
			//continue; Continue bad?
		}
		if(!input[portNumber].isEmpty()){
			temp = input[portNumber].frontq();
			output[temp-1].join(temp);
			input[portNumber].leave();
			portNumber++;
			clock++;
		}
		if(portNumber >= numberOfPorts){
			portNumber=0;
		}
		if((clock % (TIMEDELAY*numberOfPorts) == 0) && clock!=0){
			for(indx=0; indx<numberOfPorts; indx++){
				if(!output[indx].isEmpty()){
					output[indx].leave();
				}
			}
		}
		for(indx=0; indx<numberOfPorts; indx++){
			if(!output[indx].isEmpty()){
				portSum = portSum + output[indx].length();
			}
		}
		if (portSum>portMax){
			portMax=portSum;
			for(indx=0; indx<numberOfPorts; indx++){
				if(!output[indx].isEmpty()){
					congestionSize[indx] = output[indx].length();
				}else{
					congestionSize[indx]=0;
				}
			}
		}
	}
	
	for(indx=0; indx<numberOfPorts; indx++){
		cout << "output port " << indx+1 << ": " << congestionSize[indx] << " packets" << endl;
	}
}


void init_sim(){
	int i;
	for (i = 0; i<N; i++){
		congestionSize[i] = 0;
	}
}

Queue::Queue(){
	front=NULL;
	rear=NULL;
	counter = 0;
}

Queue::~Queue(){
	return;
}

void Queue::join(int num1){
	Node *temp;
	temp = new Node;
	temp->data = num1;
	temp->next = NULL;
	if (rear!=NULL){
		rear->next = temp;
	}
	rear = temp;
	if (front == NULL){
		front = temp;
	}
	counter++;
}

void Queue::leave(){
	Node *temp;
	if(front == NULL){
		return;
	}
	temp = front;
	front = front-> next;
	if(front == NULL){
		rear = NULL;
	}
	delete temp;
	counter--;
}

int Queue::frontq(){
	return front->data;
}

int Queue::length(){
	return counter;
}

bool Queue::isEmpty(){
	if(front == NULL){
		return true;
	}
	return false;
}
