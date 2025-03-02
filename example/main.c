#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raylib_table_ui.h"


void OnRowClickCallback(int tableId, int row, const char *columnName) {
    printf("Table ID: %d, Row: %d, Column: %s\n", tableId, row, columnName);
}

int main(void) {
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Table Visual Component for RAYLIB");

    // Define column names for Table 1
    const char *columnNames1[] = {"Icon", "Text", "Number", "Value"};

    // Define column names for Table 2
    const char *columnNames2[] = {"Text", "Number", "Value", "Chk"};

    // Generate rows of data for Table 1
    int numRows = 100;

    TableRow *rows1 = CreateTableRows(
      numRows,
      (int) sizeof(columnNames1)/sizeof(columnNames1[0])
    );

    //TableRow *rows2 = CreateTableRows(numRows);

    // Generate rows of data for Table 2
    TableRow *rows2 = malloc(numRows * sizeof(TableRow));
    for (int i = 0; i < numRows; i++) {
        rows2[i].numCells = (int) sizeof(columnNames2)/sizeof(columnNames2[0]);
        rows2[i].cells = malloc(rows2[i].numCells * sizeof(Cell));

        rows2[i].cells[0].type = CELL_TYPE_TEXT;
        rows2[i].cells[0].text = malloc(32 * sizeof(char));
        snprintf((char *)rows2[i].cells[0].text, 32, "More then data: %d", i + 1);

        rows2[i].cells[1].type = CELL_TYPE_NUMBER;
        rows2[i].cells[1].number = (i + 1) * 10;

        rows2[i].cells[2].type = CELL_TYPE_DOUBLE;
        rows2[i].cells[2].value = (i + 1) * 1.5;
        
        rows2[i].cells[3].type = CELL_TYPE_CHECKBOX;
        rows2[i].cells[3].checkbox = i % 2;
    }

    // Define table 1 with sample data
    
    // Percentages column's definitions
    float colWidths1[] = {0.15f, 0.35f, 0.25f, 0.25f};
    Table table1 = {1, 50, 50, 500, 600, numRows, 30, colWidths1, rows1, 0, 15, columnNames1, false, 0, -1, OnRowClickCallback};

    // Define table 2 with sample data
    
    // Percentages column's definitions
    float colWidths2[] = {0.45f, 0.2f, 0.2f, 0.15f};
    Table table2 = {2, 600, 50, 500, 600, numRows, 30, colWidths2, rows2, 0, 15, columnNames2, false, 0, -1, OnRowClickCallback};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Check which table the mouse is over and update that table's scroll
        float delta = GetMouseWheelMove() * 30;
        if (IsMouseOverTable(&table1)) {
            UpdateTableScroll(&table1, delta);
        } else if (IsMouseOverTable(&table2)) {
            UpdateTableScroll(&table2, delta);
        }

        // Handle scrollbar dragging
        HandleScrollbar(&table1);
        HandleScrollbar(&table2);

        // Highlight row under the mouse
        HighlightRow(&table1);
        HighlightRow(&table2);

        // Handle row click
        HandleRowClick(&table1);
        HandleRowClick(&table2);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the tables
        DrawTable(&table1);
        DrawTable(&table2);

        EndDrawing();
    }

    DisposeTableRows(numRows, rows1);

//    DisposeTableRows(numRows, rows2);

    for (int i = 0; i < numRows; i++) {
        if (rows2[i].cells[0].text != NULL) {
          free((char *)rows2[i].cells[0].text);  // Free allocated text memory
        }
        if (rows2[i].cells != NULL) {
          free(rows2[i].cells);
        }
    }
    if (rows2 != NULL) {
      free(rows2);
    }

    CloseWindow();

    return 0;
}
