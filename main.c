#include <stdio.h>
#include <stdlib.h> // for miscellaneous functions
#include <conio.h> // for screen function i.e. clearing screen
#include <string.h> // for string functions
char option;
struct  bookingdate              // for including date
{
	int day,month,year;
};
// function prototypes
void cafe(); // show cafe menu + order items from menu
int cost(int); // return item's price
int inventory(int, int); // manage cafe stock details
void cafeReceipt(int); // print cafe bill
void Rooms(); // display types of room available
void roomBooking(); // customer info + booking customer's room
int amount(int); // rent per night
int availability(int, int); // check if required room is available or not + update accordingly
void checkout(); // update number of free rooms
void DayEnd(); // give access to administrative features

int main()
{
	do
	{
		system("cls");
		int choice;
		printf("\n---------------------------------------------------\n");
		printf("------------WELCOME TO HOLIDAY-INN HOTELS------------");
		printf("\n---------------------------------------------------\n");
		// diplaying the main options of the programs that will lead to functions
		printf("\n\n\t If you are famished, visit 'ECLECTIC TASTE' by Pressing 1.");
		printf("\n\n\t If you feel like resting, head over to 'Holiday-Inn Rooms' by Pressing 2.");
		printf("\n\n\t If you are checking out of the room then press 3.");
		printf("\n\n\t If the day has ended then press 4.\n\n");
		scanf("%d",&choice); // selecting user's choice
		while (choice!=1&&choice!=2&&choice!=3&&choice!=4)
		{
			printf("\nIncorrect choice is selected.Kindly select a valid choice.\n");
			scanf("%d",&choice);
		}
		switch (choice)
		{
			case 1:cafe();break;
			case 2:Rooms();break;
			case 3:checkout();break;
			case 4:DayEnd();break;
		};
	}while(option=='N'|| option =='n');
	if (option=='Y'|| option == 'y')
	{
		DayEnd();
	}
	return 0;
}

void cafe()
{
    system("cls");
    printf("\n-------------- WELCOME TO ECLECTIC TASTE ---------------\n");
    FILE *fp; // to call file containing the menu
    int c, more, bill=0, price=0, check=0, item;
    fp = fopen("Menu.txt","r");
    while(1) //printing menu
    {
      c = fgetc(fp);
      if( feof(fp) )
        {
         break;
        }
      printf("%c", c);
    }
    fclose(fp);
     //taking order
	do
    {
       printf("\n\nEnter the serial number of item you want to order:\n");
        do
        {
            scanf("%d",&item);
            if (item<1 || item>15)
            {
                printf("Invalid Order! Please re-enter your order.\n");
                check=1;
            }
            else
            {
                check=0;
                break;
            }
        } while (check=1);
        price= cost(item); //to get the price of item ordered
        inventory(item,0); //updating inventory
        bill= bill+price; //calculating total bill
        printf("\nAre there more items to be ordered?\nPress Y for 'yes' and N for 'no'");
        more= getch();
    } while (more == 'Y' || more == 'y');
    cafeReceipt(bill); //printing the receipt
    printf("Has the day ended?\n Press 'Y/y' for Yes. \t Press 'N/n' for No.\n");
    option=getch();
}

int cost (int menu)
{
    //array containing price of each item
    int price[]={250,500,150,75,50,300,250,250,200,300,100,100,100,90,100};
    return price[menu-1];
}

int inventory(int item, int end)
{
    int index=item-1; // adjusting input according to index of the array 
    char separator = '=';// to separate the item name and it's quantity
    FILE *prodFile= fopen ("inventory1.txt","r");
	char fileLine[200];
    int  fileLineLen;
    char name[200];
    char quantity[200];
    int itemsInFile;
	char prodName[15][200];
    int prodQty[15];
	int i=0,k=0;
	fgets(fileLine,200,prodFile);
    //checking if the file is empty
    if (feof(prodFile))
    {
        printf ("No data remaining in the file. Exiting...");
        return 1;
    }
   //taking input from file into an array
    while(!feof(prodFile))
    {
        fileLineLen = strlen(fileLine);
        i=0;
        while(fileLine[i] != separator)
        {
            name[i]=fileLine[i];//taking name as input
            i++;
        }
        name[i]='\0';
        int j=0;i++;
        while (i<fileLineLen)
        {
           quantity[j++]=fileLine[i++]; //taking quantity as input
        }
        quantity[j]='\0';

        prodQty[k] = atoi(quantity); //converting the quantity from string to integer and putting it in an array
        strcpy(prodName[k],name); //putting the name into an array
        fgets(fileLine,200,prodFile);
        k++;
    }
    itemsInFile=k+1;
    fclose(prodFile);

    if (end==0) //the day has not come to an end
    {
        prodQty[index] = prodQty[index]-1;//reducing the quantity of the item by 1
        prodFile= fopen ("inventory1.txt","w+");

        for (k=0;k<=itemsInFile;k++)
        {
            //updating the quantity in the inventory
            fprintf(prodFile, "%s=%d\n" , prodName[k],prodQty[k]);
        }
    }
    else if (end==1) //when the day ends
    {
        prodFile= fopen ("inventory1.txt","r");
        printf("S.#\t Item Name\t\tQuantity\n\n");
        for (k=0;k<itemsInFile;k++)
        {
            //printing the inventory
            printf ("%d\t%s\t%d\n",k+1,prodName[k],prodQty[k]);
        }
    }
    fclose(prodFile);
    return 0;
}

void cafeReceipt(int bill)
{
    system("cls");
    printf("\n-------------------------------------------------\n");
    printf("\t-----YOUR ECLECTIC TASTE RECEIPT-----\n");
    printf("\t Your total bill is PKR %d .\n\t Please pay the bill at the counter.\n\t\t Thank You For Dining!",bill);
    printf("\n--------------------------------------------------\n");
}

void Rooms()
{
    system("cls");
    printf("\n----------WELCOME TO HOLIDAY-INN ROOMS----------\n");
    FILE *froom;
    char c;
    froom= fopen("rooms.txt","r");
    while(1) //printing the type of rooms offered by the Hotel
    {
      c = fgetc(froom);
      if( feof(froom) )
        {
         break;
        }
      printf("%c", c);
    }
    fclose(froom);
    roomBooking(); //booking the room as per the customers requirement
 }

void roomBooking()
{
	struct bookingdate d;
    int price,type,roomrent,check=0,days;
    char customername[30];
    printf("Booking a room.\n");
    printf("\nPlease enter information of the Customer:\n");
    //taking input customer information for booking of room
    FILE *fp;
	fp = fopen("customer.txt","a+");
    printf("Enter your name.\n");
	scanf("%s",&customername);
	printf("Enter today's date.");
	scanf("%d %d %d",&d.day,&d.month,&d.year);
	printf("Enter the number of days you want to reside in the hotel's room.\n");
	scanf("%d",&days);
	printf("Enter the serial number corresponding to the type of room you want to occupy.\n");
	scanf("%d",&type);
	while (type<1 || type>3)//checking if the type is available
    {
    	printf("There is no room available of this type.\nKindly re-enter your selection.\n");
		scanf("%d",&type);
	}
    check=availability(type,0); //checking if a room of that type is available
    while (check==1)
    {
    	printf("We are sorry, but there are no free rooms available of this type at the moment.\n");
		printf("Please choose another room type.\n");
		scanf("%d",&type);
		check=availability(type,0);
    }
    price = amount(type); //getting price of the selected room
    roomrent = days * price; //calculating total room rent
    //saving all the information in a file
    fprintf(fp,"%s: %s\n%s: %d\n%s %d\n\n","Name",customername,"Number of Days",days,"Total Room Rent",roomrent);
    getch();
	system("cls");
    //printing booking details
    printf("--------------------------------------------------\n");
    printf("\tYour booking is successful!\n\n");
    printf("\tName: ");
    puts(customername);
    printf("\tNo. of Days for which room is alloted: %d starting from : %d/%d/%d",days,d.day,d.month,d.year);
    printf("\n\tPrice per day is: PKR %d/-", price);
    printf ("\n\tYour Total Amount for stay is: PKR %d/-\n\n", roomrent);
    printf("\tEnjoy your stay at HOLIDAY-INN Rooms!");
    printf("\n-------------------------------------------------\n");
    printf("Has the day ended?\n Press 'Y/y' for Yes. \t Press 'N/n' for No.\n");
	option=getch();
}

int amount(int choice)
{
    int rent;
    switch(choice)
    {
        case 1: rent = 10000; break;
        case 2: rent = 15000; break;
        case 3: rent = 20000; break;
    };
    return rent;
}

int availability(int type, int end)
{
    int index=type-1;// adjusting input according to index of the array 
    char separator = '='; // to separate the room type and available rooms
   FILE *prodFile= fopen ("availableroom.txt","r");
	char fileLine[200];
    int  fileLineLen;
    char typeroom[200];
    char quantity[200];
    int itemsInFile;
	char roomType[3][200];
    int NumberOfRooms[3];
	int i=0,k=0;
	fgets(fileLine,200,prodFile);
    //checking if the file is empty
    if (feof(prodFile))
    {
        printf ("No data remaining in the file. Exiting...");
        return 1;
    }
    while(!feof(prodFile))
    {
        fileLineLen = strlen(fileLine);
        i=0;
        while(fileLine[i] != separator)
        {
            typeroom[i]=fileLine[i]; //taking room type as input
            i++;
        }
        typeroom[i]='\0';
        int j=0;i++;
        while (i<fileLineLen)
        {
            quantity[j++]=fileLine[i++];// taking available rooms as input
        }
        quantity[j]='\0';

        NumberOfRooms[k] = atoi(quantity); //converting the quantity from string to integer and putting it in an array
        strcpy(roomType[k],typeroom); //storing type of room in an array
        fgets(fileLine,200,prodFile);
        k++;
    }
    itemsInFile=k;
    fclose(prodFile);
	if (end==0)   // day has not come to an end
    {
	if (NumberOfRooms[index]!=0)
        {
			NumberOfRooms[index] = NumberOfRooms[index]-1;//decreasing the number of available rooms by 1
            prodFile= fopen ("availableroom.txt","w+");
			for (k=0;k<itemsInFile;k++)
            {
                //updating the list of available rooms for the customers
                fprintf(prodFile, "%s = %d\n" , roomType[k],NumberOfRooms[k]);
            }
            fclose(prodFile);
            return 0;
        }
        else
        {
            return 1;//if the preferred room is not available
        }
    }
    else if (end==1)    // day has ended
    {
        prodFile= fopen ("availableroom.txt","r");
        printf("\n%12s %-20s %10s\n","S.No","Room Type","Available Rooms");
        printf("%12s %-20s %10s\n","----","---------","---------------");

        for (k=0;k<itemsInFile;k++)
        {
			printf ("%12d %-20s %10d\n",k+1,roomType[k],NumberOfRooms[k]);
        }
    }
    fclose(prodFile);
    return 0;
}

void checkout()
{
    int index, type;
    char separator = '=';
	FILE *prodFile= fopen ("availableroom.txt","r");
	char fileLine[200],typeroom[200],quantity[200];
    int  fileLineLen,itemsInFile;
	char roomType[3][256];
    int NumberOfRooms[3];
	int i=0,k=0;
	fgets(fileLine,200,prodFile);
   if (feof(prodFile)) //checking if the file is empty
    {
        printf ("No data remaining in the file. Exiting...");
        return;
    }
	while(!feof(prodFile))
    {
        fileLineLen = strlen(fileLine);
        i=0;
        while(fileLine[i] != separator)
        {
            typeroom[i]=fileLine[i]; //taking room type as input
            i++;
        }
        typeroom[i]='\0';
        int j=0;i++;
        while (i<fileLineLen)
        {
            quantity[j++]=fileLine[i++];//taking available rooms as input
        }
        quantity[j]='\0';

        NumberOfRooms[k] = atoi(quantity); //converting the quantity from string to integer and putting it in an array
        strcpy(roomType[k],typeroom); //storing type of room in an array
        fgets(fileLine,200,prodFile);
        k++;
    }
    itemsInFile=k;
    fclose(prodFile);
	printf("\nWhich room are you checking out from?\nPress 1 for Single bed room\nPress 2 for Double bed room\nPress 3 for Suite.\n");
    scanf("%d",&type);
    index = type-1;
    if (NumberOfRooms[index]!=5)
    {
        NumberOfRooms[index] = NumberOfRooms[index]+1; //increasing the number of available rooms
		prodFile= fopen ("availableroom.txt","w+");
		for (k=0;k<itemsInFile;k++)
        {
            //updating the list of available rooms
            fprintf(prodFile, "%s = %d\n" , roomType[k],NumberOfRooms[k]);
        }
            printf("\nCheck Out Successful.\n");
        }
    else
	{
		printf("\nError. No rooms of this type are booked.\n");
	} 
	fclose(prodFile);
    printf("\nHas the day ended?\n Press 'Y/y' for Yes. \t Press 'N/n' for No.\n");
    option=getch();
}

void DayEnd()
{
    char viewing, choice=0,password[7]={0};
    char passkey[7]={'s','e','c','r','e','t'};
    int i,access=0;
    //reviewing how many items are left in the inventory
    printf("\nDo you want to view the inventories?\nPress 'Y/y' for Yes 'N/n' for No\n");
    fflush(stdin);
    viewing=getch();
    if (viewing=='y'||viewing=='Y')
    {
        printf("\n\nEnter Administrator password: ");
        fflush(stdin);
        for (i=0;i<6;i++)
        {
            password[i]=getch();
        }
       for (i=0;i<6;i++) // matching the password entered
        {
            if (password[i]==passkey[i])
            {
                access=1;
            }
            else
                access=0;
        }
        if (access==1)
        {
            printf("\nACCESS GRANTED.\n\n");
            printf("\nDo you want to check the inventory?\nPress 'Y/y' for yes or 'N/n' for no.\n");
            choice=getch();
            if (choice=='Y'||choice=='y')
            {
                inventory(1,1); //printing inventory
            }
            //reviewing how many rooms are available
            printf("\nDo you want to review avaiable rooms?\nPress 'Y/y' for yes or 'N/n' for no.\n");
            choice=getch();
            if (choice=='Y'||choice=='y')
            {
                availability(1,1); //printing available rooms
            }
            printf("\nThat's all for today. Goodbye.\n");
            getch();
        }
        else if (access==0)
        {
            printf("ACCESS DENIED.");
            printf("\nThat's all for today. Goodbye.\n");
        }
    }
    else printf("Goodbye! See you soon.\n");
}
