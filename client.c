#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <pthread.h>
#include<sys/resource.h>
#include<sys/time.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include<signal.h>
#include<string.h>
int choice, curr_user_id;

struct normal_user
{
	int user_id;
	char user_name[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
};

struct joint_user
{
	int user_id;
	char user_name1[30];
    char user_name2[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
};

struct admin
{
    int admin_id;
    char admin_username[30];
    char password[10];
};

void menu(sd);
void joint_user_login(int sd);
void admin_login(int sd);

void normal_user_login(int sd)
{
    bool result;
    struct normal_user user;
    printf("User ID: ");
    scanf("%d", &user.user_id);
    printf("Enter Password: ");
    scanf("%s", user.password);

    write(sd, &choice, sizeof(int));
    write(sd, &user, sizeof(user));
    
    read(sd, &result, sizeof(result));

    if(result) printf("Successfully Logged in....!!\n");
    else
    {
        int choice;
        printf("Invalid Login!!...\n");
        
        printf("|1.| Normal user Login|\n");
        
        printf("|2.| Joint User Login |\n");
        
        printf("|3.| Admin Login      |\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        if(choice == 1)
        {
            normal_user_login(sd);
            // break;
        }

        else if(choice == 2)
        {
            joint_user_login(sd);
        }

        else if(choice == 3)
        {
            admin_login(sd);
        }
        else
        {
            printf("Invalid Credentials...\n");
        }
    }
}

void joint_user_login(int sd)
{
    bool result;
    struct joint_user user;
    printf("Joint User ID: ");
    scanf("%d", &user.user_id);
    printf("Enter Password: ");
    scanf("%s", user.password);

    write(sd, &choice, sizeof(int));
    write(sd, &user, sizeof(user));
    
    read(sd, &result, sizeof(result));

    if(result) printf("Successfully Logged in....!!\n");
    else 
    {
        int choice;
        printf("Invalid Login!!...\n");
        
        printf("|1.| Normal user Login|\n");
        
        printf("|2.| Joint User Login |\n");
        
        printf("|3.| Admin Login      |\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        if(choice == 1)
        {
            normal_user_login(sd);
            // break;
        }

        else if(choice == 2)
        {
            joint_user_login(sd);
        }

        else if(choice == 3)
        {
            admin_login(sd);
        }
        else
        {
            printf("Invalid Credentials...\n");
        }
    }
}

void admin_login(int sd)
{
    bool result;
    struct admin user;
    printf("Admin ID: ");
    scanf("%d", &user.admin_id);
    curr_user_id=user.admin_id;
    printf("Enter Password: ");
    scanf("%s", user.password);
    // printf("Given Password: %s\n", user.password);
    // printf("Choice: %d\n, choice");
    write(sd, &choice, sizeof(int));
    write(sd, &user, sizeof(user));
    
    read(sd, &result, sizeof(result));

    if(result) printf("Successfully Logged in....!!\n");
    else 
    {
        int choice;
        printf("Invalid Login!!...\n");
        
        printf("|1.| Normal user Login|\n");
        
        printf("|2.| Joint User Login |\n");
        
        printf("|3.| Admin Login      |\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        if(choice == 1)
        {
            normal_user_login(sd);
            // break;
        }

        else if(choice == 2)
        {
            joint_user_login(sd);
        }

        else if(choice == 3)
        {
            admin_login(sd);
        }
        else
        {
            printf("Invalid Credentials...\n");
        }
    }
}

void money_deposit(int sd)
{
    float amount;
    int select = 1;
    bool result;

    printf("Please enter the amount to be deposited: ");
    scanf("%f", &amount);
    
    write(sd, &select, sizeof(int));
    write(sd, &amount, sizeof(float));

    read(sd, &result, sizeof(result));

    if(result) printf("Amount Successfully Deposited!!\n");
    else printf("Error in depositing money!!\n");
    menu(sd); 
}

void money_deposit_joint(int sd)
{
    float amount;
    int select = 1;
    bool result;

    int key, semid;
    key = ftok(".", 'a');
    struct sembuf buf = {0, -1, 0|SEM_UNDO};
    semid = semget(key, 1, 0);

    semop(semid, &buf, 1);

    printf("Please enter the amount to be deposited: ");
    scanf("%f", &amount);
    
    write(sd, &select, sizeof(int));
    write(sd, &amount, sizeof(float));

    read(sd, &result, sizeof(result));

    if(result) printf("Amount Successfully Deposited!!\n");
    else printf("Error in depositing money!!\n");

    buf.sem_op = 1;
    semop(semid, &buf, 1);

    menu(sd); 
}

void money_withdraw(int sd)
{
    float amount;
    int select = 2;
    bool result;

    printf("Please enter the amount to be withdrawn: ");
    scanf("%f", &amount);
    
    write(sd, &select, sizeof(int));
    write(sd, &amount, sizeof(float));

    read(sd, &result, sizeof(result));

    if(result) printf("Amount Successfully Withdrawn!!\n");
    else printf("Error in withdrawing money!!\n");
    menu(sd); 
}

void money_withdraw_joint(int sd)
{
    float amount;
    int select = 2;
    bool result;

    int key, semid;
    key = ftok(".", 'a');
    struct sembuf buf = {0, -1, 0|SEM_UNDO};
    semid = semget(key, 1, 0);

    semop(semid, &buf, 1);

    printf("Please enter the amount to be withdrawn: ");
    scanf("%f", &amount);

    write(sd, &select, sizeof(int));
    write(sd, &amount, sizeof(float));

    read(sd, &result, sizeof(result));

    if(result) printf("Amount Successfully Withdrawn!!\n");
    else printf("Error in withdrawing money!!\n");
    buf.sem_op = 1;
    semop(semid, &buf, 1);
    
    menu(sd); 
}

void enquire_balance(int sd)
{
    float amount;
    int select = 3;
    int length;

    write(sd, &select, sizeof(int));

    length = read(sd, &amount, sizeof(amount));

    printf("Available Balance: %0.2f\n", amount);
    menu(sd);
}

void alter_password(int sd)
{
    int select = 4;
    char new_password[10];
    bool result;

    printf("Enter the new password[maximum 10 characters]: ");
    scanf("%s", new_password);

    write(sd, &select, sizeof(int));
    write(sd, new_password, sizeof(new_password));

    read(sd, &result, sizeof(result));

    if(result) printf("Successfully Changed PAssword...!!\n");
    else printf("Error in changing Password...!!\n");
    menu(sd);
}

void view_acc_details(int sd)
{
    int select = 5;
    write(sd, &select, sizeof(int));

    if(choice == 1)
    {
        struct normal_user user;
        read(sd, &user, sizeof(struct normal_user));

        printf("User ID : %d\n",user.user_id);
		printf("Name : %s\n",user.user_name);
		printf("Account Number : %d\n",user.account_no);
		printf("Available Balance : Rs.%0.2f\n",user.balance);
		printf("Status : %s\n\n",user.status);
    }
    else if(choice == 2)
    {
        struct joint_user user;
        read(sd, &user, sizeof(struct joint_user));

        printf("User ID : %d\n",user.user_id);
		printf("Main Holder's Name : %s\n",user.user_name1);
        printf("Second Account Holder's Name: %s\n", user.user_name2);
		printf("Account Number : %d\n",user.account_no);
		printf("Available Balance : Rs.%0.2f\n",user.balance);
		printf("Status : %s\n\n",user.status);
    }
    menu(sd);
}

void add_account(int sd)
{
	int select=1;
	int type;
	bool result;

	write(sd,&select,sizeof(int));

    
    printf("|1.| Normal Account |\n");
    
    printf("|2.| Joint Account  |\n");
    
	printf("Enter the type account: ");
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1)
    {
		struct normal_user add_user;
		printf("Name of the account holder : ");
		scanf(" %[^\n]",add_user.user_name);
		printf("Password(max 10 characters) : ");
		scanf("%s",add_user.password);
		printf("Initial Deposit : Rs.");
		scanf("%f",&add_user.balance);
		write(sd,&add_user,sizeof(struct normal_user));
	}

	if(type==2)
    {
		struct joint_user add_user;
		printf("Name of the primary account holder : ");
		scanf(" %[^\n]",add_user.user_name1);
		printf("Name of the secondary account holder : ");
		scanf(" %[^\n]",add_user.user_name2);
        printf("Password(max 10 characters) : ");
		scanf("%s",add_user.password);
		printf("Initial Deposit : Rs.");
		scanf("%f",&add_user.balance);
		write(sd,&add_user,sizeof(struct joint_user));
	}
	
	read(sd,&result,sizeof(result)); //from the server

	if(result)
    {
       printf("Succesfully added the account!!\n\n");
	}
	else
    {
		printf("Error adding the account!!\n\n");
    }
	menu(sd);
}

void delete_account(int sd)
{
	int select=2;
	int type, user_id;
	bool result;

	write(sd,&select,sizeof(int));

    
    printf("|1.| Normal Account |\n");
    
    printf("|2.| Joint Account  |\n");
    
	printf("Enter the type account: ");    // printf("211\n");

	scanf("%d",&type);

	write(sd,&type,sizeof(int));

    printf("Enter User ID to be deleted: ");
    scanf("%d", &user_id);
    write(sd, &user_id, sizeof(int));
	
	read(sd,&result,sizeof(result)); //from the server

	if(result)
    {
       printf("Succesfully Deleted the account!!\n\n");
	}
	else
    {
		printf("Error Deleting the account!!\n\n");
    }
	menu(sd);
}

void modify_account(int sd)
{
	int select=3;
	int type;
	bool result;

	write(sd,&select,sizeof(int));

	
    printf("|1.| Normal Account |\n");
    
    printf("|2.| Joint Account  |\n");
    
	printf("Enter the type account: ");
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1)
    {
		struct normal_user modUser1;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&modUser1.user_id);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&modUser1.account_no);
		write(1,"New Name of the account holder : ",sizeof("New Name of the account holder : "));
		scanf(" %[^\n]",modUser1.user_name);
		write(1,"New Password(max 10 characters) : ",sizeof("New Password(max 10 characters) : "));
		scanf("%s",modUser1.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&modUser1.balance);
		write(sd,&modUser1,sizeof(struct normal_user));
	}

	if(type==2){
		struct joint_user modUser2;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&modUser2.user_id);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&modUser2.account_no);
		write(1,"New Name of the primary account holder : ",sizeof("New Name of the primary account holder : "));
		scanf(" %[^\n]",modUser2.user_name1);
		write(1,"New Name of the other account holder : ",sizeof("New Name of the other account holder : "));
		scanf(" %[^\n]",modUser2.user_name2);
		write(1,"New Password(max 10 characters) : ",sizeof("New Password(max 10 characters) : "));
		scanf("%s",modUser2.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&modUser2.balance);
		write(sd,&modUser2,sizeof(struct joint_user));
	}
	
	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error modifying the account ,please re-check the User ID and Account No!!\n\n",sizeof("Error modifying the account ,please re-check the User ID and Account No!!\n\n"));
	}
	else{
		write(1,"Succesfully modified the account!!\n\n",sizeof("Succesfully modified the account!!\n\n"));
	}
	menu(sd);
	return;
}

void search_account(int sd){
	int select=4;
	int type,len;
	bool result;

	write(sd,&select,sizeof(int));

	
    printf("1. Normal Account \n");
    
    printf("2. Joint Account  \n");
    
	printf("Enter the type account: ");
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1)
    {
		struct normal_user search_user;
		int user_id;
		printf("User ID : ");
		scanf("%d",&user_id);
		write(sd,&user_id,sizeof(int));
		
		len=read(sd,&search_user,sizeof(struct normal_user));
		if(len==0)
        {
			printf("Please re-check the User ID!!\n\n");
		}
		else
        {
			printf("User ID : %d\n",search_user.user_id);
			printf("Name : %s\n",search_user.user_name);
			printf("Account Number : %d\n",search_user.account_no);
			printf("Available Balance : Rs.%0.2f\n",search_user.balance);
			printf("Status : %s\n\n",search_user.status);
		}
	}

	if(type==2){
		struct joint_user search_user;
		int user_id;
		printf("User ID : ");
		scanf("%d",&user_id);
		write(sd,&user_id,sizeof(int));
		
		len=read(sd,&search_user,sizeof(struct joint_user));
		if(len==0)
        {
			printf("Please re-check the User ID!!\n\n");
		}
		else
        {
			printf("User ID : %d\n",search_user.user_id);
			printf("Main Account Holder's Name : %s\n",search_user.user_name1);
			printf("Other Account Holder's Name : %s\n",search_user.user_name2);
			printf("Account Number : %d\n",search_user.account_no);
			printf("Available Balance : Rs.%0.2f\n",search_user.balance);
			printf("Status : %s\n\n",search_user.status);
		}
	}
	menu(sd);
}

void menu(int sd)
{
    int option; 

    if(choice == 1)
    {
        
        printf("\n");
        printf("1. Deposit         \n");
        
        printf("2. Withdraw        \n");
        
        printf("3. Enquire Balance \n");
        
        printf("4. Change Password \n");
        
        printf("5. View Details    \n");
        
        printf("6. Exit            \n");

        printf("Enter your choice: ");
        scanf("%d", &option);
        printf("\n");
        if(option == 1)
        {
            money_deposit(sd);
        }

        else if(option == 2)
        {
            money_withdraw(sd);
        }

        else if(option == 3)
        {
            enquire_balance(sd);
        }

        else if(option == 4)
        {
            alter_password(sd);
        }

        else if(option == 5)
        {
            view_acc_details(sd);
        }

        else if(option == 6)
        {
            write(sd, &option, sizeof(int));
            printf("Thank You!!\n");
        }
        else
        {
            menu(sd);
        }
    }

    else if(choice == 2)
    {
        
        printf("\n");
        printf("1. Deposit         \n");
        
        printf("2. Withdraw        \n");
        
        printf("3. Enquire Balance \n");
        
        printf("4. Change Password \n");
        
        printf("5. View Details    \n");
        
        printf("6. Exit            \n");

        printf("Enter your choice: ");
        scanf("%d", &option);
        
        if(option == 1)
        {
            money_deposit_joint(sd);
        }

        else if(option == 2)
        {
            money_withdraw_joint(sd);
        }

        else if(option == 3)
        {
            enquire_balance(sd);
        }

        else if(option == 4)
        {
            alter_password(sd);
        }

        else if(option == 5)
        {
            view_acc_details(sd);
        }

        else if(option == 6)
        {
            write(sd, &option, sizeof(int));
            printf("Thank You!!\n");
        }
        else
        {
            menu(sd);
        }
    }

    else if(choice == 3)
    {
        
        printf("1. Add account     \n");
        
        printf("2. Delete account  \n");
        
        printf("3. Modify account  \n");
        
        printf("4. Search account  \n");
        
        printf("5. Exit            \n");


        printf("Enter your choice: ");
        scanf("%d", &option);

        if(option == 1)
        {
            add_account(sd);
        }

        else if(option == 2)
        {
            delete_account(sd);
        }

        else if(option == 3)
        {
            modify_account(sd);
        }

        else if(option == 4)
        {
            search_account(sd);
        }

        else if(option == 5)
        {
            write(sd, &choice, sizeof(int));
            printf("Thank You!!\n");
            exit(0);
        }

        else
        {
            printf("Invlaid Choice!!\n");
            menu(sd);
        }
    }
}

int main()
{
	struct sockaddr_in server;
	int sd,msgLength;
	char buff[50];
	char result;

	//connection establishment
	sd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	//server.sin_addr.s_addr=inet_addr("172.16.81.54"); //other machine
	//server.sin_addr.s_addr=INADDR_ANY; //same machine
	server.sin_addr.s_addr=inet_addr("127.0.0.1"); //same machine
	server.sin_port=htons(5710);
	connect(sd,(struct sockaddr *)&server,sizeof(server));

	while(1)
    {
        
        printf("1. Normal user Login\n");
        
        printf("2. Joint User Login \n");
        
        printf("3. Admin Login      \n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        if(choice == 1)
        {
            normal_user_login(sd);
            break;
        }

        else if(choice == 2)
        {
            joint_user_login(sd);
            break;
        }

        else if(choice == 3)
        {
            admin_login(sd);
            break;
        }
        else
        {
            printf("Invalid Credentials...\n");
        }
    }

	menu(sd);	

	close(sd);

	return 0;
}
