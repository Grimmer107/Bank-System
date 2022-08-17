// Name: Usama Qureshi

#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>

void create_account(void);
void cash_withdrawel(void);
void cash_deposit(void);
void account_summary(void);
int fill_cash(FILE *fp);
char* transaction_time_set();

// data structures
struct bank {
	char username[30];
	char password[10];
	int balance;
};

struct transaction {
	char transaction_status[10];
	int value;
	char transaction_time[100];
};

struct bank acc;
struct transaction a;
long int recsize = sizeof(acc);
char cur_time[100];
char input, name[30] = { 0 }, pin[10] = { 0 }, transaction_date[100];
int cash;
FILE* fw, * fr;


//main function
int main(void) {
	
	printf(
		"\n MAIN MENU\n"
		"\tCreate a new account: 1\n"
		"\tWithdraw cash from the account: 2\n"
		"\tFill cash into the account: 3\n"
		"\tDisplay the account summary: 4\n"
		"\tExit: 5\n\n"
		"Which task do you want to perform : Enter the respective digit for specific task : \n");
	fflush(stdin);
	input = _getch();
	while (input != '5') {
		switch (input) {

		case '1': // creating new account
			create_account();
			break;

		case '2': // cash withdrawel
			cash_withdrawel();
			break;

		case '3': // cash deposit
			cash_deposit();
			break;

		case '4': // account summary
			account_summary();
			break;

		case '5': // To end the program
			exit(1);
		}

		printf("\nPress any key to continue...\n");
		_getch();
		system("cls");
		printf(
			"\n MAIN MENU\n"
			"\tCreate a new account: 1\n"
			"\tWithdraw cash from the account: 2\n"
			"\tFill cash into the account: 3\n"
			"\tDisplay the account summary: 4\n"
			"\tExit: 5\n\n"
			"Which task do you want to perform : Enter the respective digit for specific task : \n");
		fflush(stdin);
		input = _getch();
	}
	return 0;
}

void create_account(void) {
	// Creates new account and also calls fill_cash function for initial deposit and prints an intial account summary
	// along with date of creation of account.
	printf("Enter the username of the account: \n");
	scanf("%s", acc.username);
	printf("Enter the password of the account:\n");
	scanf("%s", acc.password);
	fr = fopen(acc.username, "rb");
	while (fr != NULL) {
		printf("This account already exists.\n");
		printf("Enter a new username of the account: \n");
		scanf("%s", acc.username);
		printf("Enter the password of the account:\n");
		scanf("%s", acc.password);
		fclose(fr);
		fr = fopen(acc.username, "rb");
	}
	fw = fopen(acc.username, "wb");
	if (fw == NULL) {
		printf("Can't open the file\n");
		return;
	}
	else {
		acc.balance = fill_cash(fw);
		fwrite(&acc, sizeof(acc), 1, fw);
		strcpy(transaction_date, transaction_time_set());
		printf("Your account has been created:\n\n");
		printf("\n\t    Bank Receipt\n\n");
		printf("%-15s %-15s %-15s    %-15s\n\n", "ACC.USERNAME", "PASSWORD", "CURRENT BALANCE", "TIME");
		printf("%-15s %-15s %-15d    %-15s\n\n\n", acc.username, acc.password, acc.balance, transaction_date);
	}
	fclose(fw);
	return;
}

void cash_withdrawel(void) { 
	// To withdraw cash and prints bank reciept showing username, current balance and amount withdrawn
	printf("Enter the username of the account: \n");
	scanf("%s", name);
	fr = fopen(name, "rb+");
	if (fr == NULL) {
		printf("Can't open the file\n");
		return;
	}
	else {
		printf("Enter the amount of cash you want to withdraw: ");
		scanf("%d", &cash);
		fread(&acc, sizeof(acc), 1, fr);
		while (cash > acc.balance) {
			printf("You do not have sufficient bank balance for this withdrawel: ");
			printf("Enter less amount: ");
			scanf("%d", &cash);
		}
		acc.balance -= cash;
		fseek(fr, -recsize, SEEK_CUR);
		fwrite(&acc, sizeof(acc), 1, fr);
		a.value = cash;
		strcpy(a.transaction_status, "Withdraw ");
		strcpy(a.transaction_time, transaction_time_set());
		fseek(fr, 0, SEEK_END);
		fwrite(&a, sizeof(a), 1, fr);
		printf("\n\n\t    Bank Receipt\n\n");
		printf("%-15s %-15s %-15s   %-15s\n\n", "ACC.USERNAME", "PASSWORD", "CURRENT BALANCE", "CASH WITHDRAWN");
		printf("%-15s %-15s %-15d   %-15d\n\n\n", acc.username, acc.password, acc.balance, cash);
		fclose(fr);
	}
	return;
}

void cash_deposit(void) { 
	// To deposit cash and prints bank reciept showing username, current balance and amount deposited
	printf("Enter the username of the account: \n");
	scanf("%s", name);
	fr = fopen(name, "rb+");
	if (fr == NULL) {
		printf("Can't open the file\n");
		return;
	}
	printf("Enter the amount of cash you want to deposit: ");
	scanf("%d", &cash);
	while (cash % 10 != 0) {
		printf("Ensure that amount of cash is a multiple of 10: ");
		scanf("%d", &cash);
	}
	fread(&acc, sizeof(acc), 1, fr);
	acc.balance += cash;
	fseek(fr, -recsize, SEEK_CUR);
	fwrite(&acc, sizeof(acc), 1, fr);
	a.value = cash;
	strcpy(a.transaction_status, "Deposit ");
	strcpy(a.transaction_time, transaction_time_set());
	fseek(fr, 0, SEEK_END);
	fwrite(&a, sizeof(a), 1, fr);
	printf("\n\n\t\tBank Receipt\n\n");
	printf("%-15s %-15s %-15s   %-15s\n\n", "ACC.USERNAME", "PASSWORD", "CURRENT BALANCE", "CASH DEPOSITED");
	printf("%-15s %-15s %-15d   %-15d\n\n\n", acc.username, acc.password, acc.balance, cash);
	fclose(fr);
	return;
}

void account_summary(void) { 
	// Account summary with username, password, current balance and detail of previous transactions
	printf("Enter the username of the account: \n");
	scanf("%s", name);
	printf("Enter the password of the account:\n");
	scanf("%s", pin);
	fr = fopen(name, "rb+");
	if (fr == NULL) {
		printf("Can't open the file\n");
		return;
	}
	fread(&acc, sizeof(acc), 1, fr);
	if (strcmp(pin, acc.password) == 0) {
		printf("\n\t    Account Summary\n\n");
		printf("%-15s %-15s %-15s\n\n", "ACC.USERNAME", "PASSWORD", "CURRENT BALANCE");
		printf("%-15s %-15s %-15d\n\n\n", acc.username, acc.password, acc.balance);
		printf("\t    TRANSACTION DETAILS\n\n");
		printf("%-25s   %-20s    %-20s\n", "Transaction Date", "Transaction status", "Amount");
		fread(&a, sizeof(a), 1, fr);
		while (!(feof(fr))) {
			printf("%-25s   %-20s    %-20d\n", a.transaction_time, a.transaction_status, a.value);
			fread(&a, sizeof(a), 1, fr);
		}
	}
	else {
		printf("The password did not match.\n");
	}
	fclose(fr);
	return;
}

int fill_cash(FILE* fp) {
	// To initially deposit cash for account making
	int initial_deposit;
	printf("Enter the cash you want to initially deposit: ");
	scanf("%d", &initial_deposit);
	while (initial_deposit % 10 != 0) {
		printf("Enter an amount that is a multipe of 10: ");
		scanf("%d", &initial_deposit);
	}
	return initial_deposit;
}

char* transaction_time_set() {
	// To calculate date and time

	time_t      t;
	struct tm* ptm;
	t = time(NULL);
	ptm = localtime(&t);
	strftime(cur_time, 100 , "%d-%b-%Y %H:%M:%S", ptm);
	return &cur_time[0];
}
