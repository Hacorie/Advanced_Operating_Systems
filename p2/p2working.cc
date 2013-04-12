#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

#define AOS_SUCCESS         1


#define AOS_PRINT_FAIL      -1
#define AOS_OPEN_FAIL       -2
#define AOS_SEEK_FAIL       -3

//GLOBAL VARIABLES
map<string, string> variables;
map<string, int>    files;
map<string, string>    file_content;
map<string, vector<int> > file2blocks;
int blocks[500];
struct blocks
{
    string filename;
    string contents;
};
struct openedFile
{
    string filename;
    int size;
    int currPos;
} opened;

struct blocks allBlocks[500];

//FUNCTION PROTOTYPES
int aos_set(string buff_name, string str);      //done
int aos_print(string buff_name);                //done
int aos_create(string file_name);               //done
int aos_open(string file_name);                 //done
int aos_write(string buff_name);                //done
int aos_read(int num_bytes, string buffer_name);//done
int aos_close();                                //done
int aos_delete(string file_name);               //done
int aos_stat(string buff_name);                 //done
int aos_seek(int disp);                         //done
int aos_seek(string buff_name);                 //done
int aos_showinfo();                             //done
bool checkDigit(string str);                    //done
void openedReset();                             //done
int findNextBlock();                            //done
void updateContainers(string, int, int);

//Main function
int main(int argc, char** argv)
{
    //check for correct args
    if(argc < 2 || argc > 2)
    {
        //cout << "usage: ./p2 test_filename" << endl;
        return -99;
    }

    //open the file for reading
    string filename = argv[1];
    ifstream ifs(filename.c_str());

    //make sure the file exists
    if(!ifs)
    {
        //cout << "The file: '" << filename << "' does not exist!" << endl;
        return -99;
    }

    //LOCAL VARIABLES
    string line;            //holds a line of text
    vector<string> lines;   //vector to hold file
    vector<string> tokens;  //vector to hold split line
    string sub;             //substring of a line
    int rc = 1;
    bool isDig;

    fill_n(blocks, 500, -1);

    //store the program in memory
    while(getline(ifs, line))
    {
        if(line != "" && line[0] != '#')
        {
            lines.push_back(line);
            //cout << line << endl;
        }
    }

    //go through the program one line at a time and do requested
    for(int i = 0; i < lines.size(); i++)
    {
        //clear tokens vector to ensure old data is gone
        tokens.clear();

        //string stream to help split a line
        istringstream iss(lines[i]);

        //split the line and store each component in tokens
        while(iss >> sub)
        {
            tokens.push_back(sub);
        }

        //make whatever the command is to lower case since lang is case insensitive.
        transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), ::tolower);
        //cout << tokens[0] << endl;
        //large if/else block that does the appropriate action
        if(tokens[0] == "set")
        {
            rc = aos_set(tokens[1], tokens[2]); 
        }
        else if(tokens[0] == "print")
        {
            rc = aos_print(tokens[1]);
        }
        else if(tokens[0] == "create")
        {
            rc = aos_create(tokens[1]);
        }
        else if(tokens[0] == "open")
        {
            rc = aos_open(tokens[1]);
        }
        else if(tokens[0] == "write")
        {
            rc = aos_write(tokens[1]);
        }
        else if(tokens[0] == "read")
        {
            rc = aos_read(atoi(tokens[1].c_str()), tokens[2]);
        }
        else if(tokens[0] == "close")
        {
            rc = aos_close();
        }
        else if(tokens[0] == "del")
        {
            rc = aos_delete(tokens[1]);
        }
        else if(tokens[0] == "stat")
        {
            rc = aos_stat(tokens[1]);
        }
        else if(tokens[0] == "seek")
        {
            isDig = checkDigit(tokens[1]);
            if(isDig == true)
                rc = aos_seek(atoi(tokens[1].c_str()));
            else
                rc = aos_seek(tokens[1]);
        }
        else if(tokens[0] == "showinfo")
        {
            rc = aos_showinfo();
        }
        else
        {
            cout << tokens[0] << " is not a valid command! Exterminating Program!" << endl;
            return -99;
        }

        if(rc != AOS_SUCCESS)
        {
            //cout << "PROGRAM FAILED SOMEWHERE!" << endl;
            return rc;
        }
     }

    return 0;
}

//DONE
int aos_set(string buff_name, string str)
{
    //add new variable to program memory
    variables[buff_name] = str;
    //cout << "Set Successful" << endl;
    return AOS_SUCCESS;
}

//DONE
int aos_print(string buff_name)
{
    //if th variable exists, output it's contents
    if(variables.find(buff_name) != variables.end())
        cout << buff_name << ": " << variables[buff_name] <<  endl;
    else
        return AOS_PRINT_FAIL;

    //cout << "Print Successful" << endl;
    return AOS_SUCCESS;
}

//DONE
int aos_create(string file_name)
{
    //create a new file and set its start size to 0 bytes
    files[file_name] = 0;

    //cout << "create successful" << endl;
    return AOS_SUCCESS;
}

//DONE
int aos_open(string file_name)
{
    //check and see if there is already an open file
    if(opened.filename != "")
    {
        aos_close();
    }

    //does the file exist?
    if(files.find(file_name) == files.end())
        return AOS_OPEN_FAIL;

    //fill in initial vaulues of opened struct.
    opened.filename = file_name;
    opened.size = files[file_name];
    opened.currPos = 0;

    //cout << "open successful" << endl;
    return AOS_SUCCESS;
}

//TODO
int aos_write(string buff_name)
{
    map<string, vector<int> >::iterator it = file2blocks.find(opened.filename);
    int blockNum;
    buff_name = variables[buff_name];
    int buff_size = buff_name.length();
    string str;
    int freeSize;
    int disp = opened.currPos;
    int freeOfBlock;
    if(buff_size == 0)
        return AOS_SUCCESS;
    if(it != file2blocks.end())
    {
        vector<int> vecIt = it->second;
        int j = disp/100;
        int  pos = abs((100*j)-disp);
        //cout << disp << endl;
        //cout << opened.size << endl << endl;
        if(disp != opened.size)
        {
            blockNum = vecIt[j];
            freeOfBlock = abs(100 - pos);
            if(buff_size < freeOfBlock)
            {
                allBlocks[blockNum].contents.replace(pos, buff_size, buff_name);
                file_content[opened.filename].replace(disp, buff_size, buff_name);
                opened.currPos +=  buff_size;
                opened.size = file_content[opened.filename].length();
                return AOS_SUCCESS;
            }
            else //if(freeOfBlock != 0)
            {
                //freeOfBlock = abs(freeOfBlock);
                //cout << freeOfBlock << endl << endl;
                //cout << buff_size << endl;
                if(freeOfBlock != 0)
                {
                    //fill current buffer the rest of the way
                    str = buff_name.substr(0, freeOfBlock);
                    allBlocks[blockNum].contents.replace(100-freeOfBlock, 100, str);
                    file_content[opened.filename].replace(disp, str.length(), str);
                    files[opened.filename] = file_content[opened.filename].length();
                    buff_name = buff_name.substr(freeOfBlock);
                    buff_size = buff_name.length();

                    //update disp for next block

                    //disp = 0;
                    //freeOfBlock = 100;
                    opened.currPos += str.length();
                    opened.size = file_content[opened.filename].length();
                    if(buff_size == 0)
                        return AOS_SUCCESS;
                 }
                //fill every buffer that currently exists until no more data
                freeOfBlock = 100;
                pos = 0;
                j = (opened.currPos/100)-1;
                for(int i = j+1; i < vecIt.size(); i++)
                {
                    blockNum = vecIt[i];
                    //cout << buff_size << endl << freeOfBlock << endl << endl;
                    if(buff_size < freeOfBlock)
                    {
                        allBlocks[blockNum].contents.replace(pos, buff_size, buff_name);
                        file_content[opened.filename].replace(opened.currPos, buff_size, buff_name);
                        opened.size = file_content[opened.filename].length();
                        opened.currPos += buff_size;
                        return AOS_SUCCESS;
                    }
                    else
                    {
                        str = buff_name.substr(0, freeOfBlock);
                        allBlocks[blockNum].contents.replace(100-freeOfBlock, 100, str);
                        file_content[opened.filename].replace(opened.currPos, str.length(), str);
                        files[opened.filename] = file_content[opened.filename].length();
                        buff_name = buff_name.substr(freeOfBlock);
                        buff_size = buff_name.length();
                        opened.size = file_content[opened.filename].length();
                        opened.currPos += str.length();
                        //cout << buff_size << endl;
                        if(buff_size == 0)
                            return AOS_SUCCESS;
                    }
                }
                for(int i = 0; i < 10; i++)
                {
                    blockNum = findNextBlock();
                    if(buff_size < 100)
                    {
                        allBlocks[blockNum].filename = opened.filename;
                        file2blocks[opened.filename].push_back(blockNum);
                        updateContainers(buff_name, buff_name.length(), blockNum);
                        return AOS_SUCCESS;
                    }
                    else
                    {
                        str = buff_name.substr(0, 100);
                        updateContainers(str, 100, blockNum);
                        file2blocks[opened.filename].push_back(blockNum);
                        buff_name = buff_name.substr(100);
                        buff_size = buff_name.length();
                        if(buff_size == 0)
                            return AOS_SUCCESS;
                    }
                }
            }
        }
        else
        {
            j = (opened.currPos/100);
            if(j == vecIt.size())
                j-=1;
            blockNum = vecIt[j];
            freeSize = 100 - allBlocks[blockNum].contents.length();
            if(freeSize != 0)
            {
                if(buff_size < freeSize)
                {
                    updateContainers(buff_name, buff_size, blockNum);
                    return AOS_SUCCESS;
                }
                else
                {
                    str = buff_name.substr(0, freeSize);
                    updateContainers(str, freeSize, blockNum);
                    buff_name = buff_name.substr(freeSize);
                    buff_size = buff_name.length();
                }
            }
            for(int i = 0; i < 10; i++)
            {
                blockNum = findNextBlock();
                if(buff_size < 100)
                {
                    allBlocks[blockNum].filename = opened.filename;
                    file2blocks[opened.filename].push_back(blockNum);
                    updateContainers(buff_name, buff_name.length(), blockNum);
                    return AOS_SUCCESS;
                }
                else
                {
                    str = buff_name.substr(0, 100);
                    updateContainers(str, 100, blockNum);
                    file2blocks[opened.filename].push_back(blockNum);
                    buff_name = buff_name.substr(100);
                    buff_size = buff_name.length();
                    if(buff_size == 0)
                        return AOS_SUCCESS;
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < 10; i++)
        {
            blockNum = findNextBlock();
            if(buff_size < 100)
            {
                allBlocks[blockNum].filename = opened.filename;
                file2blocks[opened.filename].push_back(blockNum);
                updateContainers(buff_name, buff_name.length(), blockNum);
                //cout << "write successful" << endl;
                //cout << "Block: " << blockNum << endl;
                //cout << "contents: " << allBlocks[blockNum].contents << endl << endl;
                return AOS_SUCCESS;
            }
            else
            {
                str = buff_name.substr(0, 100);
                file2blocks[opened.filename].push_back(blockNum);
                updateContainers(str, 100, blockNum);
                buff_name = buff_name.substr(100);
                buff_size = buff_name.length();
                if(buff_size == 0)
                    return AOS_SUCCESS;
            }
        }
    }
    return AOS_SUCCESS;
}

//Semi-Done--does read update file pointer?
int aos_read(int num_bytes, string buff_name)
{
    string subs;    //create substring holder

    //Is this file too large?..read to EOF if so.
    if( (num_bytes + opened.currPos) >= opened.size)
    {
        subs = file_content[opened.filename].substr(opened.currPos);
        opened.currPos = opened.size; //comment below
    }
    else    //read n_bytes
    {
        subs = file_content[opened.filename].substr(opened.currPos, num_bytes);
        opened.currPos += num_bytes; //does read update file location?
    }
    //if it has a \0, don't add one, else do;
    //if(subs[subs.length()-1] != '\0')
    //    subs += '\0';

    //create or overwrite a variable
    variables[buff_name] = subs;

    //cout << "call read" << endl;
    return AOS_SUCCESS;
}

//DONE
int aos_close()
{
    //reset the opened structure
    openedReset();

    return AOS_SUCCESS;
}

//DONE -- delete fat entries and reclaim blocks
int aos_delete(string file_name)
{
    map<string, vector<int> >::iterator it = file2blocks.find(file_name);
    vector<int> vecIt = it->second;
    int numBlock;

    for(int i = 0; i < vecIt.size(); i++)
    {
        numBlock = vecIt[i];
        blocks[numBlock] = -1;
        allBlocks[numBlock].filename = "";
        allBlocks[numBlock].contents = "";
    }
    file2blocks.erase(file_name);
    files.erase(file_name);
    file_content.erase(file_name);
    //cout << "call delete" << endl;

    return AOS_SUCCESS;
}

//DONE
int aos_stat(string buff_name)
{
    //store file size as a string
    stringstream ss;
    ss << opened.size;
    variables[buff_name] = ss.str();

    //cout << "Call stat "<< ss.str() << endl;
    return AOS_SUCCESS;
}

//DONE
int aos_seek(int disp)
{
    //make sure the displacement is not too large. Fail if it is.
    //cout << opened.size << endl;
    if(disp < opened.size && disp >= 0)
        opened.currPos = disp;
    else
        return AOS_SEEK_FAIL;

    //cout << "call seek" << endl;
    return AOS_SUCCESS;
}

//DONE
int aos_seek(string buff_name)
{
    bool check;
    int disp;
    //cout << opened.size << endl;
    //make sure displacement is a number and not too large. Fail if too large or not a number.
    if( checkDigit(variables[buff_name]))
    {
        disp = atoi(variables[buff_name].c_str());
        if( disp <= opened.size && disp >= 0 )
            opened.currPos = disp;
        else
            return AOS_SEEK_FAIL;
    }
    else
        return AOS_SEEK_FAIL;

    //cout << "call seek" << endl;
    return AOS_SUCCESS;
}

//DONE
int aos_showinfo()
{
    map<string, vector<int> >::iterator it;
    int numBlock;

    for(it = file2blocks.begin(); it != file2blocks.end(); it++)
    {
        vector<int> vecIt = it->second;
        cout << it->first << "\t" << file_content[it->first].length() << "\t";
        for(int i = 0; i < vecIt.size(); i++)
        {
            numBlock = vecIt[i];
            cout << numBlock << " ";
        }
        cout << endl;
        //cout << "\t" << file_content[it->first] << endl;;
    }

    //cout << "call showinfo" << endl;
    return AOS_SUCCESS;
}

//Function to check and see if the str is a digit or not
bool checkDigit(string str)
{
    //returns true if str is all numbers
    bool check = true;
    for(int j = 0; j < str.length(); j++)
    {
        if(isdigit(str[j]))
            check = true;
        else
        {
            check = false;
            break;
        }
    }
    return check;
}

//resets opened struct to default vaules
void openedReset()
{
    opened.filename = "";
    opened.size = 0;
    opened.currPos = 0;
    return;
}

int findNextBlock()
{
    for(int i = 0; i < 500; i++)
        if(blocks[i] == -1)
        {
            blocks[i] = 1;
            return i;
        }
    return -1;
}

void updateContainers(string buff_name, int buff_size, int blockNum)
{

    allBlocks[blockNum].contents += buff_name;
    //cout << "write successful" << endl;
    files[opened.filename] += buff_size;
    file_content[opened.filename] += buff_name;
    opened.size = file_content[opened.filename].length();
    opened.currPos += buff_size;
}
