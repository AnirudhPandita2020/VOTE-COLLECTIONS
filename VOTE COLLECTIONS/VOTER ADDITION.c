#include "stdio.h"
#include "stdlib.h"
#include "string.h"
int count=0;
typedef struct S_voters {

	char name[10];
	int status;
	char vot[10];
	int age;
	char voter_id[10];
	//char gender[2];
	int serial;
	char dob[10];
	int houseno;
	char name3[10];
		struct S_voters*next;
	struct S_voters *previous;
	
} voters;
void PrintList(voters *start) {
	voters *cur = start;
	int count = 0;
	
	voters *ahead = NULL;
	voters *behind = NULL;
	
	while(cur != NULL) {
		count++;
		
		ahead = cur->next;
		behind = cur->previous;
		
		printf("ENTRY:%d Name:%s age:%d Voter_Id:%s status:%d Father name:%s Dob:%s Ahead:%s Behind:%s\n",count,
			cur->name,
			cur->age,cur->vot,cur->status,cur->name3,cur->dob,
			(ahead == NULL) ? "None" : ahead->name,
			(behind == NULL) ? "None" : behind->name
			);
		cur = cur->next;
		ahead = NULL;
		behind = NULL;
	}	
	printf("Total entry:%d\n",count);
}

voters *newvoter() {
	printf("Enter Name,age,voterid,house no,father name,dob: ");
	char input[50];
	fgets( input, 49, stdin);
	voters *new1=malloc(sizeof(voters));
	sscanf(input,"%s %d %s %d %s %s",new1->name,&new1->age,new1->vot,&new1->houseno,new1->name3,new1->dob);
	new1->status=0;
	new1->next = NULL;
	new1->previous = NULL;
	return new1;
}

voters *AddToStart(voters *startPtr) {
	voters *new1 = newvoter();
	if(startPtr != NULL) {
		startPtr->previous = new1;
		new1->next = startPtr;
	}
	
	return new1;
}

voters *AddToEnd(voters *startPtr) {
	voters *returnPtr = startPtr;
	voters *new1 = NULL;
	
	if(startPtr == NULL) {
		new1 = AddToStart(startPtr);
		returnPtr = new1;
	} else {
		voters *index = startPtr;
		while (index->next != NULL) {
			index = index->next;
		}
		new1 = newvoter();
		index->next = new1;
		new1->next = NULL;
		new1->previous = index;
	}
	return returnPtr;
}

voters *Delete(voters *startPtr) {

	printf("Enter Name To Delete:");
	char input[16];
	fgets(input, 15, stdin);
	
	voters *Ref = startPtr;
	voters *Delete = NULL;
	
	while(Ref != NULL) {
	
		if(strncmp(input, Ref->name, strlen(Ref->name)) == 0) {
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
void CleanUp(voters *start) {
	
	voters *freeMe = start;
	voters *holdMe = NULL;	
	while(freeMe != NULL) {
		holdMe = freeMe->next;
		printf("Free Name:%s age:%d\n",
			freeMe->name,
			freeMe->age);
		free(freeMe);
		freeMe = holdMe;
	}	
}

void WriteListToFile(voters *start) {
	FILE *pFile;
	pFile = fopen("electorallist1.txt", "wb");
	
	if(pFile != NULL) {
		voters *cur = start;
		
		voters *holdNext = NULL;
		voters *holdPrevious = NULL;
		
		while(cur != NULL) {
			holdNext = cur->next;
			holdPrevious = cur->previous;
			
			cur->next = NULL;
			cur->previous = NULL;
			
			fseek(pFile, 0, SEEK_END);
			fwrite(cur, sizeof(voters), 1, pFile);
			
			printf("Writing:%s to file\n",cur->name);
			
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

voters *ReadNextFromFile(voters *start, FILE *pFile) {
	size_t returnValue;
	if(start == NULL) {
		start = malloc(sizeof(voters));
		returnValue = fread(start, sizeof(voters), 1, pFile);
		start->next = NULL;
		start->previous = NULL;
	} else {
		voters *index = start;
		voters *new1 = malloc(sizeof(voters));
		while(index->next != NULL) {
			index = index->next;
		}
		returnValue = fread(new1, sizeof(voters), 1, pFile);
		index->next = new1;
		new1->next = NULL;
		new1->previous = index;
	}
	return start;
}

voters *ReadListIn(voters *start) {
	
	FILE *pFile;
	pFile = fopen("electorallist.txt", "r");
	if(pFile != NULL) {
	
		CleanUp(start);
		start = NULL;
		
		fseek(pFile, 0, SEEK_END);
		long fileSize = ftell(pFile);
		rewind(pFile);
		
		int numEntries = (int)(fileSize / (sizeof(voters)));
		printf("numEntries:%d\n",numEntries);
		
		int loop = 0;
		for(loop = 0; loop < numEntries; ++loop) {
			fseek(pFile, (sizeof(voters) * loop), SEEK_SET);
			start = ReadNextFromFile(start, pFile);
		}
	}  else {
		printf("FILE OPEN ERROR FOR READ\n");
	}
	
	return start;

}
void newreport(voters *start){
  //fp1=fopen("myList.txt","wb");
   voters *temp=start;
    while(temp!=NULL){
		if(temp->status==1)
		 WriteListToFile(temp);
		temp=temp->next;
	}
}
int main() {	
	
	char command[16];
	char input[16];	
	
	voters *start = NULL;
	
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
			} 
		}else if(strncmp(command,"new",3)==0)
				newreport(start);
				else if(strncmp(command,"delete",6)==0)
				 Delete(start);
		
			//PrintList(start);
		
	}
	
	CleanUp(start);
	
	return 0;

}
