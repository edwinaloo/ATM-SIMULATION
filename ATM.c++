#include <iostream>
#include <string>
#include <unordered_map>
#include <limits> // Added for std::numeric_limits

// Account Class: Encapsulates account details and provides methods for deposit, withdrawal, and checking balance
class Account {
private:
    std::string account_number;  // Private member to store account number
    std::string pin;             // Private member to store PIN
    double balance;              // Private member to store balance

public:
    // Constructor to initialize account details
    Account(const std::string& account_number, const std::string& pin, double balance = 0)
        : account_number(account_number), pin(pin), balance(balance) {}

    // Method to check balance
    double check_balance() const {
        return balance;
    }

    // Method to deposit amount
    bool deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            return true;
        }
        return false;
    }

    // Method to withdraw amount
    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    // Method to verify PIN
    bool verify_pin(const std::string& entered_pin) const {
        return pin == entered_pin;
    }

    // Method to get account number
    const std::string& get_account_number() const {
        return account_number;
    }
};

// Transaction Abstract Base Class: Represents a generic transaction
class Transaction {
protected:
    Account* account;  // Protected member to store account

public:
    // Constructor to initialize account
    Transaction(Account* account) : account(account) {}

    // Pure virtual method to execute transaction
    virtual bool execute() = 0;

    // Virtual destructor
    virtual ~Transaction() = default;
};

// Deposit Class: Represents a deposit transaction
class Deposit : public Transaction {
private:
    double amount;  // Private member to store deposit amount

public:
    // Constructor to initialize account and deposit amount
    Deposit(Account* account, double amount) : Transaction(account), amount(amount) {}

    // Method to execute deposit transaction
    bool execute() override {
        return account->deposit(amount);
    }
};

// Withdrawal Class: Represents a withdrawal transaction
class Withdrawal : public Transaction {
private:
    double amount;  // Private member to store withdrawal amount

public:
    // Constructor to initialize account and withdrawal amount
    Withdrawal(Account* account, double amount) : Transaction(account), amount(amount) {}

    // Method to execute withdrawal transaction
    bool execute() override {
        return account->withdraw(amount);
    }
};

// ATM Class: Handles ATM interactions and transactions
class ATM {
private:
    std::unordered_map<std::string, Account*> accounts;  // Private member to store accounts

public:
    // Method to add account to ATM
    void add_account(Account* account) {
        accounts[account->get_account_number()] = account;
    }

    // Method to verify account PIN
    Account* verify_pin(const std::string& account_number, const std::string& pin) {
        auto it = accounts.find(account_number);
        if (it != accounts.end() && it->second->verify_pin(pin)) {
            return it->second;
        }
        return nullptr;
    }

    // Method to select and execute transaction
    std::string select_transaction(Account* account, const std::string& transaction_type, double amount = 0) {
        Transaction* transaction = nullptr;
        if (transaction_type == "deposit") {
            transaction = new Deposit(account, amount);
        } else if (transaction_type == "withdraw") {
            transaction = new Withdrawal(account, amount);
        } else {
            return "Invalid transaction type";
        }

        bool success = transaction->execute();
        delete transaction;
        return success ? "Transaction successful" : "Transaction failed";
    }

    // Method to check account balance
    double check_balance(Account* account) const {
        return account->check_balance();
    }
};

// Function to clear the input buffer
void clear_input_buffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Function to display the main menu
void display_main_menu() {
    std::cout << "============================\n";
    std::cout << "Welcome to the ATM Machine\n";
    std::cout << "============================\n";
    std::cout << "1. Check Balance\n";
    std::cout << "2. Deposit\n";
    std::cout << "3. Withdraw\n";
    std::cout << "4. Exit\n";
    std::cout << "============================\n";
    std::cout << "Please select an option: ";
}

int main() {
    // Create accounts
    Account account1("123456", "1234", 1000);
    Account account2("654321", "4321", 500);

    // Create ATM and add accounts
    ATM atm;
    atm.add_account(&account1);
    atm.add_account(&account2);

    while (true) {
        std::string account_number, pin;
        std::cout << "Enter account number: ";
        std::cin >> account_number;
        std::cout << "Enter PIN: ";
        std::cin >> pin;

        Account* account = atm.verify_pin(account_number, pin);
        if (account) {
            int choice;
            do {
                display_main_menu();
                std::cin >> choice;
                clear_input_buffer();

                switch (choice) {
                case 1:
                    std::cout << "Your balance is: " << atm.check_balance(account) << "\n";
                    break;
                case 2: {
                    double amount;
                    std::cout << "Enter amount to deposit: ";
                    std::cin >> amount;
                    clear_input_buffer();
                    std::cout << atm.select_transaction(account, "deposit", amount) << "\n";
                    break;
                }
                case 3: {
                    double amount;
                    std::cout << "Enter amount to withdraw: ";
                    std::cin >> amount;
                    clear_input_buffer();
                    std::cout << atm.select_transaction(account, "withdraw", amount) << "\n";
                    break;
                }
                case 4:
                    std::cout << "Thank you for using the ATM. Goodbye!\n";
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
                }
                std::cout << "\n";
            } while (choice != 4);
            break;
        } else {
            std::cout << "Invalid account number or PIN. Please try again.\n";
        }
    }

    return 0;
}
