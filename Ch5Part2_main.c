/* 
 * File:   Ch5Part2_main.c
 * Author: obeezy
 *
 */

#include <ctype.h>
#include <fcntl.h>  
#include <sys/stat.h>
#include <errno.h>
#include "apue.h"
/*
 * 
 */
struct DirectoryStruct{
        char Code[4];    //field 1
        off_t Offset;    //field 2
    };
    
struct Record{
    char CountryCode[4];   //field 2
    char CountryName[41];   //field 3
    int Pop;             //field 8
    float LifeExp;      //field 9
    };
    
int BinarySearch(struct DirectoryStruct *myDirStructPtr, char code[], int array_size);
int main(int argc, char** argv) {
  
    int N;
    struct Record myDataStruct;
    struct Record *structPtr;
    struct DirectoryStruct myDirectStruct;
    struct DirectoryStruct *directPtr;
    int numOfRecs = 239;
    int offset;
    
    FILE *countryFile;
    countryFile = fopen("RandomCountryFile.bin", "rb");
    if (!countryFile)err_sys("Random Country File not open for reading");
    
    FILE *directoryFile;
    directoryFile = fopen("CountryDirectory.bin","rb");
    if (!directoryFile)err_sys("Country directory File not open for reading");
    
    structPtr = malloc(sizeof(myDataStruct));                           //allocate memory for one struct
    directPtr = calloc(numOfRecs, sizeof(myDirectStruct));              //allocate memory for whole directory
            
    N = fread(directPtr, sizeof(struct DirectoryStruct), numOfRecs, directoryFile);
    
    printf("Enter country code to search for: ");
    char inputCode[4];
    scanf("%s",inputCode);
    inputCode[3] = (char)0;
    
    int i;
    for(i = 0; i < 3 ; i++)
        inputCode[i] = toupper(inputCode[i]);
    
    offset = BinarySearch(directPtr,inputCode,numOfRecs);
    fseek(countryFile,offset,SEEK_SET);                         //offset is returned and set 
    if(offset < 0)
    {}                                //invalid entries will have no offset hence nothing is done
    else
    {
        fread(structPtr,sizeof(myDataStruct),1,countryFile);
        printf("\nCountry Code is: %s",structPtr->CountryCode);
        printf("\nCountry Name is: %s",structPtr->CountryName);
        char buffer1[12];
        sprintf(buffer1,"%d",structPtr->Pop);         //convert Pop to string in order to write to output 
        printf("\nCountry Population is: %s",buffer1);
        char buffer2[5];
        sprintf(buffer2,"%.1f\n",structPtr->LifeExp); //convert LifeExp to string in order to write to output
        printf("\nCountry Life Expectancy is: %s",buffer2);
    }
    
    free(structPtr);
    free(directPtr);
    fclose(countryFile);
    fclose(directoryFile);
    return (EXIT_SUCCESS);
}
int BinarySearch( struct DirectoryStruct *myDirStructPtr, char code[], int array_size)
{
    char beg[4];
    char mid[4];
    char end[4];
    char target[4];
    int endI = array_size - 1;          //set index for end code in array
    int n = array_size - 1;             //set n counter
    int begI = 0;                       //set index for beginning code in array
    int midI = begI + endI/2;           //set index for middle code in array
        
    strcpy(target,code);                        
    strcpy(beg,myDirStructPtr[begI].Code);
    strcpy(end,myDirStructPtr[endI].Code);
    strcpy(mid, myDirStructPtr[midI].Code);
  
    while(( strncmp(beg, end, 3) <= 0 ) && (strncmp(mid, target, 3) != 0))
    {
        // is the target in lower or upper half?
	if (strncmp(target, mid, 3) < 0)
	{
           endI = midI - 1;                  // new end
           strcpy(end,myDirStructPtr[endI].Code);
           n = n/2;
           midI = begI + n/2;               // new middle
           strcpy(mid, myDirStructPtr[midI].Code);
        }
        else
        {
           begI = midI + 1;                  // new beginning
           strcpy(beg,myDirStructPtr[begI].Code);
           n = n/2;
           midI = begI + n/2;               // new middle  
           strcpy(mid, myDirStructPtr[midI].Code);
        }
    }   
    
    if (strncmp(mid,target,3) == 0)
    {
        printf("%s found!",target);
        return myDirStructPtr[midI].Offset;
    }
    else
    {
        printf("%s not found in directory. Please enter a valid Country");
        return -1;
    }
}  

