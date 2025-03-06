#include <stdio.h>
#include "expense.h"

int main()
{
    expense *expenseList = NULL;
    individual *individualList = NULL;
    family *familyList = NULL;
    status_code sc;
    loadExpenses(&expenseList);
    loadUsers(&individualList);
    loadFamilies(&familyList);

    


    int choice;
    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Add Expense\n");
        printf("2. Print Expenses\n");
        printf("3. Add User\n");
        printf("4. Print Users\n");
        printf("5. Add Family\n");
        printf("6. Print Families\n");
        printf("7. Update or delete user or family\n");
        printf("8. update expense\n");
        printf("9. delete expense\n");
        printf("10. get categorial expense of an user\n");
        printf("11. get total expense of a family\n");
        printf("12. get highest expense of the day by a family\n");
        printf("13.exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            int expid, date,userid;
            ExpenseCategory category;
            float expamt;

            printf("Enter Expense ID: ");
            scanf("%d", &expid);
            printf("Enter user ID: ");
            scanf("%d", &userid);
            printf("Enter Expense Categories \n 0.rent 1.utility 2.grocery 3.stationary 4.leisure ");
            scanf("%d",&category);
            printf("Enter Expense Amount: ");
            scanf("%f", &expamt);
            printf("Enter Date of Expense: ");
            scanf("%d", &date);

            sc=AddExpense(&expenseList, expid, userid, category, expamt,date);
            if(sc==SUCCESS)
            {
                writeExpenseToFile("expenses.txt",expenseList);
                printf("expense added successfully");
            }
            else
            {
                printf("saving unsuccessful");
            }
        }
        else if (choice == 2)
        {
            printExpense(expenseList);
        }
        else if (choice == 3)
        {
            int id, famid;
            char name[NAME];
            float income;
            printf("Enter UserID, Name, FamilyID, Income: ");
            scanf("%d %s %d %f", &id, name, &famid, &income);
            for(int i=0;i<FAM_MEMBERS;i++){

            }
            
            createindividual_node(&individualList, id, famid, name, income);
        }
        else if (choice == 4)
        {
            printIndividual(individualList);
        }
        else if (choice == 5)
        {
            int famid;
            char name[NAME];
            float income, expense;
            printf("Enter FamilyID, Name, Income, Expense: ");
            scanf("%d %s %f %f", &famid, name, &income, &expense);

            createFamilyNode(&familyList, famid, name, income, expense);
        }
        else if (choice == 6)
        {
            printFamily(familyList);
        }
        else if (choice == 7)
        {
            sc=update_or_delete_individual_family_details(individualList,familyList);
        }
        else if (choice == 8)
        {
            int userid,expenseid;
            printf("enter the user id and expense id of expense you want to update");
            scanf("%d %d",&userid,&expenseid );

            sc=updateExpense(expenseList,userid,expenseid);
            if(sc==SUCCESS)
            {
                writeExpenseToFile("expenses.txt",expenseList);
                printf("expense updated successfully");
            }
            else
            {
                printf("saving unsuccessful");
            }
        }
        else if (choice == 9)
        {
            int userid,expenseid;
            printf("enter the user id and expense id of expense you want to delete");
            scanf("%d %d",&userid,&expenseid );

            sc=deleteExpense(&expenseList,userid,expenseid);
            if(sc==SUCCESS)
            {
                writeExpenseToFile("expenses.txt",expenseList);
                printf("expense deleted successfully");
            }
            else
            {
                printf("saving unsuccessful");
            }
        }
        else if (choice == 10)
        {
            int userid,month;
            printf("enter the user id and month ");
            scanf("%d%d", &userid, &month);
            printCategoryExpenses(expenseList,userid,month);
        }
        else if (choice == 11)
        {
            int familyid;
            float amount;
            printf("enter the family id");
            scanf("%d ",&familyid);
            sc=totalExpenseFamily(familyList,expenseList,familyid,&amount);
            printf("the total expense family has done till now is %f",amount);
        }
        else if (choice == 12)
        {
            int familyid,month,day;
            float amount;
            printf("enter the family id and month ");
            scanf("%d %d ",&familyid,&month);
            sc=MaxExpenseFamily(familyList,expenseList,familyid,month,&day,&amount);
            printf("the max expense family has done till now in %d month is %f on day %d",month,amount,day);
        }
        else if (choice == 13)
        {
            printf("Exiting...\n");
            break;
        }
        else
        {
            printf("Invalid choice. Try again.\n");
        }
    }
    updateFamiliesFile(familyList);
    freeExpenseList(expenseList);
    freeIndividualList(individualList);
    freeFamilyList(familyList);
    return 0;
}
