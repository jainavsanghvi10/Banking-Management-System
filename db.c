#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

typedef struct normal_user
{
	int user_id;
	char user_name[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
};

typedef struct joint_user
{
	int user_id;
	char user_name1[30];
    char user_name2[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
};

typedef struct admin
{
    int admin_id;
    char admin_username[30];
    char password[10];
};

int get_normal_user()
{
    int fd = open("NormalUsers.dat", O_RDONLY, 0744);
    struct normal_user user;
    lseek(fd,-sizeof(user), SEEK_END);
    read(fd, &user, sizeof(struct normal_user));
    close(fd);
    return user.user_id + 1;
}

int get_joint_users()
{
    int fd = open("JointUsers.dat", O_RDONLY, SEEK_END);
    struct joint_user user;
    lseek(fd,-sizeof(user), SEEK_END);
    read(fd, &user, sizeof(user));
    close(fd);
    return user.user_id + 1;
}

int get_admin()
{
    int fd = open("Admins.dat", O_RDONLY, SEEK_END);
    struct admin user;
    lseek(fd,-sizeof(user), SEEK_END);
    read(fd, &user, sizeof(user));
    close(fd);
    return user.admin_id + 1;
}

int main()
{
    printf("------------------Adding Admins------------------\n");
    int fd = open("Admins.dat", O_RDWR|O_CREAT, 0744);
    int Continue = 0;
    struct admin NewAdmin;
    printf("Please enter the username of admin: ");
    scanf("%s", NewAdmin.admin_username);
    printf("Please enter the password[Max. 10 characters]: ");
    scanf("%s", NewAdmin.password);
    NewAdmin.admin_id=1;
    printf("Your Admin ID is: %d\n", NewAdmin.admin_id);
    write(fd, &NewAdmin, sizeof(NewAdmin));
    printf("Do you want to Continue?\n");
    printf("Enter 0 to quit and 1 to Continue.\n");
    scanf("%d", &Continue);
    while(Continue)
    {
        //struct admin NewAdmin;
        printf("Please enter the username of admin: ");
        scanf("%s", NewAdmin.admin_username);
        printf("Please enter the password[Max. 10 characters]: ");
        scanf("%s", NewAdmin.password);
        NewAdmin.admin_id=get_admin();
        printf("Your Admin ID is: %d\n", NewAdmin.admin_id);
        write(fd, &NewAdmin, sizeof(NewAdmin));
        printf("Do you want to Continue?\n");
        printf("Enter 0 to quit and 1 to Continue.\n");
        scanf("%d", &Continue);
    }
    close(fd);
    printf("------------------Adding Normal Users------------------\n");
    fd = open("NormalUsers.dat", O_RDWR|O_CREAT, 0744);
    Continue = 0;
    struct normal_user user;
    printf("Please enter the username: ");
    scanf("%s", user.user_name);
    printf("Please enter the password[Max. 10 characters]: ");
    scanf("%s", user.password);
    user.user_id=1;
    user.balance=1000;
    user.account_no=user.user_id;
    printf("Your User ID is: %d\n", user.user_id);
    printf("Your Account no is: %d\n", user.account_no);
	strcpy(user.status,"ACTIVE");
    write(fd, &user, sizeof(user));
    printf("Do you want to Continue?\n");
    printf("Enter 0 to quit and 1 to Continue.\n");
    scanf("%d", &Continue);
    while(Continue)
    {
        printf("Please enter the username: ");
        scanf("%s", user.user_name);
        printf("Please enter the password[Max. 10 characters]: ");
        scanf("%s", user.password);
        user.user_id=get_normal_user();
        user.balance=1000;
        user.account_no=user.user_id;
        printf("Your User ID is: %d\n", user.user_id);
        printf("Your Account no is: %d\n", user.account_no);
	    strcpy(user.status,"ACTIVE");
        write(fd, &user, sizeof(user));
        printf("Do you want to Continue?\n");
        printf("Enter 0 to quit and 1 to Continue.\n");
        scanf("%d", &Continue);
    }
    close(fd);

    printf("------------------Adding Joint Users------------------\n");
    fd = open("JointUsers.dat", O_RDWR|O_CREAT, 0744);
    Continue = 0;
    struct joint_user User;
    printf("Please enter the main username: ");
    scanf("%s", User.user_name1);
    printf("Please enter the second username: ");
    scanf("%s", User.user_name2);
    printf("Please enter the password[Max. 10 characters]: ");
    scanf("%s", User.password);
    User.user_id=1;
    User.balance=1000;
    User.account_no=User.user_id;
    printf("Here\n");
    printf("Your User ID is: %d\n", User.user_id);
    printf("Your Account no is: %d\n", User.account_no);
	strcpy(User.status,"ACTIVE");
    write(fd, &User, sizeof(User));
    printf("Do you want to Continue?\n");
    printf("Enter 0 to quit and 1 to Continue.\n");
    scanf("%d", &Continue);
    while(Continue)
    {
        printf("Please enter the main username: ");
        scanf("%s", User.user_name1);
        printf("Please enter the second username: ");
        scanf("%s", User.user_name2);
        printf("Please enter the password[Max. 10 characters]: ");
        scanf("%s", User.password);
        User.user_id=get_joint_users();
        User.balance=1000;
        User.account_no=User.user_id;
        printf("Your User ID is: %d\n", user.user_id);
        printf("Your Account no is: %d\n", user.account_no);
	    strcpy(User.status,"ACTIVE");
        write(fd, &User, sizeof(User));
        printf("Do you want to Continue?\n");
        printf("Enter 0 to quit and 1 to Continue.\n");
        scanf("%d", &Continue);
    }
    close(fd);
    return 0;
}