// CHANG SHIAU HUEI
// TP060322

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define FILENAME_SIZE 100
#define MODEL_SIZE 100
#define WAREHOUSE_CHOICE_SIZE 100
#define PART_ID_SIZE 100
#define PART_NAME_SIZE 100
#define SUP_ID_SIZE 100
#define SUP_NAME_SIZE 100
#define MAX_SIZE_OF_SUPPLIER_NO 6                   // 5 digits max
#define MAX_SIZE_OF_PART_NO 6                       // 5 digits max
#define MAX_PER_LINE_SIZE 200                       // Maximum characters per line in a file
#define MAX_SIZE_OF_QUANTITY 6                      // 5 digits max

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Warehouse {
    char warehouseChoice[WAREHOUSE_CHOICE_SIZE];
    char partID[PART_ID_SIZE];
    char partName[PART_NAME_SIZE];
    int partQuantity;
} warehouse;

typedef struct Supplier {
    char supID[SUP_ID_SIZE];
    char supName[SUP_NAME_SIZE];
} supplier;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print customized separator
void printingSeparator(int length, char item) {
    for (int i = 0; i < length; i++)
        printf("%c", item);
}

// To count how many lines are in the file
int lineCount(char fileUsed[FILENAME_SIZE]) {
    int lineCount = 0;
    char chr;
    FILE *fptr;
    fptr = fopen(fileUsed, "r");

    chr = getc(fptr);                   // Extract character from file and store in chr
    while (chr != EOF) {
        if (chr == '\n')                // Count whenever a new line is encountered
            lineCount = lineCount + 1;
        chr = getc(fptr);               // Take next character from file
    }

    fclose(fptr);
    return lineCount;
}

// Clears input buffer -> To prevent scanf() duplicate when >1 characters are inserted
void seekToNextLine() {
    int c;
    while((c = fgetc(stdin)) != EOF && c != '\n');
}

// Make sure the text printed is in the center
void messageCentered(char *message) {
    int length;
    int position;

    length = (69 - strlen(message)) / 2; // Calculate the number of spaces that need to be printed

    for(position = 0; position < length; position++)
        printf(" ");                     // Print that many spaces

    printf("%s", message);               // Print the messages displayed through the functions
}

// Exiting program message
void exitingProgram(int status) {
    if (status == 1)
        printf("Thank you for using this system :D\n");
    if (status == 2)
        printf("Entry does not exist. Exiting program...\n");
    if (status == 3)
        printf("Invalid input. Exiting program...\n");
    if (status == 4)
        printf("Error opening files(s). Exiting program...\n");
    if (status == 5)
        printf("Entry already exists. Exiting program...\n");
    if (status == 6)
        printf("No records exist. Exiting system...\n");
    printingSeparator(69, '=');
    exit(0);
}

// Check if the entry already exists (partFinder == 1) in the file chosen
int entryExists(char *entry, FILE *file) {
    int partFinder = 0;
    char lineInFile[MAX_PER_LINE_SIZE];

    while (fgets(lineInFile, MAX_PER_LINE_SIZE, file) != NULL) {    // Check by line
        if ((strstr(lineInFile, entry)) != NULL)                    // Increases partFinder when the entry exists
            partFinder++;
        if (partFinder == 1)       // Part exists
            return partFinder;
    }
    return partFinder;             // Part does not exist
}

// Check if Part ID exists and return its respective filename, if no, exits program
char *entryCheckForExistencePartID(char *entryPartID) {
    int flag = 0;
    while (1) {
        printf("Please insert your Part ID");
        printf("\nEnter 'X' to quit:");
        scanf("%[^\n]s", entryPartID);                         // Getting input until a newline is scanned
        strupr(entryPartID);
        seekToNextLine();

        if (strcmpi(entryPartID, "X") == 0) {                  // If input is "X", exit program
            exitingProgram(1);
        } else if (strlen(entryPartID) > PART_ID_SIZE) {       // Validating input length
            printf("\nPlease input your Part ID within %d characters.\n", PART_ID_SIZE);
            continue;
        } else if (lineCount("supplier.txt") > 0) {
            // Extract warehouse info from partID to assign the respective warehouse .txt file
            char warehouse[PART_ID_SIZE];
            snprintf(warehouse, PART_ID_SIZE, "%.3s", entryPartID); // snprintf(target, sizeof target, "%.10s", source);

            // Assigning the respective warehouse .txt file
            char fileName[FILENAME_SIZE];
            strcat(fileName, warehouse);
            strcat(fileName, ".txt");

            FILE *fptr;
            fptr = fopen(fileName, "r");

            /*
             Check file content if user input item already exists, if no (flag = 0), exits program
             flag = 1 (Exist in file)
             flag = 0 (Doesn't exist in file)
            */
            flag = entryExists(entryPartID, fptr);
            fclose(fptr);
            if (flag == 1) {
                if (strcmpi(fileName,"WBZ.txt") == 0){
                    return "WBZ.txt";
                }
                if (strcmpi(fileName,"WSL.txt") == 0){
                    return "WSL.txt";
                }
                if (strcmpi(fileName,"WAR.txt") == 0) {
                    return "WAR.txt";
                }
            } else {
                exitingProgram(2); // Exit the program -> An entry doesn't exist
            }
        }
    }
}

// Check if an entry already exists and exits the program if so
void entryCheckForDuplicate(char fileUsed[FILENAME_SIZE], char item[10], int maxSizeOfItem, char *entry) {
    int flag = 1;
    FILE *fptr;
    fptr = fopen(fileUsed, "r");

    while (1) {
        printf("Please insert the %s that you want to register", item);
        printf("\nEnter 'X' to quit:");
        scanf("%[^\n]s", entry);                         // Getting input until a newline is scanned
        strupr(entry);
        seekToNextLine();

        if (strcmpi(entry, "X") == 0) {                  // If input is "X", exit program
            exitingProgram(1);
        } else if (strlen(entry) > maxSizeOfItem) {      // Validating input length
            printf("\nPlease input a %s within %d characters.\n", item, maxSizeOfItem);
            continue;
        } else if (lineCount(fileUsed) > 0) {
            /*
             Check file content if user input item already exists, if yes (flag = 1), exits program
             flag = 1 (Exist in file)
             flag = 0 (Doesn't exist in file)
            */
            flag = entryExists(entry, fptr);
            fclose(fptr);
            if (flag == 0)
                break;
            else
                exitingProgram(5); // Exiting program -> Entry already exists
        } else
            break;
    }
}

// Replace the line
// Write all the lines of the original file to a temp file EXCEPT the line we want to replace (Write a replacement line in its place)
// Delete the original file
// Rename the temp file to the original file's name
void replaceLine(char fileUsed[FILENAME_SIZE], int lineNum, char replaceText[MAX_PER_LINE_SIZE]) {
    FILE *filePtr, *tempPtr;
    char buffer[MAX_PER_LINE_SIZE];                  // Buffer will store each line from the original file

    // Open file for reading and writing
    filePtr = fopen(fileUsed, "r");
    tempPtr = fopen("tempFile.txt", "w");            // Making the temporary file

    if (filePtr == NULL || tempPtr == NULL)          // Check if either file failed to open, if either did exit with error status
        exitingProgram(4);                     // Exit program -> Error opening files

    bool keepReading = true;
    int currentLine = 1;
    do {
        fgets(buffer, MAX_PER_LINE_SIZE, filePtr);   // Read the next line of the file into the buffer
        if (feof(filePtr))
            keepReading = false;                     // Stop reading if EOF reached
        else if (currentLine == lineNum)
            fputs(replaceText, tempPtr);             // Write the replacement text to this line of the temp file if the line we've reached is the one we wish to replace
        else
            fputs(buffer, tempPtr);                  // Otherwise, write this line to the temp file
        currentLine++;                               // Increment to read the next line
    } while (keepReading);

    fclose(filePtr);
    fclose(tempPtr);

    remove(fileUsed);                                // Delete the original file
    rename("tempFile.txt", fileUsed);                // Rename temp file to the original file's name
}

// Update stock quantity and generate the line to replace original row
void updateStockQuantity(char partID[PART_ID_SIZE], char partName[PART_NAME_SIZE], char quantity[MAX_SIZE_OF_QUANTITY], char supID[SUP_ID_SIZE], char fileUsed[FILENAME_SIZE], int lineNum) {
    int currentQuantity, result, inputNum;
    currentQuantity = atoi(quantity);

    // Select from menu
    while (1) {
        int choice;
        printf("\nPick an operation by entering the corresponding number:");
        printf("\n\t1 - Increase stock quantity\n\t2 - Decrease stock quantity");
        printf("\nEnter negative numbers to quit:");
        scanf("%d", &choice);
        seekToNextLine();

        if (choice > 0 && choice < 3) {
            if (choice == 1) {
                printf("Please insert the stock amount to be added:");
                scanf("%d", &inputNum);
                seekToNextLine();
                result = currentQuantity + inputNum;  // Add inventory
            }
            if (choice == 2) {
                printf("Please insert the stock amount to be subtracted: ");
                scanf("%d", &inputNum);
                seekToNextLine();
                if (inputNum > currentQuantity) {
                    printf("\nCurrent stock is insufficient, unable to grant parts for assembly. Please try again.\n\n");
                    continue;
                }
                result = currentQuantity - inputNum;  // Subtract inventory
            }
            printf("\nDetails updated. Returning to main menu...\n");
            break;
        } else if (choice == 0 || choice > 2) {       // Validating input
            printf("Please choose a number from 1 to 2 only.\n");
        } else {
            exitingProgram(1);                  // If input is a negative number, exit program
        }
    }

    // Store the line to be replaced
    char replaceText[MAX_PER_LINE_SIZE];
    if (result < 10)
        snprintf(replaceText, MAX_PER_LINE_SIZE,"%s|%s|%d|%s|LOW\n",partID, partName, result, supID);
    else
        snprintf(replaceText, MAX_PER_LINE_SIZE,"%s|%s|%d|%s|NORMAL\n",partID, partName, result, supID);

    replaceLine(fileUsed, lineNum, replaceText);
}

// Check which line number the entry is in and print/write the details using tokens
void lineNumberFinderAndPrintResults(char fileUsed[FILENAME_SIZE], char ID[PART_ID_SIZE], int choice, char extraPartForSection1[PART_ID_SIZE]) {
    FILE *filePtr;
    int lineNum = 0;
    int findPart = 0;
    char lineInFile[MAX_PER_LINE_SIZE];

    filePtr = fopen(fileUsed, "r");

    while (fgets(lineInFile, MAX_PER_LINE_SIZE, filePtr) != NULL) {     // Check by line
        if ((strstr(lineInFile, ID)) != NULL) {                         // Increases findPart when the ID exists
            findPart++;
        }
        lineNum++;
        if (findPart == 1) {                                            // Part exists
            break;
        }
    }
    fclose(filePtr);

    // To convert string in file to token
    int x = 0;
    char *array[5];
    lineInFile[strcspn(lineInFile, "\n")] = 0; // Set the length of the number of characters per line before \n in []
                                               // That index will be set to 0 -> Marks the end of a line

    // Extract the first token
    char *token = strtok(lineInFile, "|");     // strtok() -> Breaks string into a series of tokens using a | delimiter
    token[strcspn(token, "\n")] = 0;           // strcspn() -> Calculates the length of the number of characters before the
                                               //              1st occurrence of character present in the string
                                               // That index will be set to 0 -> Marks the end of a token

    // Loop through the string to extract all other tokens
    while (token != NULL) {
        array[x] = token;
        x++;
        token = strtok(NULL, "|");             // Stop condition to break the loop
    }
    printf("\n");
    printingSeparator(69, '-');
    printf("\n");
    if ((choice == 1) || (choice == 3)) {
        // Display Part Details for Section 2 and 4
        messageCentered("Part Details");
        printf("\n");
        printingSeparator(69, '-');
        printf("\n\tPart ID           = %s", array[0]);
        printf("\n\tPart Name         = %s", array[1]);
        printf("\n\tPart Quantity     = %s", array[2]);
        printf("\n\tPart Status       = %s", array[4]);
        printf("\n\tSupplier ID       = %s\n", array[3]);
        printingSeparator(69, '-');
        if (choice == 3) {
            updateStockQuantity(array[0], array[1], array[2], array[3], fileUsed, lineNum);
        }
    }
    if (choice == 2) {
        // Display Supplier Details for Section 4
        messageCentered("Supplier Details");
        printf("\n");
        printingSeparator(69, '-');
        printf("\n\tPart ID           = %s", array[2]);
        printf("\n\tSupplier ID       = %s", array[0]);
        printf("\n\tSupplier Name     = %s\n", array[1]);
        printingSeparator(69, '-');
    }
    if (choice == 4) {
        // Write Supplier Details to supplier.txt for Section 1
        filePtr = fopen(fileUsed, "a");
        fprintf(filePtr, "%s|%s|%s\n", array[0], array[1], extraPartForSection1);
        fclose(filePtr);
    }
}

// Assigns the respective warehouse .txt file according to model chosen
char *selectingWarehouse() {
    int choice;
    printf("Pick a model by entering the corresponding number:");
    printf("\n\t1 - Blaze (BZ)\n\t2 - Silk (SL)\n\t3 - Armer (AR)");
    while (1) {
        printf("\nEnter negative numbers to quit:");
        scanf("%d", &choice);
        seekToNextLine();
        if (choice > 0 && choice < 4) {
            if (choice == 1)
                return "WBZ.txt";    // Blaze warehouse
            if (choice == 2)
                return "WSL.txt";    // Silk warehouse
            return "WAR.txt";        // Armer warehouse
        } else if (choice == 0 || choice > 3) {
            printf("Please choose a number from 1 to 3 only.\n");  // Validating input
            continue;
        } else {
            exitingProgram(1);                               // If input is a negative number, exit program
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print Section 1 results
void printResults1(char *partID, char* model, char *partName, int partQuantity, char* supID) {
    printf("\n");
    printingSeparator(69, '-');
    printf("\n");
    messageCentered("Inventory Creation Details");
    printf("\n");
    printingSeparator(69, '-');
    printf("\nYou have successfully registered %s.", partID);
    printf("\n\tModel Chosen      = %s", model);
    printf("\n\tPart ID           = %s", partID);
    printf("\n\tPart Name         = %s", partName);
    printf("\n\tPart Quantity     = %d", partQuantity);
    printf("\n\tSupplier ID       = %s\n", supID);
    printingSeparator(69, '-');
    printf("\nDetails are successfully printed.\n");
}

// Checks if Supplier ID exist, if not, exits program
void entryCheckForExistenceSupID(char *entrySupID) {
    int flag = 0;
    FILE *fptr;
    fptr = fopen("supplier.txt", "r");
    while (1) {
        printf("Please insert your Supplier ID");
        printf("\nEnter 'X' to quit:");
        scanf("%[^\n]s", entrySupID);                   // Getting input until a newline is scanned
        strupr(entrySupID);
        seekToNextLine();

        if (strcmpi(entrySupID, "X") == 0) {            // If input is "X", exit program
            exitingProgram(1);
        } else if (strlen(entrySupID) > SUP_ID_SIZE) {  // Validating input length
            printf("\nPlease input a Supplier ID within %d characters.\n", SUP_ID_SIZE);
            continue;
        } else if (lineCount("supplier.txt") > 0) {
            /*
             Check file content if user input item already exists, if no (flag = 0), exits program
             flag = 1 (Exist in file)
             flag = 0 (Doesn't exist in file)
            */
            flag = entryExists(entrySupID, fptr);
            fclose(fptr);
            if (flag == 1)
                break;
            else
                exitingProgram(2); // Exit the program -> An entry doesn't exist
        } else
            exitingProgram(6);     // Exit the program -> Records doesn't exist
    }
}

// To count how many distinct supID lines are in the file
int distinctSupIDLineCount() {
    FILE *filePtr;
    filePtr = fopen("supplier.txt", "r");
    char buffer[MAX_PER_LINE_SIZE];                                 // Buffer will store each line from the original file
    int supFinder = 0;
    char supID[SUP_ID_SIZE];                                        // To hold the value of generated supID

    for(int i = 1; i <= lineCount("supplier.txt"); ++i) {
        while (fgets(buffer, MAX_PER_LINE_SIZE, filePtr) != NULL) { // Check by line (Doesn't repeat the line - Since supID entries are in ascending/incremental order)
            // Constructing a Supplier ID
            supID[0] = '\0';
            strcat(supID, "SUP");
            char placeholderNum[MAX_SIZE_OF_SUPPLIER_NO] = {0};
            sprintf(placeholderNum, "%d", i);
            strcat(supID, placeholderNum);

            if ((strstr(buffer, supID)) != NULL) {                  // Increases supFinder when the entry exists
                supFinder++;                                        // How many times supFinder occurred => Will be the distinct value
                break;
            }
        }
    }
    fclose(filePtr);
    return supFinder;
}

// Section 1 - Parts Inventory Creation
void partsInventoryCreation() {
    // Assign an object to the structure
    warehouse part;

    // Setting the file name as the respective warehouse.txt file
    char *fileName = selectingWarehouse();

    // Picking a model and setting the warehouse name
    char model[MODEL_SIZE];
    if (strcmpi(fileName,"WBZ.txt") == 0) {
        strcpy(model, "Blaze");
        strcpy(part.warehouseChoice, "WBZ");
    }
    if (strcmpi(fileName,"WSL.txt") == 0) {
        strcpy(model, "Silk");
        strcpy(part.warehouseChoice, "WSL");
    }
    if (strcmpi(fileName,"WAR.txt") == 0) {
        strcpy(model, "Armer");
        strcpy(part.warehouseChoice, "WAR");
    }

    printingSeparator(69, '-');
    printf("\n");

    // To check if the inventory/part name already exists in the file chosen, if yes, user exits the program
    part.partName[0] = '\0';
    entryCheckForDuplicate(fileName, "part", PART_NAME_SIZE, part.partName);
    strcpy(part.partName, strupr(part.partName)); // Changing the part into UPPER CASE before placing it into the file

    printingSeparator(69, '-');
    printf("\n");

    // Letting user insert the quantity for the new inventory
    part.partQuantity = '\0';
    printf("Please insert the quantity of part (%s) that you would like to register", part.partName);
    printf("\nEnter negative numbers to quit:");
    scanf("%d", &part.partQuantity);
    if (part.partQuantity < 0) {
        exitingProgram(1);
    }

    printingSeparator(69, '-');
    printf("\n");

    // Count the number of lines in all 3 files to give a number for part ID
    int counterForPart = 0;  // Total line count
    counterForPart = lineCount("WBZ.txt") + lineCount("WSL.txt") + lineCount("WAR.txt");

    // Create a unique Part ID
    part.partID[0] = '\0';                                  // Clearing content - Terminate the string regardless of the length of the array or memory
    char placeholderNum[MAX_SIZE_OF_PART_NO] = {0};
    strcat(part.partID, part.warehouseChoice);
    sprintf(placeholderNum, "%d", counterForPart + 1);      // Adding 1 to the ID so it starts with 1 instead of 0
    strcat(part.partID, placeholderNum);
    // NOTE: strcat() behavior is undefined if the destination array is not large enough for the contents of both src
    //       and dest and the terminating null character
    // Hence we need to leave extra (one) space for its null character

    // Assign an object to the structure
    supplier sup;

    // Getting the ID and name of the supplier
    char option;
    printf("Do you have an existing Supplier ID? [ Y (Yes) / N (No) / X (To quit) ]:");
    scanf(" %c", &option);
    strupr(&option);
    seekToNextLine();
    if (option == 'X') {
        exitingProgram(1);
    }
    else if (option == 'Y') {
        if (lineCount("supplier.txt") == 0) {
            exitingProgram(6); // Exit program -> No entries exist
        } else {
            entryCheckForExistenceSupID(sup.supID);
            // Determining the status of stock for the respective part
            FILE *fSection1PartExtra;
            fSection1PartExtra = fopen(fileName, "a");
            if (part.partQuantity < 10)
                fprintf(fSection1PartExtra, "%s|%s|%d|%s|LOW\n", part.partID, part.partName, part.partQuantity, sup.supID);
            else
                fprintf(fSection1PartExtra, "%s|%s|%d|%s|NORMAL\n", part.partID, part.partName, part.partQuantity, sup.supID);
            fclose(fSection1PartExtra);

            // Find the supName in supplier.txt using supID and put supplier information in "supplier.txt"
            lineNumberFinderAndPrintResults("supplier.txt", sup.supID, 4, part.partID);

            // Print Section 1 Results
            printResults1(part.partID, model, part.partName, part.partQuantity, sup.supID);
            exitingProgram(1);
        }
    } else if (option == 'N') {
        // To check if the name already exists in "supplier.txt", if yes, exit program
        entryCheckForDuplicate("supplier.txt", "name", SUP_NAME_SIZE, sup.supName);
        strupr(sup.supName);

        // Count the number of lines in supplier.txt to give a number for part ID
        int counterForSupplier;                                    // Total line count
        counterForSupplier = distinctSupIDLineCount();

        // Create a unique Supplier ID
        sup.supID[0] = '\0';                                       // Clearing content - Terminate the string regardless of the length of the array or memory
        char placeholderNum2[MAX_SIZE_OF_SUPPLIER_NO] = {0};       // To store number so it could be considered as strings to be used
        strcat(sup.supID, "SUP");
        sprintf(placeholderNum2, "%d", counterForSupplier + 1);    // Adding 1 to the ID so it starts with 1 instead of 0
        strcat(sup.supID, placeholderNum2);
    } else {
        exitingProgram(3); // Exit program -> Invalid input
    }

    /*
    // Getting the email of the supplier
    // Issue: Influences other variables
    sup.supEmail[0] = '\0';
    printf("Please enter your email within %d characters", supEmailSize);
    printf("\nEnter 'X' to quit:");
    scanf("%s", sup.supEmail);
    strupr(sup.supEmail);
    seekToNextLine();
    if (strcmpi(sup.supEmail, "X") == 0) {
        exitingProgram(1);
    }
    // Validating input length
    // Issue: Always take the first input as incorrect, second as correct
        inputValidationEmail:
            printf("Please enter your email within %d characters", supEmailSize);
            printf("\nPlease enter your email");
            printf("\nEnter 'X' to quit:");
            scanf(" %s", sup.supEmail);
            strupr(sup.supEmail);
            seekToNextLine();
        if (strlen(sup.supEmail) < supEmailSize) {
        } else
            goto inputValidationEmail;

    // Getting the contact of the supplier
    // Issue: Crashes program when numbers are inserted, but exiting works fine
    sup.supContact[0] = '\0';
    printf("Please enter your contact number without '-'");
    printf("\nEnter 'X' to quit:");
    scanf("%s", sup.supContact);
    seekToNextLine();
    if ((strcmpi(sup.supContact, "X") == 0) || (strcmpi(sup.supContact, "x") == 0)) {
        exitingProgram(1);
    }
    // Validating input length
    // Issue: Always take the first input as incorrect, second as correct
    inputValidationContact:
        sup.supContact[0] = '\0';
        printf("Please input a contact number within %d digits.\n", supContactSize - 1); // If it matches the size value, it leads to error
        printf("\nEnter 'X' to quit:");
        scanf("%s", sup.supContact);
        seekToNextLine();
    if (strlen(sup.supContact) > supContactSize)
        goto inputValidationContact;
    */

    // Put parts information in its respective warehouse ".txt"
    FILE *fSection1Part;
    fSection1Part = fopen(fileName, "a");

    // Determining the status of stock for the respective part
    if (part.partQuantity < 10)
        fprintf(fSection1Part, "%s|%s|%d|%s|LOW\n", part.partID, part.partName, part.partQuantity, sup.supID);
    else
        fprintf(fSection1Part, "%s|%s|%d|%s|NORMAL\n", part.partID, part.partName, part.partQuantity, sup.supID);
    fclose(fSection1Part);

    // Put supplier information in "supplier.txt"
    FILE *fSection1Sup;
    fSection1Sup = fopen("supplier.txt", "a");
    fprintf(fSection1Sup, "%s|%s|%s\n", sup.supID, sup.supName, part.partID);
    fclose(fSection1Sup);

    // Print Section 1 Results
    printResults1(part.partID, model, part.partName, part.partQuantity, sup.supID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Section 2 - Parts Inventory Update
void partsInventoryUpdate() {
    // Assign an object to the structure
    warehouse part;

    // Check if Part ID exists and return its respective filename
    char *fileName = entryCheckForExistencePartID(part.partID);

    // Find the line in warehouse.txt using partID, update stock, and print results
    lineNumberFinderAndPrintResults(fileName, part.partID, 3, "Empty");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Section 3 - Parts Inventory Tracking
void partsInventoryTracking() {
    // Setting the file name as the respective warehouse.txt file
    char *fileName = selectingWarehouse();

    FILE *filePtr;
    char storeLine[MAX_PER_LINE_SIZE];
    filePtr = fopen(fileName, "r");
    int LOWStatusCounter = 0;

    // Select from menu
    int choice;
    printingSeparator(69, '-');
    printf("\nPick an operation by entering the corresponding number:");
    printf("\n\t1 - Print all parts in ascending order (According to Part ID)\n\t2 - Print parts that has < 10 stock");
    while (1) {
        printf("\nEnter negative numbers to quit:");
        scanf("%d", &choice);
        seekToNextLine();
        if (lineCount(fileName) == 0) {
            printf("This file has no records yet.\n");
            break;
        }
        if (choice > 0 && choice < 3) {
            printf("\n");
            printingSeparator(69, '-');
            printf("\n");
            messageCentered("Parts Inventory Details");
            printf("\n");
            printingSeparator(69, '-');
            printf("\n");
            if (choice == 1) {
                while (fgets(storeLine, MAX_PER_LINE_SIZE, filePtr) != NULL) {   // Check by line
                    printf("%s\n", storeLine);
                }
            } else {    // if (choice == 2)
                while (fgets(storeLine, MAX_PER_LINE_SIZE, filePtr) != NULL) {
                    if ((strstr(storeLine, "LOW")) != NULL) {
                        LOWStatusCounter++;
                        printf("%s\n", storeLine);
                    }
                }
                if (LOWStatusCounter == 0) {
                    printf("\nNo parts are less than 10 in %s.\n", fileName);
                }
            }
            fclose(filePtr);
            break;
        } else if (choice == 0 || choice > 2) {
            // Validating input
            printf("Please choose a number from 1 to 2 only.\n");
        } else {
            // If input is a negative number, exit program
            exitingProgram(1);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Section 4 - Search Records & Supplier Details
void searchRecordsAndSupplierDetails() {
    // Assign an object to the structure
    warehouse part;

    // Check if Part ID exists and return its respective filename
    char *fileName = entryCheckForExistencePartID(part.partID);

    // Select from menu
    int choice;
    printingSeparator(69, '-');
    printf("\nPick an operation by entering the corresponding number:");
    printf("\n\t1 - View Part Details\n\t2 - View Supplier Details");
    while (1) {
        printf("\nEnter negative numbers to quit:");
        scanf("%d", &choice);
        seekToNextLine();
        if (choice > 0 && choice < 3) {
            if (choice == 1) {
                // Find the line in warehouse.txt using partID and print results
                lineNumberFinderAndPrintResults(fileName, part.partID, 1, "Empty");
            } else {     // if (choice == 2)
                // Find the line in supplier.txt using partID and print results
                lineNumberFinderAndPrintResults("supplier.txt", part.partID, 2, "Empty");
            }
            break;
        } else if (choice == 0 || choice > 2) {
            // Validating input
            printf("Please choose a number from 1 to 2 only.\n");
        } else {
            // If input is a negative number, exit program
            exitingProgram(1);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generate the necessary files
void fileCheck() {
    FILE *fptr;
    char files[4][FILENAME_MAX] = {"WBZ.txt", "WSL.txt", "WAR.txt", "supplier.txt"}; // Loop for each file
    for(int i = 0; i < 4; i++) {
        // Returns 0 if the files exists
        if(access(files[i], F_OK) != 0) {
            fptr = fopen(files[i], "w");
            fclose(fptr);
        }
    }
}

// Print menu
void printMenu() {
    printf("\n");
    printingSeparator(69, '=');
    printf("\n");
    messageCentered("Welcome to the Automobile Parts Management System");
    printf("\n");
    printingSeparator(69, '=');
    printf("\n\t1. Parts Inventory Creation In Warehouse\n\t2. Parts Inventory Update\n\t3. Parts Inventory Tracking");
    printf("\n\t4. Search Records & Supplier Details\n\t5. Exit");
}

// Selecting operation from menu
void menuSelection() {
    int choice;
    while (1) {
        // Check if the file needed exists, if not then create new ones
        fileCheck();

        // Print menu
        printMenu();
        printf("\nPlease choose any operation from the given options:");
        scanf("%d", &choice);
        seekToNextLine();

        // Input validation
        while ((choice <= 0) || (choice > 5)) {
            printf("Please choose a number from 1 to 5 only.");
            printf("\n\nPlease choose any operation from the given options:");
            scanf("%d", &choice);
            seekToNextLine();
        }

        switch (choice) {
            case 1:
                // Parts Inventory Creation In Warehouse
                printingSeparator(69, '=');
                partsInventoryCreation();
                break;
            case 2:
                // Parts Inventory Update
                printingSeparator(69, '=');
                if (lineCount("supplier.txt") > 0)
                    partsInventoryUpdate();
                else
                    printf("No record exists.\nPlease register in 'Section 1 - Parts Inventory Creation In Warehouse' before proceeding to this section.\n");
                break;
            case 3:
                // Parts Inventory Tracking
                printingSeparator(69, '=');
                if (lineCount("supplier.txt") > 0)
                    partsInventoryTracking();
                else
                    printf("No record exists.\nPlease register in 'Section 1 - Parts Inventory Creation In Warehouse' before proceeding to this section.\n");
                break;
            case 4:
                // Search Records & Supplier Details
                printingSeparator(69, '=');
                if (lineCount("supplier.txt") > 0)
                    searchRecordsAndSupplierDetails();
                else
                    printf("No record exists.\nPlease register in 'Section 1 - Parts Inventory Creation In Warehouse' before proceeding to this section.\n");
                break;
            default:
                // End the program
                printf("Thank you for using this system :D\n");
                printingSeparator(69, '=');
                exit(0);
        }
    }
}

int main() {
    menuSelection();
    return 0;
}