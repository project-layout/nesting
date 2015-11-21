#include <stdio.h>
#include <stdlib.h>

#include "PlainTextReader.h"
#include "DxfReader.h"
#include "ScrWriter.h"
#include "SimpleLayouter.h"
#include "Utils.h"
#include "unittest/UnitTest.h"

int main()
{
    std::vector<GraphInfo> graphSet;
    //IInputReader *inputReader = new PlainTextReader("test_input.txt", graphSet);
    IInputReader *inputReader = new DxfReader("try2.dxf", graphSet);

    if(inputReader->ReadData())
    {
        graphSet[0].num = 10;
        printf("Read graph data successfully.\n");
        printf("The number of different graphs: %d\n", graphSet.size());
        for(int i = 0; i < (int)graphSet.size(); i++)
        {
            printf("==========\n");
            printf("Graph type %d:\n", i+1);
            printf("The number of graphs for this type: %d\n", graphSet[i].num);
            graphSet[i].graph->Print();
        }
    }
    else
    {
        printf("Failed to read graph data.\n");
        return 0;
    }

    Point boardSize = { (double)3000, (double)200000 };
    std::vector<NestingResult> result;
    ILayouter *layouter = new SimpleLayouter(graphSet, boardSize, result, 0.01);
    layouter->Solve();

    printf("==========\n");
    printf("Nesting result:\n");
    for(int i = 0; i < (int)result.size(); i++)
    {
        printf("Graph %d: (%d, %f, %f, %f)\n", i+1, result[i].board, result[i].pos.x, result[i].pos.y, result[i].angle);
    }

    IOutputWriter *writer = new ScrWriter("output", result);
    writer->OutputFile();

    return 0;
}
