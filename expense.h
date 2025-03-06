#ifndef EXPENSE_H
#define EXPENSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME 100
#define CATEGORY_NUM 5
#define CATEGORY_NAME 15
#define FAM_MEMBERS 4
#define MAX_DAY 10

typedef enum
{
    rent,
    utility,
    grocery,
    stationary,
    leisure
} ExpenseCategory;

typedef enum
{
    FAILURE,
    SUCCESS
} status_code;

typedef struct individual_node
{
    int UserID;
    char *UserName;
    float income;
    int FamilyID;
    struct individual_node *nextindiv;
} individual;

typedef struct family_node
{
    int FamilyID;
    char *FamilyName;
    individual *familyMember;
    int fam_mem_ids[FAM_MEMBERS];
    float monthly_expense;
    float family_income;
    int number_of_members;
    struct family_node *nextfam;
} family;

typedef struct expense_node
{
    int expenseID;
    int userID_exp; // reference to individual node
    ExpenseCategory expense_category;
    float expense_amount;
    int date_of_expense;
    struct expense_node *next_exp;
} expense;

individual *add_member_to_family(individual *individual_start, family *family_start, int family_id, int user_id);
individual *add_user_node_sorted(individual *indivptr, individual *node_to_be_added);
family *add_family_node_sorted(family *famptr, family *node_to_be_added);
void writeUserToFile(individual *user);
status_code createindividual_node(individual **indivpptr, int Id, int famid, char *name, float income);
void writeFamilyToFile(family *fam);
status_code createFamilyNode(family **fampptr, int famid, char *famname, float fam_income, float fam_expense_monthly);
status_code find_individual(individual *indivptr, int userid, individual **temp);
status_code find_family(family *famptr, int famid, family **temp);
void deleteFamilyNode(family **familyptr, family *famfound);
status_code createFamily(family **famnode);
void updateFamiliesFile(family *famptr);
status_code AddUser(individual **individualptr, family *familyptr);
void deleteFamily(family **fam_head, family *fam_to_delete);
void updateUsersFile(individual *head);
status_code update_or_delete_individual_family_details(individual *indivptr, family *famptr);
void printindividual(individual *person);
void printfamily(family *fam);
void freeindividualList(individual *indivptr);
void freefamilyList(family *famptr);
void loadUsers(individual **indivpptr);
void loadFamilies(family **fampptr);
int getMonth(int date);
int getDay(int date);
void printCategoryExpenses(expense *exp_start, int userID, int month);
status_code totalExpenseFamily(family *fam_Start, expense *exp_start, int famid, float *amount);
status_code MaxExpenseFamily(family *fam_Start, expense *exp_start, int famid, int month, int *day, float *amount);
status_code createExpenseNode(expense **exp_pptr, int user_id, int exp_id, ExpenseCategory expense_category, float exp_amount, int date);
int isDuplicateExpense(expense *exp_start, int userID, int expenseID);
expense *add_node_sorted(expense *exp_start, expense *node_to_be_added);
status_code AddExpense(expense **exp_start, int expenseID, int userID, ExpenseCategory category, float amount, int date);
status_code updateExpense(expense *exp_start, int userID, int expenseID);
status_code deleteExpense(expense **exp_start, int userID, int expenseID);
void writeExpenseToFile(const char *filename, expense *exp_start);
void ReadExpensesFromFile(const char *filename, expense **exp_start);
void loadExpenses(expense **expptr);
void printExpense(expense *exp);
void freeExpenseList(expense *expptr);
void printIndividual(individual *indiv);
void printFamily(family *fam);
void freeIndividualList(individual *indivptr);
void freeFamilyList(family *famptr);
#endif // EXPENSE_H
