#include <stdio.h>
#include <stdlib.h>

#define MAX_PARAM_CNT 100
#define RESULT_FILE "Result.csv"
#define NUM_CNT 10
#define ASCII_CHANGE_ROW 13
#define ASCII_0 48
#define ASCII_9 57


#define PRINT_DEBUGMSG
#ifdef PRINT_DEBUGMSG
#define PRINT_DEBUG printf
#else
#define PRINT_DEBUG
#endif 

/////// Global Var. //////////
int parseStart = 0;
//int min = 255;
//int max = -255;
char fileName[1000][100];
char resultBuf[1000][100];

/*
static int GetDirFileName()
{
    FILE *ftr=popen("dir /b","r");
    char temp;
    int fileCnt=0,j=0;

    while(!feof(ftr))
    {
        temp=fgetc(ftr);
        if(temp=='\n')
        {
            fileName[fileCnt++][j]='\0';
            j=0;
        }
        else
        {
            fileName[fileCnt][j++]=temp;
        }
    }
    fclose(ftr);
    return fileCnt;
}

static int ParseRow(char* subStr, char* delim, char* dstBuf[])
{
    int txtCnt = 0;

    while(subStr != NULL)
    {
        dstBuf[txtCnt] = subStr;

        if(txtCnt < MAX_PARAM_CNT)
        {
            txtCnt++;
            subStr = strtok(NULL, delim);
        }
        else
        {
            break;
        }
    }
    return txtCnt;
}
*/

int main(int argc, char *argv[])
{
    FILE *fptr = NULL;
    char tmpbuf[256];
    int i, j, fileCnt = 0;
    //int rowCnt = 1;
    int nameCnt = 0;

    // Creat a new result file.
    if((fptr = fopen(RESULT_FILE, "w")) == NULL)
    {
        printf("%s can't be opened\n", RESULT_FILE);
        system("PAUSE");
        return -1;
    }
    fclose(fptr);
    fptr = NULL;


    if((fptr = fopen("tmp.txt", "r+b")) == NULL)
    {
        printf("tmp.txt open failed\n");
        system("PAUSE");
        return -1;
    }

    // Set buf to zero.
    memset(tmpbuf, 0, sizeof(tmpbuf));

    // Start to parse csv file line by line.
    while(fgets(tmpbuf, sizeof(tmpbuf), fptr) != 0)
    {
        char* subStr;
        char* delim = "\n,";
        //int fldCnt = 0;
        int curChar = 0;
        char findNum = 0;
        //char findSpace = 0;
        char numCnt = 0;
        char param[MAX_PARAM_CNT];

        // Set buf to zero.
        memset(param, 0, sizeof(param));

        subStr = strtok(tmpbuf, delim);

        //fldCnt = ParseRow(subStr, delim, &param[0]);

        if(subStr)
        {
            strncpy(param, subStr, sizeof(param));
            if(param[0] == NULL)
            {
                printf("NULL found! \n"); //continue;
            }
            else
            {
                PRINT_DEBUG("param:[%s]. \n", subStr);
                for(j=0;j < MAX_PARAM_CNT; j++)
                {
                    findNum = 0;

                    if(param[j] == ASCII_CHANGE_ROW) break;

                    if((param[j] < ASCII_9) && (param[j] > ASCII_0))//for(i = 0; i < NUM_CNT; i++)
                    {// Find number.
                        findNum = 1;
                        numCnt++;
                        //PRINT_DEBUG("find num param[%d]:[%c] \n", j, param[j]);
                    }

                    if(findNum)
                    {// 目前字元為數字
                        if(curChar < 3)
                        {// 不接受數字當名稱開頭的人
                            break;
                        }
                        else
                        {
                            if(numCnt == 1)
                            {// insert "," before the 1st. number.
                                resultBuf[nameCnt][curChar] = ',';
                                curChar++;
                            }
                            resultBuf[nameCnt][curChar] = param[j];
                            curChar++;
                        }
                    }
                    else
                    {// 目前字元非數字
                        if(numCnt == 0)
                        {// 尚未找到過數字
                            if((param[j] == '+') || (param[j] == '*'))
                            {// 以"，"取代 + *
                                resultBuf[nameCnt][curChar] = ',';
                                curChar++;
                            }
                            else if(param[j] == ' ')
                            {// 先找出空白字元
                                printf("findSpace param[%d] : %d \n", j, param[j+1]); // Y test
                                if( (param[j+1]< 65) || (param[j+1] > 122))
                                {// 空白字元後非英文則以"，"取代之；切開名稱與 "各一" & "加一" 之類
                                    resultBuf[nameCnt][curChar] = ',';
                                    curChar++;
                                }
                                else
                                {// 空白字元後亦為空白；默認為英文姓名，需保留空白
                                    resultBuf[nameCnt][curChar] = param[j];
                                    curChar++;
                                }
                            }
                            else
                            {
                                resultBuf[nameCnt][curChar] = param[j];
                                curChar++;
                            }
                        }
                        else if((param[j] == '+') || (param[j] == '*'))
                        {// 先前已找到過數字
                            resultBuf[nameCnt][curChar] = ',';
                            curChar++;
                        }
                        else
                        {// nop.
                        }
                    }
                }
            }

            if(curChar > 0) nameCnt++;

            PRINT_DEBUG("== curChar:%d, nameCnt:%d numCnt:%d ==\n", curChar, nameCnt, numCnt);
        }// if(subStr)
    }// while

    fclose(fptr);
    fptr = NULL;



    // Save result in file.
    if((fptr = fopen(RESULT_FILE, "a+")) == NULL)
    {
        PRINT_DEBUG("%s can't be opened\n", RESULT_FILE);
        system("PAUSE");
        return -1;
    }
    for(j = 0; j < nameCnt; j++)
    {
        printf("%s\n", resultBuf[j]);
        fprintf(fptr, "%s\n", resultBuf[j]);
    }
    fclose(fptr);
    fptr = NULL;


    system("PAUSE");
    return 0;
}





/* Sample code.
static size_t GetBinFile(FILE* pFile, char* buff)
{
    long lSize;
    size_t result;

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buff = (char*) malloc (sizeof(char)*lSize);
    if (buff == NULL)
    {
        printf("Memory error\n");
        fputs ("Memory error",stderr);
        exit (2);
    }

    // copy the file into the buffer:
    result = fread (buff,1,lSize,pFile);
    if (result != lSize)
    {
        printf("Reading error\n");
        fputs ("Reading error",stderr);
        exit (3);
    }
//printf("size:0x%x\n", sizeof(*buff));
    // the whole file is now loaded in the memory buffer.

    return result;
}

static int GetDirFileName()
{
    FILE *ftr=popen("dir /b","r");
    char temp;
    int fileCnt=0,j=0;

    while(!feof(ftr))
    {
        temp=fgetc(ftr);
        if(temp=='\n')
        {
            fileName[fileCnt++][j]='\0';
            j=0;
        }
        else
        {
            fileName[fileCnt][j++]=temp;
        }
    }

    for(j = 0; j < fileCnt; j++)
    {
        printf("%s\n", fileName[j]);
    }

    fclose(ftr);
    system("PAUSE");
    return fileCnt;
}
*/

