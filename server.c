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

typedef struct normal_user
{
	int user_id;
	char user_name[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
} normal_user;

typedef struct joint_user
{
	int user_id;
	char user_name1[30];
    char user_name2[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
} joint_user;

typedef struct admin
{
    int admin_id;
    char admin_username[30];
    char password[10];
} admin;

normal_user get_normal_user(int user_id)
{
    normal_user curr_user;
    int fd = open("NormalUsers.dat", O_RDONLY, 0744);

    int fl;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (user_id - 1)*sizeof(curr_user);
    lock.l_len = sizeof(curr_user);
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (user_id - 1)*sizeof(curr_user), SEEK_SET);
    read(fd, &curr_user, sizeof(curr_user));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return curr_user; 
}

joint_user get_joint_user(int user_id)
{
    joint_user curr_user;
    int fd = open("JointUsers.dat", O_RDONLY, 0744);

    int fl;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (user_id - 1)*sizeof(curr_user);
    lock.l_len = sizeof(curr_user);
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (user_id - 1)*sizeof(curr_user), SEEK_SET);
    read(fd, &curr_user, sizeof(curr_user));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return curr_user; 
}

admin get_admin(int user_id)
{
    admin curr_user;
    int fd = open("Admins.dat", O_RDONLY, 0744);

    int fl;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (user_id - 1)*sizeof(curr_user);
    lock.l_len = sizeof(curr_user);
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (user_id - 1)*sizeof(curr_user), SEEK_SET);
    read(fd, &curr_user, sizeof(curr_user));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return curr_user; 
}

bool check_normal_user(normal_user curr_user)
{
    normal_user temp;
    bool result;
    int fd = open("NormalUsers.dat", O_RDONLY, 0744);

    int fl;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (curr_user.user_id - 1)*sizeof(curr_user);
    lock.l_len = sizeof(curr_user);
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (curr_user.user_id - 1)*sizeof(temp), SEEK_SET);
    read(fd, &temp, sizeof(temp));
    
    if(temp.user_id == curr_user.user_id && !strcmp(temp.password, curr_user.password) && !strcmp(temp.status, "ACTIVE")) result = true;
    else result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result; 
}

bool check_joint_user(joint_user curr_user)
{
    joint_user temp;
    bool result;
    int fd = open("JointUsers.dat", O_RDONLY, 0744);

    int fl;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (curr_user.user_id - 1)*sizeof(curr_user);
    lock.l_len = sizeof(curr_user);
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (curr_user.user_id - 1)*sizeof(temp), SEEK_SET);
    read(fd, &temp, sizeof(temp));
    
    if(temp.user_id == curr_user.user_id && !strcmp(temp.password, curr_user.password) && !strcmp(temp.status, "ACTIVE")) result = true;
    else result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result; 
}

bool check_admin(struct admin curr_user)
{
    struct admin temp;
    bool result;
    int fd = open("Admins.dat", O_RDONLY, 0744);

    int fl;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (curr_user.admin_id - 1)*sizeof(curr_user);
    lock.l_len = sizeof(admin);
    lock.l_pid = getpid();

    fl = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (curr_user.admin_id - 1)*sizeof(admin), SEEK_SET);
    read(fd, &temp, sizeof(admin));
    
	printf("%d %d %s %s", temp.admin_id, curr_user.admin_id, temp.password, curr_user.password);
    if(temp.admin_id == curr_user.admin_id && !strcmp(temp.password, curr_user.password)) result = true;
    else result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result; 
}

bool deposit_money(int acc_type, int user_id, float amount)
{
	if(acc_type == 1)
    {
		int fd = open("NormalUsers.dat", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(user_id - 1)*sizeof(normal_user);    
		lock.l_len=sizeof(normal_user);	             
		lock.l_pid=getpid();
		fl=fcntl(fd,F_SETLKW,&lock);	 

		normal_user currUser;
		lseek(fd,(user_id-1)*sizeof(normal_user),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normal_user));
		
		if(!strcmp(currUser.status,"ACTIVE"))
        {
			currUser.balance+=amount;
			lseek(fd,sizeof(normal_user)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normal_user));
			result=true;
		}
		else	
            result=false;
		
        lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
    }

    else if(acc_type == 2)
    {
        // printf("251\n");
		int fd = open("JointUsers.dat", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(user_id - 1)*sizeof(joint_user);    
		lock.l_len=sizeof(joint_user);	             
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		
		printf("Amount: %f", amount);
		joint_user currUser;
		lseek(fd,(user_id-1)*sizeof(joint_user),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(joint_user));
		printf("%s\n", currUser.status);
		

		if(!strcmp(currUser.status,"ACTIVE"))
        {
			// printf("272\n");
			currUser.balance+=amount;
			printf("%f", currUser.balance);
			lseek(fd,sizeof(joint_user)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(joint_user));
			result=true;
		}
		else	
            result=false;
		
        lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		// printf("284\n");
		printf("%d", result);
		return result;
    }
    return false;
}

bool withdraw_money(int acc_type, int user_id, float amount)
{
    if(acc_type == 1)
    {
        int fd = open("NormalUsers.dat", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(user_id - 1)*sizeof(normal_user);    
		lock.l_len=sizeof(normal_user);	             
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		

		normal_user currUser;
		lseek(fd,(user_id-1)*sizeof(normal_user),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normal_user));
		
		if(!strcmp(currUser.status,"ACTIVE") && currUser.balance-amount>0)
        {
			currUser.balance-=amount;
			lseek(fd,sizeof(normal_user)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normal_user));
			result=true;
		}
		else	
            result=false;
		
        lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
    }

    else if(acc_type == 2)
    {
        int fd = open("JointUsers.dat", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(user_id - 1)*sizeof(joint_user);    
		lock.l_len=sizeof(joint_user);	             
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		

		joint_user currUser;
		lseek(fd,(user_id-1)*sizeof(joint_user),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(joint_user));
		
		if(!strcmp(currUser.status,"ACTIVE"))
        {
			currUser.balance-=amount;
			lseek(fd,sizeof(joint_user)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(joint_user));
			result=true;
		}
		else	
            result=false;
		
        lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
    }
    return false;
}

float get_balance(int acc_type, int user_id)
{
    float result;
    if(acc_type == 1)
    {
        int fd = open("NormalUsers.dat", O_RDWR, 0744);
        int fl;
        struct flock lock;
        lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(user_id - 1)*sizeof(normal_user);    
		lock.l_len=sizeof(normal_user);	             
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		

		normal_user currUser;
		lseek(fd,(user_id-1)*sizeof(normal_user),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normal_user));
		
		if(!strcmp(currUser.status,"ACTIVE"))
        {
			result = currUser.balance;
		}
		else	
            result=0;
		
        lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
    }

    else if(acc_type == 2)
    {
        int fd = open("JointUsers.dat", O_RDWR, 0744);
        int fl;
        struct flock lock;
        lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(user_id - 1)*sizeof(joint_user);    
		lock.l_len=sizeof(joint_user);	             
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		

		joint_user currUser;
		lseek(fd,(user_id-1)*sizeof(joint_user),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(joint_user));
		
		if(!strcmp(currUser.status,"ACTIVE"))
        {
			result=currUser.balance;
		}
		else	
            result=0;
		
        lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
    }
    return 0;
}

bool change_password(int acc_type, int user_id, char new_password[10])
{
    if(acc_type == 1)
    {
        int fd = open("NormalUsers.dat", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(user_id - 1)*sizeof(normal_user);    
		lock.l_len=sizeof(normal_user);	             
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		

		normal_user currUser;
		lseek(fd,(user_id-1)*sizeof(normal_user),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normal_user));
		
		if(!strcmp(currUser.status,"ACTIVE"))
        {
			strcpy(currUser.password, new_password);
			lseek(fd,sizeof(normal_user)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normal_user));
			result=true;
		}
		else	
            result=false;
		
        lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
    }

    else if(acc_type == 2)
    {
        int fd = open("JointUsers.dat", O_RDWR, 0744);
        bool result;
        int fl;
        struct flock lock;
        lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(user_id - 1)*sizeof(joint_user);    
		lock.l_len=sizeof(joint_user);	             
		lock.l_pid=getpid();
	
		fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		

		joint_user currUser;
		lseek(fd,(user_id-1)*sizeof(joint_user),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(joint_user));
		
		if(!strcmp(currUser.status,"ACTIVE"))
        {
			strcpy(currUser.password, new_password);
			lseek(fd,sizeof(joint_user)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(joint_user));
			result=true;
		}
		else	
            result=false;
		
        lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
    }
    return false;
}

bool add_normal_user(normal_user record)
{
    int fd = open("NormalUsers.dat", O_RDWR);
    bool result;
    int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(normal_user);    
	lock.l_len=sizeof(normal_user);	             
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	

	normal_user endUser;
	lseek(fd,(-1)*sizeof(endUser),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(endUser));
		
	record.user_id=endUser.user_id+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(normal_user));
	if(j!=0)	result=true;
	else	result=false;
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;
}

bool add_joint_user(joint_user record)
{
    int fd = open("JointUsers.dat", O_RDWR);
    bool result;
    int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(joint_user);    
	lock.l_len=sizeof(joint_user);	             
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	

	joint_user endUser;
	lseek(fd,(-1)*sizeof(endUser),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(endUser));
		
	record.user_id=endUser.user_id+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(joint_user));
	if(j!=0)	result=true;
	else	result=false;
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;
}

bool delete_normal_user(int user_id)
{
    printf("585");
	int fd = open("NormalUsers.dat", O_RDWR);
    bool result;
    int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(user_id-1)*sizeof(normal_user);    
	lock.l_len=sizeof(normal_user);	             
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record


	normal_user endUser;
	lseek(fd,(user_id-1)*sizeof(endUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(endUser));
	printf("601\n");
	printf("%s\n", endUser.status);
	if(!strcmp(endUser.status,"ACTIVE"))
    {	
		printf("604");
		strcpy(endUser.status,"CLOSED");
		endUser.balance=0;
		
		lseek(fd,(-1)*sizeof(normal_user),SEEK_CUR); 
		int j=write(fd,&endUser,sizeof(normal_user));
		printf("%d", j);
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;
}

bool delete_joint_user(int user_id)
{
    int fd = open("JointUsers.dat", O_RDWR);
    bool result;
    int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(user_id-1)*sizeof(joint_user);    
	lock.l_len=sizeof(joint_user);	             
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	joint_user endUser;
	lseek(fd,(user_id-1)*sizeof(endUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(endUser));
	
	if(!strcmp(endUser.status,"ACTIVE"))
    {	
		strcpy(endUser.status,"CLOSED");
		endUser.balance=0;
		
		lseek(fd,(-1)*sizeof(joint_user),SEEK_CUR); 
		int j=write(fd,&endUser,sizeof(joint_user));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;
}

bool modify_normal_user(normal_user modUser){
	//int i=modUser.userID-1000;
	int fd=open("NormalUsers.dat",O_RDWR,0744);
	bool result=false;
	
	int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(modUser.user_id-1)*sizeof(normal_user);    
	lock.l_len=sizeof(normal_user);	             
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	

	normal_user currUser;
	lseek(fd,(modUser.user_id-1)*sizeof(normal_user),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(normal_user));
	
	if(!strcmp(currUser.status,"ACTIVE") && (modUser.account_no==currUser.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(normal_user),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(normal_user));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modify_joint_user(joint_user modUser){
	//int i=modUser.userID-1000;
	int fd=open("JointUsers.dat",O_RDWR,0744);
	bool result=false;
	
	int fl;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(modUser.user_id-1)*sizeof(joint_user);    
	lock.l_len=sizeof(joint_user);	             
	lock.l_pid=getpid();
	
	fl=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	

	normal_user currUser;
	lseek(fd,(modUser.user_id-1)*sizeof(joint_user),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(joint_user));
	
	if(!strcmp(currUser.status,"ACTIVE") && (modUser.account_no==currUser.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(joint_user),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(joint_user));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

void server_task(int nsd)
{
	int msg_length, select, type, choice, acc_type, curr_user_id;
	bool result;
	while(1)
	{
		read(nsd, &choice, sizeof(choice));
		printf("Choice: %d\n", choice);

		if(choice == 1)
		{
			struct normal_user curr_user;
			acc_type = 1;
			msg_length = read(nsd, &curr_user, sizeof(curr_user));
			printf("User ID: %d\n", curr_user.user_id);
			printf("Password: %s\n", curr_user.password);
			result = check_normal_user(curr_user);
			write(nsd, &result, sizeof(result));
		}
		else if(choice == 2)
		{
			struct joint_user curr_user;
			acc_type = 2;
			msg_length = read(nsd, &curr_user, sizeof(curr_user));
			printf("User ID: %d\n", curr_user.user_id);
			printf("Password: %s\n", curr_user.password);
			result = check_joint_user(curr_user);
			write(nsd, &result, sizeof(result));
		}
		else if(choice == 3)
		{
			struct admin curr_user2;
			acc_type = 3;
			printf("HERE\n");
			msg_length = read(nsd, &curr_user2, sizeof(curr_user2));
			printf("User ID: %d\n", curr_user2.admin_id);
			printf("Password: %s\n", curr_user2.password);
			result = check_admin(curr_user2);
			write(nsd, &result, sizeof(result));
		}
		else
		{
			result = false;
			write(nsd, &result, sizeof(result));
		}
		if(result) break;
	}

	while(1)
	{
		read(nsd,&select,sizeof(int));
		if(choice==1 || choice==2)
		{
			if(select==1)
			{
				float amount;
				read(nsd,&amount,sizeof(float));
				result=deposit_money(acc_type,curr_user_id,amount);
				write(nsd,&result,sizeof(result));
			}
			else if(select==2)
			{
				float amount;
				read(nsd,&amount,sizeof(float));
				result=withdraw_money(acc_type,curr_user_id,amount);
				write(nsd,&result,sizeof(result));
			}
			else if(select==3)
			{
				float amount;
				amount=get_balance(acc_type,curr_user_id);
				write(nsd,&amount,sizeof(float));
			}
			else if(select==4)
			{
				char password[10];
				read(nsd,password,sizeof(password));
				result=change_password(acc_type,curr_user_id,password);
				write(nsd,&result,sizeof(result));
			}
			else if(select==5)
			{
				if(choice==1)
				{
					normal_user user1 = get_normal_user(curr_user_id);
					write(nsd,&user1,sizeof(normal_user));
				}
				else if(choice==2)
				{
					joint_user user2=get_joint_user(curr_user_id);
					write(nsd,&user2,sizeof(joint_user));
				}
			}
			else if(select==6)	break;
		}
		else if(choice==3)
		{
			read(nsd,&type,sizeof(int));
			if(select==1)
			{
				if(type==1)
				{
					struct normal_user new_user;
					read(nsd,&new_user,sizeof(normal_user));
					result=add_normal_user(new_user);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2)
				{
					struct joint_user new_user;
					read(nsd,&new_user,sizeof(joint_user));
					result=add_joint_user(new_user);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==2)
			{
				if(type==1){
					int del_user_ID;
					read(nsd,&del_user_ID,sizeof(int));
					printf("User ID: %d\n", del_user_ID);
					result=delete_normal_user(del_user_ID);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					int del_user_ID;
					read(nsd,&del_user_ID,sizeof(int));
					result=delete_joint_user(del_user_ID);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==3)
			{
				if(type==1)
				{
					struct normal_user modify_user;
					read(nsd,&modify_user,sizeof(normal_user));
					result=modify_normal_user(modify_user);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2)
				{
					struct joint_user modify_user;
					read(nsd,&modify_user,sizeof(joint_user));
					result=modify_joint_user(modify_user);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==4)
			{
				if(type==1)
				{
					struct normal_user user;
					int user_id;
					read(nsd,&user_id,sizeof(int));
					user=get_normal_user(user_id);
					write(nsd,&user,sizeof(normal_user));
				}
				else if(type==2){
					struct joint_user user;
					int user_id;
					read(nsd,&user_id,sizeof(int));
					user=get_joint_user(user_id);
					write(nsd,&user,sizeof(joint_user));
				}
			}
			else if(select==5)	break;
		}
	}
	close(nsd);
	write(1,"Ended client session.....\n",sizeof("Ended client session.....\n"));
	return; 
}

void *connection_handler(void *nsd)
{
	int Nsd = *(int*)nsd;
	server_task(Nsd);
} 

int main()
{
	struct sockaddr_in serv, cli;
	int sd, nsd, client_length;
	pthread_t threads;
	bool result;
	sd=socket(AF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(5710);

	bind(sd, (struct sockaddr*)&serv, sizeof(serv));
	listen(sd, 5);

	write(1, "Waiting for the client....\n", sizeof("Waiting for the client....\n"));
	while(1)
	{
		client_length = sizeof(cli);
		nsd = accept(sd, (struct sockaddr *)&cli, &client_length);
		
		write(1,"Connected to the client.....\n",sizeof("Connected to the client.....\n"));
		if(pthread_create(&threads, NULL, connection_handler, (void*) &nsd) < 0)
		{
			perror("Cannot create thread");
			return 1;
		}
		
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}