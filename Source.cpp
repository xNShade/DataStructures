// Binary project for data structures
// Usage: binaryproject.exe <inputfilename>
//
//
// Programmer: Ryan Shoemake
//
// 6/14/2018
//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int *ReadData(ifstream &yourfile, int &Size);	// Reads in a file from the command line
void WriteData(int *dataarray,int Size);		// Writes to a maximum of 3 binary files. Information based off metadata

int main(int argc, char *argv[]) {

	// Error checking to make sure proper inputs.
	if (argc != 2) {
		cout << "You supplied the wrong number of arguements." << endl << "The correct usage is <filename.exe> <dataname>" << endl;
		system("pause");
		return 1;
	}
	else {
		string FileComingIn = argv[1];
		ifstream InFile(FileComingIn, ios::in | ios::binary);	//File grabbed from command line to send into functions.
		
		int Size;			//Declared outside of function so that it will be able to be passed into all functions
		int *AllMyData;	//Double pointer variable that will turn into dynamic double array. The zero column will contain metadata

		AllMyData = ReadData(InFile, Size);	//Returns double pointer in format mentioned previously.
		WriteData(AllMyData, Size);			//Writes the data to a max of 3 binary files. Written to as many files based off metadata
		//system("pause");

		delete [] AllMyData;
		//system("pause");
	}
	return 0;
}

int *ReadData(ifstream &yourfile, int &Size) {
	
	int MetaNumber;
	int *metaarray = new int[4];

	for (int i = 0; i < 4; i++) {
		yourfile.read(reinterpret_cast<char *>(&MetaNumber), sizeof(int));	//Reading the metadata from the binary file.
		metaarray[i] = MetaNumber;
	}

	Size = MetaNumber;	//Just to make it easier to interpret.

	int multiplier = 0;
	for (int i = 0; i < 3; i++) {				//This is checking the metadata and creating the array
		if (metaarray[i] == 1) { multiplier++; }
	}

	int *dataarray;	//This will be the variable that is returned.
	dataarray = new int[(Size * multiplier) + 4];
	//Now that we have dataarray size, we can assign metaarray into dataarray
	for (int i = 0; i < 4; i++) {
		dataarray[i] = metaarray[i];
	}

	int DataNumber;								//Placeholder for the information read in from the binary file.


	for (int j = 0; j < Size * multiplier; j++) {	//And stores into the dynamically created array
		yourfile.read(reinterpret_cast<char *>(&DataNumber), sizeof(int));
		dataarray[j + 4] = DataNumber;
		//cout << DataNumber;
	}
	yourfile.close();
	return dataarray;							//Returning the double pointer.
}

void WriteData(int *dataarray, int Size) {

	ofstream OutST;			//Creating an output variable to get ready to be assigned if needed
	ofstream OutSalinity;	// 
	ofstream OutSH;			// 
	int j = 0;

	int WriteNumber;
	bool doneOnce = false;
	for (int i = 0; i < 3; i++) {
		if(dataarray[i] == 1){			//Checks the metadata if there are values
			if(i == 0){ 
				OutST.open("surfaceTemp." + to_string(dataarray[3]), ios::out | ios::binary);
				for (; j < Size; j++) {
					WriteNumber = dataarray[j + 4];
					OutST.write(reinterpret_cast<const char *>(&WriteNumber), sizeof(int));
					//cout << WriteNumber << " ";
				}
				Size = Size + Size;
				doneOnce = true;
				OutST.close();
			}		//If there is a value for SurfaceTemp. Create the file
			if(i == 1){
				OutSalinity.open("salinity." + to_string(dataarray[3]), ios::out | ios::binary);
				for (; j < Size; j++) {
					if(doneOnce)
						WriteNumber = dataarray[j];
					else
						WriteNumber = dataarray[j + 4];
					OutSalinity.write(reinterpret_cast<const char *>(&WriteNumber), sizeof(int));
					//cout << WriteNumber << " ";
				}
				if (doneOnce) { Size = Size + (Size / 2);}
				else Size = Size + Size;
				doneOnce = true;
				OutSalinity.close();
			}		//Same for salinity
			if(i == 2){
				OutSH.open("surfaceHeight." + to_string(dataarray[3]), ios::out | ios::binary);
				for (; j < Size; j++) {
					if (doneOnce)
						WriteNumber = dataarray[j];
					else
						WriteNumber = dataarray[j + 4];
					OutSH.write(reinterpret_cast<const char *>(&WriteNumber), sizeof(int));
				}
				//doneOnce = true;
				OutSH.close();
			}		//Lastly for Surface Height
		}
	}
}