# Banking-Management-System
Developed a user-friendly and multi-functional banking system with the following functionalities:

* All account transactional details and customer information are stored in files.
* Account holders have to pass through a login system to enter their accounts.
* Password-protected administrative access prevents unauthorized access to the management system.
* Three types of login were created: normal user, joint account user, and administrator.
* The administrator can add, delete, modify, and search for specific account details.
* The customer is prompted with a login and password prompt upon connecting to the server.
* Once logged in, the customer can perform operations such as deposit, withdraw, balance enquiry, password change, view details, or exit.
* Proper file locking was implemented for joint account holders to protect the critical data section during read and write operations.
* Socket programming was used to enable the server to maintain the database and service multiple clients concurrently.
* System calls were preferred over library functions wherever possible for process management, file management, file locking, multithreading, and interprocess communication mechanisms.

## Instructions to run:
1. Run db.c to create database.
2. Run create_semaphore.c to create semaphore.
2. Then run cc -pthread server.c -o server and then ./server to compile and run server.
3. Then run cc client.c -o client and then ./client to run client. 
