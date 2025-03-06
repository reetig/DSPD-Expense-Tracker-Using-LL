#include "expense.h"

expense *exp_Start = NULL;
// individual *indivptr=NULL;
// family *famptr=NULL;
 // Global start pointer for the expense list
// add users function sorted

individual *add_member_to_family(individual *individual_start, family *family_start, int family_id, int user_id)
{
    family* tempfam;
    status_code sc= find_family(family_start,family_id,&tempfam);
    individual* current= tempfam->familyMember;
    individual* prev;
    while(current)
    {
        prev=current;
        current=current->nextindiv;
    }
    individual* nptr;
    sc=find_individual(individual_start,user_id,&nptr);
    return add_user_node_sorted(individual_start,nptr);

}
individual *add_user_node_sorted(individual *indivptr, individual *node_to_be_added)
{
    // node to be added should not be null
    if (indivptr == NULL || indivptr->UserID > node_to_be_added->UserID)
    {
        node_to_be_added->nextindiv = indivptr;
        indivptr = node_to_be_added;
    }
    else
    {
        individual *current = indivptr;
        while (current->nextindiv != NULL && current->nextindiv->UserID < node_to_be_added->UserID)
        {
            current = current->nextindiv;
        }
        node_to_be_added->nextindiv = current->nextindiv;
        current->nextindiv = node_to_be_added;
    }
    return indivptr;
}

//  sorting family functions
family *add_family_node_sorted(family *famptr, family *node_to_be_added)
{
    if (famptr == NULL || famptr->FamilyID > node_to_be_added->FamilyID)
    {
        node_to_be_added->nextfam = famptr;
        famptr = node_to_be_added;
    }
    else
    {
        family *current = famptr;
        while (current->nextfam != NULL && current->nextfam->FamilyID < node_to_be_added->FamilyID)
        {
            current = current->nextfam;
        }
        node_to_be_added->nextfam = current->nextfam;
        current->nextfam = node_to_be_added;
    }
    return famptr;
}

// WORKS
// writing to users.txt
void writeUserToFile(individual *user)
{
    FILE *file = fopen("users.txt", "a"); // write new users,dont overwrite
    if (!file)
    {
        printf("Error opening file for writing!\n");
        return;
    }

    fprintf(file, "%d %s %d %.2f\n", user->UserID, user->UserName, user->FamilyID, user->income);
    fclose(file);
    printf("users added to users.txt successfully!");
}
// create node functions
// WORKS
status_code createindividual_node(individual **indivpptr, int Id, int famid, char *name, float income)
{
    individual *nptr;
    status_code sc = SUCCESS;

    nptr = (individual *)malloc(sizeof(individual));

    if (nptr == NULL)
    {
        free(nptr);
        sc = FAILURE;
    }
    else
    {
        nptr->UserID = Id;
        nptr->FamilyID = famid;
        nptr->UserName = (char *)malloc(strlen(name) + 1);
        if (nptr->UserName != NULL)
        {
            strcpy(nptr->UserName, name);
            nptr->income = income;
            nptr->nextindiv = NULL;

            // insertsort
            // when list is empty or the id is the smallest and have to be inserted in the end
            /*
            if (*indivpptr == NULL || (*indivpptr)->UserID > Id)
            {
                nptr->nextindiv = *indivpptr;
                *indivpptr = nptr;
            }
            else
            {
                // insert at correct postion
                individual *prev = *indivpptr;
                while (prev->nextindiv != NULL && prev->nextindiv->UserID < Id)
                {
                    prev = prev->nextindiv;
                }
                nptr->nextindiv = prev->nextindiv;
                prev->nextindiv = nptr;
            }
            */
            *indivpptr = add_user_node_sorted(*indivpptr, nptr);

            writeUserToFile(nptr);
        }
        else
        {
            sc = FAILURE;
        }
    }
    return sc;
}
// WORKS
void writeFamilyToFile(family *fam)
{
    FILE *file = fopen("families.txt", "a");
    if (!file)
    {
        printf("file could not open.");
        return;
    }

    fprintf(file, "%d %s %.2f %.2f %d ", fam->FamilyID, fam->FamilyName, fam->family_income, fam->monthly_expense, fam->number_of_members);
    fclose(file);
}
// WORKS
status_code createFamilyNode(family **fampptr, int famid, char *famname, float fam_income, float fam_expense_monthly)
{
    family *nptr;
    status_code sc = SUCCESS;
    int i=0;

    nptr = (family *)malloc(sizeof(family));

    if (nptr == NULL)
    {
        free(nptr);
        sc = FAILURE;
    }
    else
    {
        nptr->FamilyID = famid;
        nptr->FamilyName = (char *)malloc(strlen(famname) + 1);
        if (nptr->FamilyName == NULL)
        {
            free(nptr);
            sc = FAILURE;
        }
        else
        {
            strcpy(nptr->FamilyName, famname);
            nptr->family_income = fam_income;
            nptr->number_of_members = 1;
            nptr->monthly_expense = fam_expense_monthly;
            nptr->nextfam = NULL;
            
            *fampptr = add_family_node_sorted(*fampptr, nptr);
            writeFamilyToFile(nptr);
        }
    }
    return sc;
}

// traverse the individual list
// WORKS
status_code find_individual(individual *indivptr, int userid, individual **temp)
{
    status_code found = FAILURE;
    individual *nptr = indivptr;
    while (nptr != NULL && found == FAILURE)
    {
        if (nptr->UserID == userid)
        {
            found = SUCCESS;
            if (temp != NULL)
            {
                *temp = nptr;
            }
        }
        else
        {
            nptr = nptr->nextindiv;
        }
    }
    return found;
}
// find family
// WORKS
status_code find_family(family *famptr, int famid, family **temp)
{
    status_code found = FAILURE;
    family *nptr = famptr;
    while (nptr != NULL && found == FAILURE)
    {
        if (nptr->FamilyID == famid)
        {
            found = SUCCESS;
            if (temp != NULL)
            {
                *temp = nptr;
            }
        }
        else
        {
            nptr = nptr->nextfam;
        }
    }
    return found;
}

// delete family  node
// deletes only the family node
void deleteFamilyNode(family **familyptr, family *famfound)
{
    if (familyptr == NULL || *familyptr == NULL || famfound == NULL)
    {
        printf("Error: Family list is empty or family not found.\n");
        return;
    }

    if (*familyptr == famfound) // If the family to delete is the head
    {
        *familyptr = famfound->nextfam;
    }
    else
    {
        family *temp = *familyptr;
        while (temp->nextfam != NULL && temp->nextfam != famfound)
        {
            temp = temp->nextfam;
        }
        if (temp->nextfam != NULL) // Found the family to delete
        {
            temp->nextfam = famfound->nextfam; // Unlink it from the list
            free(famfound);
            famfound = NULL;
        }
    }

    updateFamiliesFile(*familyptr);
}

// create Family=>WORKS
status_code createFamily(family **famnode)
{
    printf("i entered create family function");
    int fid;
    char fname[NAME];
    float fincome;
    float fexpmon;
    status_code sc = SUCCESS;
    family *nptr;
    printf("Enter the family ID: ");
    scanf("%d", &fid);

    status_code family_exist = FAILURE;
    if (famnode != NULL)
    {
        status_code family_exist = find_family(*famnode, fid, NULL);
    }

    if (family_exist == SUCCESS)
    {
        printf("\nFamily already exists.");
        sc = FAILURE;
    }
    else
    {
        // a new family is created
        printf("\nEnter your family name: ");
        scanf("%s", fname);
        printf("\nEnter total family  income: ");
        scanf("%f", &fincome);
        printf("\nEnter total family monthly expense: ");
        scanf("%f", &fexpmon);

        sc = createFamilyNode(&nptr, fid, fname, fincome, fexpmon);
        *famnode=add_family_node_sorted(*famnode,nptr);
    }
    printf("ended create family function");
    return sc;
}
// WORKS
// updating the files
// completely rewrites the files for updation deletion or additions
void updateFamiliesFile(family *famptr)
{
    FILE *file = fopen("families.txt", "w");
    if (!file)
    {
        printf("error");
        return;
    }

    family *temp = famptr;
    while (temp)
    {
        individual* current=temp->familyMember;
        fprintf(file, "%d %s %.2f %.2f %d\n", temp->FamilyID, temp->FamilyName, temp->family_income, temp->monthly_expense, temp->number_of_members);
        while(current)
        {
            fprintf(file,"%d",current->UserID);
            current=current->nextindiv;
        }
        fprintf(file, "---\n");
        temp = temp->nextfam;
    }
    fclose(file);
}

// Adding user
// WORKS
status_code AddUser(individual **individualptr, family *familyptr)
{
    printf("Please enter the userID of the individual: ");
    int id;
    scanf("%d", &id);
    status_code sc = SUCCESS;
    status_code user_exists = find_individual(*individualptr, id, NULL);
    if (user_exists == SUCCESS)
    {
        printf("\nUser already exists.");
        sc = FAILURE;
    }
    else
    {
        printf("Please enter the familyID of the individual: ");
        int familyid;
        scanf("%d", &familyid);
        printf("\nPlease enter the name of the individual: ");
        char *username = (char *)malloc(NAME);
        scanf("%s", username);
        printf("\nPlease enter the income of the individual: ");
        float income;
        scanf("%f", &income);
        // works in line above it
        individual *lptr = *individualptr;

        sc = createindividual_node(individualptr, id, familyid, username, income);
        printf("\nindividual node created.");

        // check for if family id exists

        status_code family_exists = FAILURE;
        family *found_family = NULL;
        family_exists = find_family(familyptr, familyid, &found_family);
        if (family_exists == FAILURE)
        {
            printf("\nFamily doesn't exist. Creating a new family.\n");
            createFamily(&familyptr);
        }
        else
        {
            if (found_family->number_of_members < FAM_MEMBERS)
            {
                // // Correctly add user to family
                individual *temp = found_family->familyMember;
                individual* prev;
                individual* node_to_be_added;
                status_code sc2;
                sc2=find_individual(*individualptr,id,&node_to_be_added);
                // found_family->familyMember = *individualptr;
                // (*individualptr)->nextindiv = temp; // Insert at start
                // (found_family->number_of_members)++;
                while(temp->nextindiv)
                {
                    prev=temp;
                    temp=temp->nextindiv;
                }
                prev->nextindiv=node_to_be_added;
                found_family->number_of_members++;
            }
            else
            {
                printf("\nFamily is full, cannot add more members.\n");
                sc = FAILURE;
            }
        }

        free(username);
    }
    printf("add user function is ended.");
    // we need to sort it to uska logic baadme filhal ke liye
    return sc;
}
// delete family and corresponding individual
void deleteFamily(family **fam_head, family *fam_to_delete)
{
    if (fam_head == NULL || *fam_head == NULL)
    {
        printf("Error: Family list is empty.\n");
        return;
    }

    if (fam_to_delete == NULL)
    {
        printf("Error: Family to delete is NULL.\n");
        return;
    }

    printf("Deleting Family ID: %d, Name: %s\n", fam_to_delete->FamilyID, fam_to_delete->FamilyName);

    // Step 1: Delete all individuals in the family
    individual *delindivs = fam_to_delete->familyMember;
    while (delindivs != NULL)
    {
        individual *temp = delindivs;
        delindivs = delindivs->nextindiv;

        free(temp->UserName); // Free dynamically allocated name
        free(temp);           // Free individual node
    }
    fam_to_delete->familyMember = NULL; // Prevent dangling pointer

    // Step 2: Remove the family from the linked list
    if (*fam_head == fam_to_delete)
    {
        // If the family to delete is the head of the list
        *fam_head = fam_to_delete->nextfam;
    }
    else
    {
        // Traverse the list to find the previous node
        family *prev = *fam_head;
        while (prev->nextfam != NULL && prev->nextfam != fam_to_delete)
        {
            prev = prev->nextfam;
        }

        if (prev->nextfam == fam_to_delete)
        {
            // Unlink the family node from the list
            prev->nextfam = fam_to_delete->nextfam;
        }
        else
        {
            printf("Error: Family not found in the list.\n");
            return;
        }
    }

    // Step 3: Free the family node itself
    free(fam_to_delete->FamilyName); // Free dynamically allocated family name
    free(fam_to_delete);
    fam_to_delete = NULL; // Prevent dangling pointer (local to this function)

    // Step 4: Update the families file
    updateFamiliesFile(*fam_head);
    printf("Family deleted successfully.\n");
}
void updateUsersFile(individual *head)
{
    FILE *file = fopen("users.txt", "w"); // Open in write mode to overwrite
    if (!file)
    {
        printf("error");
        return;
    }

    individual *temp = head;
    while (temp)
    {
        fprintf(file, "%d %s %d %.2f\n", temp->UserID, temp->UserName, temp->FamilyID, temp->income);
        temp = temp->nextindiv;
    }
    fclose(file);
}
// NOT WORKING
status_code update_or_delete_individual_family_details(individual *indivptr, family *famptr)
{
    int choice;
    printf("Do you want to \n 1.Update individual details.\n 2. Update Family details.\n 3.Delete individual\n 4.Delete Family\n");
    scanf("%d", &choice);

    if (choice == 1) // Update Individual
    {                // WORKS
        int userid;
        printf("Enter the user ID for whom you want to update details: ");
        scanf("%d", &userid);

        individual *user_node;
        status_code user_exists = find_individual(indivptr, userid, &user_node);

        if (user_exists == SUCCESS)
        {
            char name[NAME];
            printf("\nEnter the new name you want to keep: ");
            scanf("%s", name);
            free(user_node->UserName); // Free old memory
            user_node->UserName = strdup(name);

            printf("\nEnter the updated income: ");
            float updated_income;
            scanf("%f", &updated_income);
            user_node->income = updated_income;
            updateUsersFile(indivptr);
            printf("updated user\n");

            int famid = user_node->FamilyID;
            family *update_family_node;
            status_code family_exists = find_family(famptr, famid, &update_family_node);

            if (family_exists == SUCCESS)
            {
                update_family_node->familyMember = user_node;
                updateUsersFile(indivptr);
            }
            else
            {
                printf("Error: Family not found.\n");
            }
        }
        else
        {
            printf("\nUser does not exist.\n");
        }
    }
    else if (choice == 2) // Update Family
    {
        // WORKS
        int famid;
        printf("\nEnter your family ID to update details: ");
        scanf("%d", &famid);

        family *update_family_node;
        status_code family_exists = find_family(famptr, famid, &update_family_node);

        if (family_exists == SUCCESS)
        {
            char familyname[NAME];
            printf("Enter updated family name: ");
            scanf("%s", familyname);
            strcpy(update_family_node->FamilyName, familyname);

            printf("Enter updated total family income: ");
            float newincome;
            scanf("%f", &newincome);
            update_family_node->family_income = newincome;

            printf("Enter updated family monthly expense: ");
            float expense;
            scanf("%f", &expense);
            update_family_node->monthly_expense = expense;

            updateFamiliesFile(famptr);
        }
        else
        {
            printf("Error: Family not found.\n");
        }
    }
    else if (choice == 3) // Delete Individual
    {
        // WORKS
        int userid;
        printf("Enter the user ID to delete: ");
        scanf("%d", &userid);

        individual *user_node;
        status_code user_exists = find_individual(indivptr, userid, &user_node);

        if (user_exists == SUCCESS)
        {
            int famid = user_node->FamilyID;
            family *famdelnode;
            status_code famdel = find_family(famptr, famid, &famdelnode);

            // Remove user from linked list
            if (indivptr == user_node)
            {
                indivptr = user_node->nextindiv;
            }
            else
            {
                individual *prev = indivptr;
                while (prev->nextindiv != NULL && prev->nextindiv != user_node)
                {
                    prev = prev->nextindiv;
                }
                if (prev->nextindiv == user_node)
                {
                    prev->nextindiv = user_node->nextindiv;
                }
            }

            free(user_node->UserName);
            free(user_node);

            if (famdel == SUCCESS)
            {
                famdelnode->number_of_members--;

                if (famdelnode->number_of_members == 0)
                {
                    deleteFamilyNode(&famptr, famdelnode);
                    printf("\nFamily deleted as last member was removed.\n");
                }
                else
                {
                    updateFamiliesFile(famptr);
                }
            }

            updateUsersFile(indivptr);
        }
        else
        {
            printf("User does not exist.\n");
        }
    }
    else if (choice == 4) // Delete Family
    {

        int famid;
        printf("Enter the family ID to delete: ");
        scanf("%d", &famid);

        family *fam_found = NULL;
        status_code family_found = find_family(famptr, famid, &fam_found);

        if (family_found == SUCCESS)
        {
            deleteFamily(&famptr, fam_found);

            printf("\nFamily and members deleted.\n");
        }
        else
        {
            printf("\nFamily does not exist!\n");
        }
    }

    return SUCCESS;
}
// WORKS
void printindividual(individual *person)
{
    if (!person)
    {
        printf("No users found.\n");
    }
    else
    {
        while (person != NULL)
        {
            printf("UserID: %d, Name: %s, FamilyID: %d, Income: %.2f\n",
                   person->UserID, person->UserName, person->FamilyID, person->income);
            person = person->nextindiv;
        }
    }
}
// WORKS
void printfamily(family *fam)
{
    if (!fam)
    {
        printf("No families found.\n");
    }
    else
    {
        while (fam != NULL)
        {
            printf("FamilyID: %d, Name: %s, Income: %.2f, Monthly Expense: %.2f, Members: %d\n",
                   fam->FamilyID, fam->FamilyName, fam->family_income, fam->monthly_expense, fam->number_of_members);
            individual*temp=fam->familyMember;
            while(temp)
            {
                printf("% d", temp);
                temp=temp->nextindiv;
            }
            fam = fam->nextfam;
        }
    }
}
// WORKS
void freeindividualList(individual *indivptr)
{
    individual *temp;
    while (indivptr != NULL)
    {
        temp = indivptr;
        indivptr = indivptr->nextindiv;
        free(temp->UserName);
        free(temp);
    }
}

void freefamilyList(family *famptr)
{
    family *temp;
    while (famptr != NULL)
    {
        temp = famptr;
        famptr = famptr->nextfam;
        freeindividualList(temp->familyMember); // Free individuals first
        free(temp->FamilyName);
        free(temp);
    }
}
// WORKS
// read files
void loadUsers(individual **indivpptr)
{
    FILE *file = fopen("users.txt", "r");
    if (!file)
    {
        printf("error");
        return;
    }

    individual *tempList = NULL;

    int id, famid;
    char name[NAME];
    float income;
    // gets stored in the memory and prevents to be overwritten

    while (fscanf(file, "%d %s %d %f", &id, name, &famid, &income) == 4)
    {
        individual *newUser = (individual *)malloc(sizeof(individual));
        newUser->UserID = id;
        newUser->FamilyID = famid;
        newUser->UserName = strdup(name); // duplicates a string by allocating memory dynamically
        newUser->income = income;
        newUser->nextindiv = tempList;
        tempList = newUser;
    }
    fclose(file);

    // Sort users before storing them in memory
    while (tempList)
    {
        individual *next = tempList->nextindiv;
        tempList->nextindiv = NULL;
        *indivpptr = add_user_node_sorted(*indivpptr, tempList);
        tempList = next;
    }
}
// WORKS
void loadFamilies(family **fampptr)
{
    FILE *file = fopen("families.txt", "r");
    if (!file)
    {
        printf("error");
        return;
    }

    family *tempList = NULL;

    int famid, members;
    char name[NAME];
    float income, expense;

    // while (fscanf(file, "%d %s %f %f %d", &famid, name, &income, &expense, &members) == 5)
    // {
    family *newFamily = (family *)malloc(sizeof(family));
    //     newFamily->FamilyID = famid;
    //     newFamily->FamilyName = strdup(name);
    //     newFamily->family_income = income;
    //     newFamily->monthly_expense = expense;
    //     newFamily->number_of_members = members;
    //     newFamily->nextfam = tempList;
    //     tempList = newFamily;
    // }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d %s %f %f %d",
                   &(newFamily->FamilyID), newFamily->FamilyName, &(newFamily->family_income), &(newFamily)->monthly_expense, &(newFamily)->number_of_members) == 5)
                {
                    while (fgets(line, sizeof(line), file)) 
                    {
                        if (strcmp(line, "---\n") == 0) 
                        {
                            break;
                        }
                        individual*temp =newFamily->familyMember;
                        if(sscanf(line,"%d",&temp))
                        {
                            temp=temp->nextindiv;
                        }
                    }
                    family *next = tempList->nextfam;
                    *fampptr = add_family_node_sorted(*fampptr, tempList);
                }
    fclose(file);
    }
}

// Helper function to get the month from the date (DDMMYYYY)
int getMonth(int date)
{
    return (date / 10000) % 100; // Extract month (MM) from DDMMYYYY
}
int getDay(int date)
{
    return (date / 1000000); // Extract month (MM) from DDMMYYYY
}

// Function to print categorized expenses for a user in a given month
void printCategoryExpenses(expense *exp_start, int userID, int month) {
    printf("inside print category");
    expense *categoryExpenses[CATEGORY_NUM] = {NULL, NULL, NULL, NULL, NULL};  // One list per category
    float categoryTotal[CATEGORY_NUM] = {0};  // To track total spent per category
    int category_map[CATEGORY_NUM]={0,1,2,3,4};
    
    // Traverse the linked list and filter expenses by user and month
    expense *current = exp_start;
    while (current != NULL) {
        if (current->userID_exp == userID && getMonth(current->date_of_expense) == month) {
            expense *newExpense = (expense *)malloc(sizeof(expense));
            *newExpense = *current;  // Copy the current expense details
            newExpense->next_exp = categoryExpenses[current->expense_category];
            categoryExpenses[current->expense_category] = newExpense;
            categoryTotal[current->expense_category] += current->expense_amount;
        }
        current = current->next_exp;
    }
    int flag=0;
    expense *temp;
    float temp2;
    int temp_map=0;
    for(int i=0;i<CATEGORY_NUM && flag==0;i++)
    {
        flag=0;
        for(int j=0;j<CATEGORY_NUM-i-1;j++)
        {
            if(categoryTotal[j+1]>categoryTotal[j])
            {
                // temp=categoryExpenses[j];
                // categoryExpenses[j]=categoryExpenses[j+1];
                // categoryExpenses[j+1]=temp;
                temp_map=category_map[j];
                category_map[j]=category_map[j+1];
                category_map[j+1]=temp_map;

                temp2=categoryTotal[j];
                categoryTotal[j]=categoryTotal[j+1];
                categoryTotal[j+1]=temp2;
            }
        }
    }

    for(int i=0;i<CATEGORY_NUM;i++)
    {
        if(categoryTotal[i]>0)
        {
            printf("Category: %d (Total: %f)\n",category_map[i],categoryTotal[i]);
            temp=categoryExpenses[i];
            while(temp!=NULL)
            {
                printf("Expense ID: %d, Amount: %f, Date: %d\n", temp->expenseID, temp->expense_amount, temp->date_of_expense);
                temp=temp->next_exp;
            }
            printf("\n");
        }
    }
}
status_code totalExpenseFamily(family* fam_Start, expense *exp_start, int famid, float *amount )
{
    family *family_temp;
    status_code sc= find_family(fam_Start,famid,&family_temp);
    individual* indivtemp;
    expense *current =exp_start;
    float total=0;
    if(sc==SUCCESS){
    while(current)
    {
        indivtemp=family_temp->familyMember;
        while(indivtemp)
        {
            if(current->userID_exp==indivtemp->UserID)
            {
                total=total+current->expense_amount;
            }
            indivtemp=indivtemp->nextindiv;
        }
        current=current->next_exp;
    }
    *amount=total;
    }
    else
    {
        printf("no family with this family id found");
    }
    return sc;
}

status_code MaxExpenseFamily(family* fam_Start, expense *exp_start, int famid,int month, int* day, float *amount )
{
    family *family_temp;
    status_code sc= find_family(fam_Start,famid,&family_temp);
    individual* indivtemp;
    expense *current =exp_start;
    float total[MAX_DAY]={0};
    int i=0,max_index=0;
    float max=0;
    if(sc == SUCCESS)
    {
        while(current)
        {
                indivtemp=family_temp->familyMember;
                while(indivtemp)
                {
                    if(current->userID_exp == indivtemp->UserID && getMonth(current->date_of_expense)==month)
                    {
                        total[getDay(current->date_of_expense)]=total[getDay(current->date_of_expense)]+current->expense_amount;
                    }
                    indivtemp=indivtemp->nextindiv;
                }
                current=current->next_exp;
        }
        for(i=0;i<MAX_DAY;i++)
        {
            if(total[i]>max)
            {
                max=total[i];
                max_index=i;
            }
        }  
        *amount=max;
        *day=max_index;
    }
    else
    {
        printf("no family with this family id found");
    }

    return sc;
}

status_code createExpenseNode(expense **exp_pptr, int user_id, int exp_id, ExpenseCategory expense_category, float exp_amount, int date)
{
    expense *nptr;
    status_code sc = SUCCESS;

    nptr = (expense *)malloc(sizeof(expense));
    if (nptr == NULL)
    {
        sc = FAILURE;
    }
    else
    {
        nptr->userID_exp = user_id;
        nptr->expenseID = exp_id;
        nptr->expense_amount = exp_amount;
        nptr->expense_category = expense_category;
        nptr->date_of_expense = date;
        nptr->next_exp = NULL;
        *exp_pptr = nptr;
    }
    return sc;
}

int isDuplicateExpense(expense *exp_start, int userID, int expenseID)
{
    expense *current = exp_start;
    while (current != NULL)
    {
        if (current->userID_exp == userID && current->expenseID == expenseID)
        {
            return 1; // Duplicate found
        }
        current = current->next_exp;
    }
    return 0; // No duplicate found
}

expense *add_node_sorted(expense *exp_start, expense *node_to_be_added)
{
    if (exp_start == NULL || exp_start->userID_exp > node_to_be_added->userID_exp ||
        (exp_start->userID_exp == node_to_be_added->userID_exp && exp_start->expenseID > node_to_be_added->expenseID))
    {
        node_to_be_added->next_exp = exp_start;
        exp_start = node_to_be_added;
    }
    else
    {
        expense *current = exp_start;
        while (current->next_exp != NULL &&
               (current->next_exp->userID_exp < node_to_be_added->userID_exp ||
                (current->next_exp->userID_exp == node_to_be_added->userID_exp && current->next_exp->expenseID < node_to_be_added->expenseID)))
        {
            current = current->next_exp;
        }
        node_to_be_added->next_exp = current->next_exp;
        current->next_exp = node_to_be_added;
    }
    return exp_start;
}

status_code AddExpense(expense **exp_start, int expenseID, int userID, ExpenseCategory category, float amount, int date)
{
    if (isDuplicateExpense(*exp_start, userID, expenseID))
    {
        printf("Error: Expense ID %d already exists. Cannot add duplicate expense.\n", expenseID);
        return FAILURE;
    }

    expense *nptr;
    status_code sc = createExpenseNode(&nptr, userID, expenseID, category, amount, date);
    if (sc == SUCCESS)
    {
        *exp_start = add_node_sorted(*exp_start, nptr);
        printf("Expense added successfully!\n");
    }
    else
    {
        printf("Some error occurred!\n");
    }
    return sc;
}

status_code updateExpense(expense *exp_start, int userID, int expenseID)
{
    expense *current = exp_start;
    while (current != NULL)
    {
        if (current->userID_exp == userID && current->expenseID == expenseID)
        {
            printf("Expense found: ID: %d, Category: %d, Amount: %.2f, Date: %d\n",
                   current->expenseID, current->expense_category, current->expense_amount, current->date_of_expense);
            char choice;
            printf("Do you want to update this expense? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y')
            {
                printf("Enter new amount: ");
                scanf("%f", &current->expense_amount);
                printf("Enter new date (DDMMYYYY): ");
                scanf("%d", &current->date_of_expense);
                printf("Expense updated successfully!\n");
                return SUCCESS;
            }
            else
            {
                printf("No changes made.\n");
                return FAILURE;
            }
        }
        current = current->next_exp;
    }
    printf("Expense with ID %d not found for user %d.\n", expenseID, userID);
    return FAILURE;
}

status_code deleteExpense(expense **exp_start, int userID, int expenseID)
{
    expense *current = *exp_start;
    expense *previous = NULL;
    while (current != NULL)
    {
        if (current->userID_exp == userID && current->expenseID == expenseID)
        {
            if (previous == NULL)
            {
                *exp_start = current->next_exp; // Removing the first node
            }
            else
            {
                previous->next_exp = current->next_exp; // Removing a middle node
            }
            free(current); // Free the memory
            printf("Expense deleted successfully!\n");
            return SUCCESS;
        }
        previous = current;
        current = current->next_exp;
    }
    printf("Expense with ID %d not found for user %d.\n", expenseID, userID);
    return FAILURE;
}

void writeExpenseToFile(const char *filename, expense *exp_start)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file for writing!\n");
        return;
    }

    expense *current = exp_start;
    while (current != NULL)
    {
        fprintf(file, "Expense ID: %d, User ID: %d, Category: %d, Amount: %.2f, Date: %d\n",
                current->expenseID, current->userID_exp, current->expense_category, current->expense_amount, current->date_of_expense);
        current = current->next_exp;
    }

    fclose(file);
    printf("Expenses written to file successfully!\n");
}

void ReadExpensesFromFile(const char *filename, expense **exp_start)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("File does not exist. Creating new file...\n");
        return; // File doesn't exist, we won't proceed further
    }

    int expenseID, userID, date;
    float amount;
    ExpenseCategory category;
    char line[100];

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "Expense ID: %d, User ID: %d, Category: %d, Amount: %f, Date: %d",
               &expenseID, &userID, &category, &amount, &date);
        AddExpense(exp_start, expenseID, userID, category, amount, date);
    }

    fclose(file);
    printf("Expenses read from file and added to the linked list!\n");
}


void loadExpenses(expense **expptr) {
    ReadExpensesFromFile("expenses.txt", expptr); // If ReadExpensesFromFile exists
}

void printExpense(expense *exp) {
    while (exp) {
        printf("Expense ID: %d, User ID: %d, Category: %d, Amount: %.2f, Date: %d\n",
               exp->expenseID, exp->userID_exp, exp->expense_category, exp->expense_amount, exp->date_of_expense);
        exp = exp->next_exp;
    }
}

void freeExpenseList(expense *expptr) {
    expense *temp;
    while (expptr != NULL) {
        temp = expptr;
        expptr = expptr->next_exp;
        free(temp);
    }
}


void printIndividual(individual *indiv) {
    while (indiv) {
        printf("User ID: %d,  family id: %d, User name: %s, income: %.2f\n",
               indiv->UserID, indiv->FamilyID, indiv->UserName, indiv->income);
        indiv = indiv->nextindiv;
    }
}

void printFamily(family *fam) {
    if (!fam)
    {
        printf("No families found.\n");
    }
    else
    {
        while (fam != NULL)
        {
            printf("FamilyID: %d, Name: %s, Income: %.2f, Monthly Expense: %.2f, Members: %d\n",
                   fam->FamilyID, fam->FamilyName, fam->family_income, fam->monthly_expense, fam->number_of_members);
            fam = fam->nextfam;
        }
    }
    // while (fam) {
    //     printf("family id: %d family name:%s family income:%.2f family expense%.2f ", fam->FamilyID, fam->FamilyName, fam->family_income, fam->monthly_expense);
    //     individual *temp=fam->familyMember;
    //     while(temp)
    //     {
    //         printf("member in family:%d  ",temp);
    //         temp=temp->nextindiv;
    //     }
    //     fam = fam->nextfam;
    // }
}

void freeIndividualList(individual *indivptr) {
    individual *temp;
    while (indivptr != NULL) {
        temp = indivptr;
        indivptr = indivptr->nextindiv;
        free(temp);
    }
}

void freeFamilyList(family *famptr) {
    family *temp;
    while (famptr != NULL) {
        temp = famptr;
        famptr = famptr->nextfam;
        free(temp);
    }
}

