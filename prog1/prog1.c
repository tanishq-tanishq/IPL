#include <stdio.h>
#include <stdlib.h>

struct Student
{
    int id;
    char name[30];
    float marks;
};

// Function to display mth record
void displayRecord(FILE *fp, int m)
{
    struct Student s;

    // Move file pointer to mth record
    fseek(fp, (m - 1) * sizeof(struct Student), SEEK_SET);

    // Read record
    if (fread(&s, sizeof(struct Student), 1, fp))
    {
        printf("\n--- Record %d ---\n", m);
        printf("ID    : %d\n", s.id);
        printf("Name  : %s\n", s.name);
        printf("Marks : %.2f\n", s.marks);
    }
    else
    {
        printf("\nRecord not found.\n");
    }
}

// Function to delete a record
void deleteRecord(const char *filename, int deletePos, int total)
{
    FILE *fp, *temp;
    struct Student s;
    int i;

    fp = fopen(filename, "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL)
    {
        printf("File error.\n");
        return;
    }

    for (i = 1; i <= total; i++)
    {
        fread(&s, sizeof(struct Student), 1, fp);

        // Skip record to be deleted
        if (i != deletePos)
        {
            fwrite(&s, sizeof(struct Student), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    // Replace old file with new file
    remove(filename);
    rename("temp.dat", filename);

    printf("\nRecord %d deleted successfully.\n", deletePos);
}

int main()
{
    FILE *fp;
    struct Student s;
    int n, i, m, del;

    fp = fopen("student.dat", "wb");

    if (fp == NULL)
    {
        printf("Cannot open file.\n");
        return 1;
    }

    printf("Enter number of records: ");
    scanf("%d", &n);

    // Store records
    for (i = 0; i < n; i++)
    {
        printf("\nEnter details for student %d\n", i + 1);

        printf("Enter ID: ");
        scanf("%d", &s.id);

        printf("Enter Name: ");
        scanf("%s", s.name);

        printf("Enter Marks: ");
        scanf("%f", &s.marks);

        fwrite(&s, sizeof(struct Student), 1, fp);
    }

    fclose(fp);

    // Open file for reading
    fp = fopen("student.dat", "rb");

    printf("\nEnter record number to display: ");
    scanf("%d", &m);

    displayRecord(fp, m);

    fclose(fp);

    // Delete record
    printf("\nEnter record number to delete: ");
    scanf("%d", &del);

    deleteRecord("student.dat", del, n);

    // Display remaining records
    fp = fopen("student.dat", "rb");

    printf("\n--- Remaining Records ---\n");

    while (fread(&s, sizeof(struct Student), 1, fp))
    {
        printf("%d %s %.2f\n", s.id, s.name, s.marks);
    }

    fclose(fp);

    return 0;
}

