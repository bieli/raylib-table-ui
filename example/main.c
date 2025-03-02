#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "raylib_table_ui.h"


void OnRowClickCallback(int tableId, int row, const char *columnName) {
    printf("Table ID: %d, Row: %d, Column: %s\n", tableId, row, columnName);
}

int main(void) {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const int darkMode = false;
    //const int darkMode = true;

    InitWindow(screenWidth, screenHeight, "Table Visual Component for RAYLIB");

    TableColors tblColors = GetTableDefaultColorsScheme(darkMode);

    int numRows1 = 1;
    int numCols1 = 5;
  
    ColumnDefinition **columnDefinitions1;
    TableRow *rows1;

    columnDefinitions1 = CreateTableWithHeader(numCols1);
    rows1 = CreateTableRows(columnDefinitions1, numCols1, numRows1);
    Table table1 = {1, 50, 50, 500, 600, numRows1, numCols1, 0.0f, rows1, 15, 0, columnDefinitions1, 0, -1, 0, OnRowClickCallback};

    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, 0, "Ic11", CELL_TYPE_IMAGE, 0.15f);
    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, 1, "Text", CELL_TYPE_TEXT, 0.20f);
    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, 2, "Number", CELL_TYPE_NUMBER, 0.20f);
    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, 3, "Value", CELL_TYPE_DOUBLE, 0.25f);
    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, 4, "Actions", CELL_TYPE_ACTIONS, 0.20f);

printf("\n\n columnDefinitions1 END. \n\n");

    // Generate rows of data for Table 1
    GenerateTableRows(
      rows1,
      numRows1,
      numCols1,
      columnDefinitions1
    );

printf("\n\n TableRow *rows1 = CreateTableRows END. \n\n");
/*
    TableRow *rows2 = CreateTableRows(
      numRows2,
      numCols2,
      columnDefinitions2
    );
    
printf("\n\n TableRow *rows2 = CreateTableRows END. \n\n");
*/
    // Define table 1 with sample data

    // Percentages column's definitions
    //float colWidths1[] = {0.15f, 0.20f, 0.20f, 0.25f, 0.20f};
    //Table table1 = {1, 50, 50, 500, 600, numRows1, 30, rows1, 0, 15, columnDefinitions1, false, 0, -1, OnRowClickCallback};

    // Define table 2 with sample data

    // Percentages column's definitions
//    float colWidths2[] = {0.45f, 0.2f, 0.2f, 0.15f};
//    Table table2 = {2, 600, 50, 500, 600, numRows2, 30, colWidths2, rows2, 0, 15, columnDefinitions2, false, 0, -1, OnRowClickCallback};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Check which table the mouse is over and update that table's scroll
        float delta = GetMouseWheelMove() * 30;
        if (IsMouseOverTable(&table1)) {
            UpdateTableScroll(&table1, delta);
        } /* else if (IsMouseOverTable(&table2)) {
            UpdateTableScroll(&table2, delta);
        }*/

        // Handle scrollbar dragging
        HandleScrollbar(&table1);
        //HandleScrollbar(&table2);

        // Highlight row under the mouse
        HighlightRow(&table1);
        //HighlightRow(&table2);

        // Handle row click
        HandleRowClick(&table1);
        //HandleRowClick(&table2);

        BeginDrawing();

        if (darkMode == true) { 
            ClearBackground(BLACK);
        } else {
            ClearBackground(RAYWHITE);
        }

        // Draw the tables
        DrawTable(&table1, tblColors);
        //DrawTable(&table2, tblColors);

        EndDrawing();
    }

    //DisposeTableRows(numRows2, rows2);
    //DisposeTableColumnDefinitions(numCols2, columnDefinitions2);

    DisposeTableRows(numRows1, rows1);
    DisposeTableColumnDefinitions(numCols1, columnDefinitions1);

    CloseWindow();

    return 0;
}
