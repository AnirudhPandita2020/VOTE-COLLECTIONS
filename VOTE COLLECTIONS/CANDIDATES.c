#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct S_admin {

	char Name[20];
	int serial_no;
	 char identity[5];
	 int total_vote;
	struct S_admin *next;
	struct S_admin *previous;
	
} admin;

void PrintList(admin *start) {
	admin *cur = start;
	int count = 0;
	
	admin *ahead = NULL;
	admin *behind = NULL;
	
	while(cur != NULL) {
		count++;
		
		ahead = cur->next;
		behind = cur->previous;
		
		printf("ENTRY:%d Name:%s Total vote:%d Identify:%s Serial NO:%d Ahead:%s Behind:%s\n",count,
			cur->Name,
			cur->total_vote,cur->identity,cur->serial_no,
			(ahead == NULL) ? "None" : ahead->Name,
			(behind == NULL) ? "None" : behind->Name
			);
		cur = cur->next;
		ahead = NULL;
		behind = NULL;
	}	
	printf("Total entry:%d\n",count);
}

admin *newParty() {

	printf("Enter Name And Identification sysmbol and serial np: ");
	char input[25];
	fgets( input, 24, stdin);
	
	admin *new1 = malloc(sizeof(admin));
	sscanf(input, "%s %s %d", new1->Name,new1->identity,&new1->serial_no);
	new1->total_vote=0;
	printf("Added:%s Total vote:%d\n\n",new1->Name,new1->total_vote);
	new1->next = NULL;
	new1->previous = NULL;
	return new1;
}

admin *AddToStart(admin *startPtr) {
	admin *new1 = newParty();
	if(startPtr != NULL) {
		startPtr->previous = new1;
		new1->next = startPtr;
	}
	
	return new1;
}

admin *AddToEnd(admin *startPtr) {
	admin *returnPtr = startPtr;
	admin *new1 = NULL;
	
	if(startPtr == NULL) {
		new1 = AddToStart(startPtr);
		returnPtr = new1;
	} else {
		admin *index = startPtr;
		while (index->next != NULL) {
			index = index->next;
		}
		new1 = newParty();
		index->next = new1;
		new1->next = NULL;
		new1->previous = index;
	}
	return returnPtr;
}

admin *Delete(admin *startPtr) {

	printf("Enter Name To Delete:");
	char input[16];
	fgets(input, 15, stdin);
	
	admin *Ref = startPtr;
	admin *Delete = NULL;
	
	while(Ref != NULL) {
	
		if(strncmp(input, Ref->Name, strlen(Ref->Name)) == 0) {
			Delete = Ref;
			break;
		}
		
		Ref = Ref->next;
	}
	
	if(startPtr != NULL && Delete == startPtr) {
		if(Delete->next != NULL) {
		 Delete->next->previous = NULL;
			startPtr = Delete->next;
		} else {
			startPtr = NULL;
		}
	} else {
		if(Delete != NULL) {
			if(Delete->previous != NULL) {
				Delete->previous->next = Delete->next;
			}
			
			if(Delete->next != NULL) {
				Delete->next->previous = Delete->previous;
			}
		}
	}
	
	if(Delete != NULL) {
		Delete = NULL;
		free(Delete);
	}
	
	return startPtr;
	
}

/*admin *Insert(admin *startPtr) {
	printf("Insert New Car After Car:");
	char input[16];
	fgets(input, 15, stdin);
	
	admin *afterCar = startPtr;
	admin *newCar = NULL;
	
	if(strncmp(input, afterCar->Party, strlen(afterCar->Party)) == 0) {
		newCar = newvoter();
	} else {
		while(afterCar->next != NULL) {
			afterCar = afterCar->next;
			if(strncmp(input, afterCar->Party, strlen(afterCar->Party)) == 0) {
				newCar = newvoter();
				break;
			}
		}
	}
	
	if(newCar != NULL) {
		newCar->next = afterCar->next;
		if(newCar->next != NULL) {
			newCar->next->previous = newCar;
		}
		afterCar->next = newCar;
		newCar->previous = afterCar;
	} else {
		printf("Car Not Found\n");
	}
	
	return startPtr;
}*/

void CleanUp(admin *start) {
	
	admin *freeMe = start;
	admin *holdMe = NULL;	
	while(freeMe != NULL) {
		holdMe = freeMe->next;
		printf("Free Name:%s Total vote:%d\n",
			freeMe->Name,
			freeMe->total_vote);
		free(freeMe);
		freeMe = holdMe;
	}	
}

void WriteListToFile(admin *start) {
	FILE *pFile;
	pFile = fopen("admin1.txt", "wb");
	
	if(pFile != NULL) {
		admin *cur = start;
		
		admin *holdNext = NULL;
		admin *holdPrevious = NULL;
		
		while(cur != NULL) {
			holdNext = cur->next;
			holdPrevious = cur->previous;
			
			cur->next = NULL;
			cur->previous = NULL;
			
			fseek(pFile, 0, SEEK_END);
			fwrite(cur, sizeof(admin), 1, pFile);
			
			printf("Writing:%s to file\n",cur->Name);
			
			cur->next = holdNext;
			cur->previous = holdPrevious;
			
			holdNext = NULL;
			holdPrevious = NULL;
			
			cur = cur->next;
		}
		fclose(pFile);
		pFile = NULL;
	} else {
		printf("FILE OPEN ERROR\n");
	}
	
}

admin *ReadNextFromFile(admin *start, FILE *pFile) {
	size_t returnValue;
	if(start == NULL) {
		start = malloc(sizeof(admin));
		returnValue = fread(start, sizeof(admin), 1, pFile);
		start->next = NULL;
		start->previous = NULL;
	} else {
		admin *index = start;
		admin *new1 = malloc(sizeof(admin));
		while(index->next != NULL) {
			index = index->next;
		}
		returnValue = fread(new1, sizeof(admin), 1, pFile);
		index->next = new1;
		new1->next = NULL;
		new1->previous = index;
	}
	return start;
}

admin *ReadListIn(admin *start) {
	
	FILE *pFile;
	pFile = fopen("admin.txt", "r");
	if(pFile != NULL) {
	
		CleanUp(start);
		start = NULL;
		
		fseek(pFile, 0, SEEK_END);
		long fileSize = ftell(pFile);
		rewind(pFile);
		
		int numEntries = (int)(fileSize / (sizeof(admin)));
		printf("numEntries:%d\n",numEntries);
		
		int loop = 0;
		for(loop = 0; loop < numEntries; ++loop) {
			fseek(pFile, (sizeof(admin) * loop), SEEK_SET);
			start = ReadNextFromFile(start, pFile);
		}
	}  else {
		printf("FILE OPEN ERROR FOR READ\n");
	}
	
	return start;

}

int main() {	
	
	char command[16];
	char input[16];	
	
	admin *start = NULL;
	
	while( fgets( input, 15, stdin) ) {
		
		sscanf(input,"%s",command);	
		
		if ( strncmp(command, "quit", 4) == 0) {
			printf("\n\nBreaking...\n");
			break;		
		} else if ( strncmp(command, "print", 5) == 0) {
			PrintList(start);
		} else if ( strncmp(command, "write", 5) == 0) {
			WriteListToFile(start);
		} else if ( strncmp(command, "read", 4) == 0) {
			start = ReadListIn(start);
		} else if ( strncmp(command, "addstart", 8) == 0) {
			start = AddToStart(start);
			PrintList(start);
		} else if ( strncmp(command, "add", 3) == 0) {
			start = AddToEnd(start);
			PrintList(start);
		} else if ( strncmp(command, "insert", 6) == 0) {
			if(start == NULL) {
				start = AddToStart(start);
			} else {
				//start = Insert(start);
			}
		}
			else if(strncmp(command,"delete",6)==0)
			 Delete(start);
		//PrintList(start);
		
	}
	
	CleanUp(start);
	
	return 0;

}
