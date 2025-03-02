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

    int numRows1 = 3;
    int numCols1 = 5;

    ColumnDefinition **columnDefinitions1 = CreateTableHeader(numCols1);
    TableRow *rows1 = CreateTableRows(columnDefinitions1, numCols1, numRows1);
    //Table table1 = {1, 50, 50, 500, 600, numRows1, numCols1, 0.0f, rows1, 15, 0, columnDefinitions1, 0, -1, 0, OnRowClickCallback};
    Table table1 = CreateTable(1, 50, 50, 500, 600, numRows1, numCols1, 0.0f, rows1, 15, 0, columnDefinitions1, 0, -1, 0, OnRowClickCallback);

    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, "Icon", CELL_TYPE_IMAGE, 0.15f);
    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, "Text", CELL_TYPE_TEXT, 0.20f);
    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, "Number", CELL_TYPE_NUMBER, 0.20f);
    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, "Value", CELL_TYPE_DOUBLE, 0.25f);
    AddColumnToTableHeaderColumn(&table1, columnDefinitions1, "Actions", CELL_TYPE_ACTIONS, 0.20f);

printf("\n\n columnDefinitions1 END. \n\n");

    // Generate rows of data for Table 1
    GenerateTableRows(
      rows1,
      numRows1,
      numCols1,
      columnDefinitions1
    );

printf("\n\n TableRow *rows1 = CreateTableRows END. \n\n");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Check which table the mouse is over and update that table's scroll
        float delta = GetMouseWheelMove() * 30;
        if (IsMouseOverTable(&table1)) {
            UpdateTableScroll(&table1, delta);
        }
        HandleScrollbar(&table1);
        HighlightRow(&table1);
        HandleRowClick(&table1);

        BeginDrawing();

        if (darkMode == true) { 
            ClearBackground(BLACK);
        } else {
            ClearBackground(RAYWHITE);
        }

        DrawTable(&table1, tblColors);

        EndDrawing();
    }

    DisposeTableRows(numRows1, rows1);
    DisposeTableColumnDefinitions(numCols1, columnDefinitions1);

    CloseWindow();

    return 0;
}
