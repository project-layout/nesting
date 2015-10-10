#include <stdio.h>

#include "PlainTextReader.h"
#include "Utils.h"
#include "unittest/UnitTest.h"

int main()
{
    std::vector<GraphInfo> graphSet;
    IInputReader *inputReader = new PlainTextReader("test_input.txt", graphSet);
    if(inputReader->ReadData())
    {
        printf("Read graph data successfully.\n");
        printf("The number of different graphs: %d\n", graphSet.size());
        for(int i = 0; i < (int)graphSet.size(); i++)
        {
            printf("Graph type %d:\n", i+1);
            printf("The number of graphs for this type: %d\n", graphSet[i].num);
            graphSet[i].graph->Print();
        }
    }
    else
        printf("Failed to read graph data.\n");
    return 0;
}
