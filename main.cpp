#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "PlainTextReader.h"
#include "DxfReader.h"
#include "ScrWriter.h"
#include "SimpleLayouter.h"
#include "Utils.h"

int main(int argc, char **argv)
{
    std::vector<GraphInfo> graphSet;
    std::string filename;
    IInputReader *inputReader;

    if(argc > 1)
    {
        filename = argv[1];
        if(filename.rfind(".txt") == filename.length()-4)
        {
            inputReader = new PlainTextReader(filename.c_str(), graphSet);
        }
        else
        {
            inputReader = new DxfReader(filename.c_str(), graphSet);
        }
    }
    else
    {
        inputReader = new DxfReader("/Users/yiyuan/Desktop/nesting/input.dxf", graphSet);
    }

    if(inputReader->ReadData())
    {
        graphSet[0].num = 10;
        printf("Read graph data successfully.\n");
        printf("The number of different graphs: %d\n", (int)graphSet.size());
        for(int i = 0; i < (int)graphSet.size(); i++)
        {
            printf("==========\n");
            printf("Graph type %d:\n", i+1);
            printf("The number of graphs for this type: %d\n", graphSet[i].num);
            if(graphSet[i].graph->GetType() == Graph::GENERAL)
                ((GeneralGraph *)graphSet[i].graph)->Print();
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

    IOutputWriter *writer = new ScrWriter("/Users/yiyuan/Desktop/nesting/output", result);
    writer->SetPageSize(boardSize);
    writer->OutputFile();

    return 0;
}
