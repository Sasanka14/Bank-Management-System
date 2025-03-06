#include <stdio.h>
#include <string.h>

void createAccount();
void depositAmount();
void withdrawAmount();
void balanceEnquiry();

const char* ACCOUNT_FILE = "account.dat";

typedef struct {
    int accountNumber;
    char name[50];
    float balance;
    int pin;
} Account;

int accountExists(int accountNumber) {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (!file) return 0;
    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accountNumber) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int verifyPin(int accountNumber, int pin) {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (!file) return 0;
    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accountNumber && acc.pin == pin) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int main() {
    while (1) {
        int choice;
        printf("\n *** Bank Management System *** ");
        printf("\n 1. Create Account");
        printf("\n 2. Deposit Amount");
        printf("\n 3. Withdraw Amount");
        printf("\n 4. Balance Enquiry");
        printf("\n 5. Exit");
        printf("\n Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                depositAmount();
                break;
            case 3:
                withdrawAmount();
                break;
            case 4:
                balanceEnquiry();
                break;
            case 5:
                printf("\n Thank you for using our banking system. Goodbye!");
                return 0;
            default:
                printf("\n Invalid choice");
        }
    }
}

void createAccount() {
    Account acc;
    FILE *file = fopen(ACCOUNT_FILE, "ab+");
    if (!file) {
        printf("\n Error opening file");
        return;
    }
    
    printf("\n Enter Your Name: ");
    getchar();
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';
    
    do {
        printf("\n Enter Your Account Number: ");
        scanf("%d", &acc.accountNumber);
        if (acc.accountNumber <= 0) {
            printf("\n Invalid account number. Please enter a positive integer.");
        } else if (accountExists(acc.accountNumber)) {
            printf("\n Account number already exists. Please choose another.");
        }
    } while (acc.accountNumber <= 0 || accountExists(acc.accountNumber));
    
    printf("\n Set a 4-digit PIN for authentication: ");
    scanf("%d", &acc.pin);
    
    acc.balance = 0;
    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);
    printf("\n Account created successfully!");
}

void depositAmount() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (!file) {
        printf("\n Error opening file");
        return;
    }
    
    int accountNumber;
    float amount;
    Account acc;
    
    printf("\n Enter Your Account Number: ");
    scanf("%d", &accountNumber);
    
    printf("\n Enter the amount to deposit: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("\n Invalid amount. Please enter a positive value.");
        fclose(file);
        return;
    }
    
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accountNumber) {
            acc.balance += amount;
            fseek(file, -(long)sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);
            fclose(file);
            printf("\n Successfully deposited Rs: %.2f", amount);
            printf("\n Updated Balance: Rs: %.2f\n", acc.balance);
            return;
        }
    }
    
    fclose(file);
    printf("\n Account No: %d does not exist.", accountNumber);
}

void withdrawAmount() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (!file) {
        printf("\n Error opening file");
        return;
    }
    
    int accountNumber, pin;
    float amount;
    Account acc;
    
    printf("\n Enter Your Account Number: ");
    scanf("%d", &accountNumber);
    printf("\n Enter Your PIN: ");
    scanf("%d", &pin);
    
    if (!verifyPin(accountNumber, pin)) {
        printf("\n Incorrect PIN. Transaction failed.");
        fclose(file);
        return;
    }
    
    printf("\n Enter the amount to withdraw: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("\n Invalid amount. Please enter a positive value.");
        fclose(file);
        return;
    }
    
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accountNumber) {
            if (acc.balance >= amount) {
                acc.balance -= amount;
                fseek(file, -(long)sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                fclose(file);
                printf("\n Successfully withdrawn Rs: %.2f", amount);
                printf("\n Updated Balance: Rs: %.2f\n", acc.balance);
                return;
            } else {
                printf("\n Insufficient balance");
                fclose(file);
                return;
            }
        }
    }
    fclose(file);
    printf("\n Account No: %d does not exist.", accountNumber);
}

void balanceEnquiry() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (!file) {
        printf("\n Error opening file");
        return;
    }
    
    int accountNumber, pin;
    Account acc;
    
    printf("\n Enter Your Account Number: ");
    scanf("%d", &accountNumber);
    printf("\n Enter Your PIN: ");
    scanf("%d", &pin);
    
    if (!verifyPin(accountNumber, pin)) {
        printf("\n Incorrect PIN.");
        fclose(file);
        return;
    }
    
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.accountNumber == accountNumber) {
            printf("\n Your Account Balance is: Rs: %.2f", acc.balance);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("\n Account No: %d not found", accountNumber);
}
