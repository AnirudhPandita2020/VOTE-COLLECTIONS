#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#include<time.h>
clock_t t=0,t1;
char str[10], dt[10], vt[10];
int enadis;
typedef struct S_voters
{

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
	struct S_voters *next;
	struct S_voters *previous;

} voters;
voters *start = NULL;

typedef struct S_admin
{

	char Name[20];
	int serial_no;
	char identity[5];
	int total_vote;
	struct S_admin *next;
	struct S_admin *previous;

} admin;
admin *start1 = NULL;
void PrintList1(voters *start)
{
	system("cls");
	printf("************************************************************************************************************************\n");
	voters *cur = start;
	int count = 0;

	voters *ahead = NULL;
	voters *behind = NULL;

	while (cur != NULL)
	{
		count++;

		ahead = cur->next;
		behind = cur->previous;

		printf("ENTRY:%d\n Name:%s\n age:%d\n Voter_Id:%s\n Voted:%s Father name:%s Dob:%s\n", count,
			   cur->name,
			   cur->age, cur->vot, ((cur->status) == 1) ? "YES" : "NO", cur->name3, cur->dob);
		cur = cur->next;
		ahead = NULL;
		behind = NULL;
	}
	sleep(5);
	//printf("Total entry:%d\n", count);
}
voters *ReadNextFromFile(voters *start, FILE *pFile)
{
	size_t returnValue;
	if (start == NULL)
	{
		start = malloc(sizeof(voters));
		returnValue = fread(start, sizeof(voters), 1, pFile);
		start->next = NULL;
		start->previous = NULL;
	}
	else
	{
		voters *index = start;
		voters *new1 = malloc(sizeof(voters));
		while (index->next != NULL)
		{
			index = index->next;
		}
		returnValue = fread(new1, sizeof(voters), 1, pFile);
		index->next = new1;
		new1->next = NULL;
		new1->previous = index;
	}
	return start;
}
voters *ReadListIn(voters *start)
{

	FILE *pFile;
	pFile = fopen("electorallist1.txt", "r");
	if (pFile != NULL)
	{

		start = NULL;

		fseek(pFile, 0, SEEK_END);
		long fileSize = ftell(pFile);
		rewind(pFile);

		int numEntries = (int)(fileSize / (sizeof(voters)));

		int loop = 0;
		for (loop = 0; loop < numEntries; ++loop)
		{
			fseek(pFile, (sizeof(voters) * loop), SEEK_SET);
			start = ReadNextFromFile(start, pFile);
		}
	}
	else
	{
		printf("FILE OPEN ERROR FOR READ\n");
	}

	return start;
}
void WriteListToFile(voters *start)
{
	FILE *pFile;
	pFile = fopen("electorallist1.txt", "wb");

	if (pFile != NULL)
	{
		voters *cur = start;

		voters *holdNext = NULL;
		voters *holdPrevious = NULL;

		while (cur != NULL)
		{
			holdNext = cur->next;
			holdPrevious = cur->previous;

			cur->next = NULL;
			cur->previous = NULL;

			fseek(pFile, 0, SEEK_END);
			fwrite(cur, sizeof(voters), 1, pFile);

			cur->next = holdNext;
			cur->previous = holdPrevious;

			holdNext = NULL;
			holdPrevious = NULL;

			cur = cur->next;
		}
		fclose(pFile);
		pFile = NULL;
	}
	else
	{
		printf("FILE OPEN ERROR\n");
	}
}
void WriteListToFile1(admin *start1)
{
	FILE *pFile;
	pFile = fopen("admin1.txt", "wb");

	if (pFile != NULL)
	{
		admin *cur = start1;

		admin *holdNext = NULL;
		admin *holdPrevious = NULL;

		while (cur != NULL)
		{
			holdNext = cur->next;
			holdPrevious = cur->previous;

			cur->next = NULL;
			cur->previous = NULL;

			fseek(pFile, 0, SEEK_END);
			fwrite(cur, sizeof(admin), 1, pFile);

			cur->next = holdNext;
			cur->previous = holdPrevious;

			holdNext = NULL;
			holdPrevious = NULL;

			cur = cur->next;
		}
		fclose(pFile);
		pFile = NULL;
	}
	else
	{
		printf("FILE OPEN ERROR\n");
	}
}
admin *ReadNextFromFile1(admin *start1, FILE *pFile)
{
	size_t returnValue;
	if (start1 == NULL)
	{
		start1 = malloc(sizeof(admin));
		returnValue = fread(start1, sizeof(admin), 1, pFile);
		start1->next = NULL;
		start1->previous = NULL;
	}
	else
	{
		admin *index = start1;
		admin *new1 = malloc(sizeof(admin));
		while (index->next != NULL)
		{
			index = index->next;
		}
		returnValue = fread(new1, sizeof(admin), 1, pFile);
		index->next = new1;
		new1->next = NULL;
		new1->previous = index;
	}
	return start1;
}

admin *ReadListIn1(admin *start1)
{

	FILE *pFile;
	pFile = fopen("admin1.txt", "r");
	if (pFile != NULL)
	{

		//CleanUp(start);
		start1 = NULL;

		fseek(pFile, 0, SEEK_END);
		long fileSize = ftell(pFile);
		rewind(pFile);

		int numEntries = (int)(fileSize / (sizeof(admin)));

		int loop = 0;
		for (loop = 0; loop < numEntries; ++loop)
		{
			fseek(pFile, (sizeof(admin) * loop), SEEK_SET);
			start1 = ReadNextFromFile1(start1, pFile);
		}
	}
	else
	{
		printf("FILE OPEN ERROR FOR READ\n");
	}

	return start1;
}
void readena()
{
	FILE *pfile;
	pfile = fopen("enabledisable.txt", "r");
	enadis = getw(pfile);
	fclose(pfile);
}
void chenadis()
{
	FILE *pfile;
	pfile = fopen("enabledisable.txt", "wb+");
	if (enadis == 0)
	{
		printf("\n");
		printf("\t\t\t\t          ENABLING VOTE PROCESS.PLEASE WAIT...\n");
		enadis = 1;
		putw(enadis, pfile);
	}
	else
	{
		printf("\n");
		printf("\t\t\t\t          DISABLING PROCESS.PLEASE WAIT....\n");
		enadis = 0;
		putw(enadis, pfile);
	}
	fclose(pfile);
	sleep(3);
}
void PrintList(admin *start1)
{
	printf("\t\t\t\t       \t'->Candidates Taking Part<-'\n");
	printf("\n");
	admin *cur = start1;

	admin *ahead = NULL;
	admin *behind = NULL;
	while (cur != NULL)
	{

		ahead = cur->next;
		behind = cur->previous;
		printf("\t\t\t\t\tNo:%d Name:%s  Identify:%s  \t\n", cur->serial_no,
			   cur->Name,
			   cur->identity);
		printf("\n");

		cur = cur->next;
		ahead = NULL;
		behind = NULL;
	}
}
int search(char *x)
{
	voters *temp = start;
	while (temp != NULL)
	{
		if ((strcmp((temp->dob), x) == 0) && (2020 - atoi((temp->dob) + 6) == (temp->age)))
			return 1;
		temp = temp->next;
	}
	if (temp == NULL)
		return 0;
}
int search2(char *x)
{
	voters *temp = start;
	while (temp != NULL)
	{
		if (strcmp((temp->name3), x) == 0)
			return 1;
		temp = temp->next;
	}
	if (temp == NULL)
		return 0;
}
int search3(char *x)
{
	voters *temp = start;
	while (temp != NULL)
	{
		if (strcmp((temp->vot), x) == 0)
			return 1;
		temp = temp->next;
	}
	if (temp == NULL)
		return 0;
}
void update(char *x)
{
	int t;
	voters *temp = start;
	while (temp != NULL)
	{
		if (strcmp((temp->vot), x) == 0)
		{
			t = temp->status;
			t = 1;
			temp->status = t;
		}
		temp = temp->next;
	}
}
int check_status(char *x)
{
	voters *temp = start;
	while (temp != NULL)
	{
		if ((strcmp((temp->vot), x)) == 0)
		{
			if ((temp->status) == 1)
				return 0;
			else
				return 1;
		}
		temp = temp->next;
	}
}
void collect(int c)
{
	int i;
	admin *temp = start1;
	temp = start1;
	while (temp != NULL)
	{
		if ((temp->serial_no) == c)
		{
			i = temp->total_vote;
			i += 1;
			temp->total_vote = i;
		}
		temp = temp->next;
	}
}
void welcome(char *name)
{
	system("cls");
	voters *temp = start;
	while (temp != NULL)
	{
		if (strcmp((temp->vot), name) == 0)
			break;
		temp = temp->next;
	}
	printf("************************************************************************************************************************\n");
	printf("\n\t\t\t\t\tWELCOME MR/MRS %s.\n", temp->name);
	printf("\n\t\t\t\tA timer of 10 sec will be provided to caste your vote,\n");
	printf("\n\t\t\t\tFailing which the vote will be punched against 'NOTA'.\n");
	printf("\n\t\t\t\tPLEASE WAIT....\n");
	printf("\n\t\t\t\t.HAPPY VOTEING.");
	sleep(6);
	return;
}
void display()
{
	int count = 10;
	int op = 13;
	system("cls");
	printf("************************************************************************************************************************\n");
	printf("\t\t\t\t\t:VERIFICATION BEFORE COMMENCING:\n");
	printf("\n");
	printf("\t\t\t\tEnter Date of Birth as given in your voter id:");
	scanf("%s", dt);
	printf("\n");
	if (search(dt) == 1)
	{
		printf("\t\t\t\t\tVERFICATION LEVEL 1 SUCCESS.\n");
		printf("\n");
		printf("\t\t\t\tEnter Father/Husband Name:");
		scanf("%s", str);
		if (search2(str) == 1)
		{
			printf("\n");
			printf("\t\t\t\t\tVERIFICATION LEVEL 2 SUCCESS.\n");
			printf("\n");
			printf("\t\t\t\t\t Enter Voter Id:");
			scanf("%s", vt);
			if (search3(vt) == 1)
			{
				if (check_status(vt) != 0)
				{
					welcome(vt);
					while (!kbhit() || scanf("%d", &op))
					{
						system("cls");
						printf("************************************************************************************************************************\n");
						printf("Time left:%d\n", count);
						PrintList(start1);
						printf("\n");
						printf("Punch the serial no against the candiates name to cast vote:");
						if (op <= 12)
							break;
						if (count == 0)
						{
							printf("\n\t\tNo activity has been detected.DEFAULT VOTE IS PUCNHED AGAINST NOTA.\n");
							op = 12;
							break;
						}
						sleep(1);
						count--;
					}

					collect(op);
					printf("Vote registerd.\n");
					WriteListToFile1(start1);
				}
			}
			else
				printf("\t\t\tNO DATA.Data not matching.Consult your Election officer.\n");
		}
		else
			printf("\t\t\tNO DATA.Data not matching.Consult your Election officer.\n");
	}
	else
	{
		printf("\t\t\t\t ERROR.EITHER THE VOTE HAS BEEN PUCNHED OR\n");
		printf("\t\t\t\t DATA IS NOT MATCHING WITH THE RECORDS.\n");
	}
	update(vt);
	printf("\t\t\t\t ERROR.EITHER THE VOTE HAS BEEN PUCNHED OR\n");
		printf("\t\t\t\t DATA IS NOT MATCHING WITH THE RECORDS.\n");
	WriteListToFile(start);
	
	printf("\t\t\tPLEASE WAIT WHILE WE PROCESS THE DATA.YOU WILL BE DIVERTED TO THE MAIN MENU");
	sleep(4);
	return;
}
void displaystats()
{
	int op;
	system("cls");
	admin *temp=start1;
	while(temp!=NULL){
			printf("\t\t\t\t\tNo:%d Name:%s  Identify:%s  TOTAL VOTES:%d\n", temp->serial_no,
			   temp->Name,
			   temp->identity,temp->total_vote);
		printf("\n");
		temp=temp->next;
	}
	printf("Press zero to exit:");
	 scanf("%d",&op);
	  if(op==0){
	   return ;
}
	 
}

void electionresults()
{
	int count = 0;
	int max = 0, op;
	if (enadis == 0)
	{
		admin *temp = start1;
		while (temp != NULL)
		{
			if (max < (temp->total_vote))
				max = temp->total_vote;
			temp = temp->next;
		}
		temp = start1;
		while (temp != NULL)
		{
			if (max == (temp->total_vote))
				count++;
			temp = temp->next;
		}
		if (count > 1)
		{
			printf("RESULTS COULDNT BE DECLARED.\n");
			sleep(5);
			return;
		}
		temp = start1;
		while (temp != NULL)
		{
			if (max == (temp->total_vote) && strcmp((temp->Name), "NOTA") != 0)
				printf("\n\t\t\t\tELECTION WON BY:%s Party:%s Total votes collected:%d \n", temp->Name, temp->identity, temp->total_vote);
			else if (max == (temp->total_vote) && (strcmp((temp->Name), "NOTA") == 0))
				printf("\n\t\t\t\t\tRESULTS COULDNT BE DECLARED.\n");
			temp = temp->next;
		}

		WriteListToFile1(start1);
	}
	else
		printf("Please wait for the Vote collection .\n");
	printf("\t\t\t\t\t\tPRESS 0 TO EXIT:");
	scanf("%d", &op);
	if (op == 0)
		return;
}
void controlpanel()
{
	int op;
	system("cls");
	while (op!=5)
	{
		system("cls");
		printf("************************************************************************************************************************\n");
		printf("\t\t\t\t\t\t:ADMIN CONTROL PANEL:\n");
		printf("\n");
		printf("\t\t\t\t\t\t1.Display Stats.\n");
		printf("\n");
		printf("\t\t\t\t\t\t2.Election Results.\n");
		printf("\n");
		printf("\t\t\t\t\t\t3.Enable/Disable VOTE COLLECTION PROCESS.\n");
		printf("\n");
		printf("\t\t\t\t\t\t4.Updated ELECTRORAL ROLL.\n");
		printf("\n");
		printf("\t\t\t\t\t\tEnter Option:");
		scanf("%d",&op);
		switch (op)
		{
		case 1:
			displaystats();
			break;
		case 2:
			electionresults();
			break;
		case 3:
			chenadis();
			break;
		case 4:
			PrintList1(start);
			break;
			 case 5:return;break;
		}
	}
}
void intro()
{
	printf("************************************************************************************************************************\n");
	printf("\n");
	printf("\t\t\t\t\t:NEW HORIZON COLLEGE OF ENGINEERING:\n");
	printf("\n");
	printf("\t\t\t\tName of the mini Project:DIGITAL VOTE COLLECTION\n");
	printf("\n");
	printf("\t\t\t\t\t      Name:ANIRUDH PANDITA\n");
	printf("\n");
	printf("\t\t\t\t\t\tUSN:1NH19CS018\n");
	printf("\n");
	printf("\t\t\t\t\t\t  Section:3 A\n");
	printf("\n");
	printf("\t\t\t\tPROGRAM IS GETTING LOADED .PLEASE WAIT.....");
	sleep(5);
}
int main()
{
	readena();
	//intro();
	start = ReadListIn(start);
	start1 = ReadListIn1(start1);
	char command[16];

	while (1)
	{
		system("cls");
		printf("************************************************************************************************************************\n");
		printf("\t\t\t\t\t    ->DIGITAL VOTE COLLECTION<-\n");
		printf("\n");

		printf("\n");
		printf("\t\t\t\t\t\t  :Commands:\n");
		printf("\t\t\t\t 'VOTE' -> To begin the VERIFICATION PROCESS.\n");
		printf("\n");
		printf("\t\t\t\t 'QUIT' -> To exit.\n");
		printf("\n");
		printf("\t\t\t\t  Enter Command:");

		scanf("%s", command);
		if ((strcmp(command, "VOTE") == 0) && enadis == 1)
		{
			display();
		}
		else if ((strcmp(command, "VOTE") == 0) && enadis == 0)
		{
			printf("\n\n\t\t\t\t  ->'Please wait for voting to commence'<-\n");
			sleep(3);
		}
		else if (strcmp(command, "QUIT") == 0)
			break;
		else if (strcmp(command, "MASTERACCESS") == 0)
		{
			controlpanel();
		}
	}
	return 0;
}
