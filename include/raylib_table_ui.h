#ifndef RAYLIB_TABLE_UI_H
#define RAYLIB_TABLE_UI_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define  ACTION_MAX_CHARS         16
#define  ACTION_MAX_PER_ROW       4
#define  TABLE_MAX_COLUMNS_COUNT  10

typedef enum {
    CELL_TYPE_IMAGE,
    CELL_TYPE_TEXT,
    CELL_TYPE_NUMBER,
    CELL_TYPE_DOUBLE,
    CELL_TYPE_CHECKBOX,
    CELL_TYPE_ACTIONS,
} CellType;

typedef struct {
        char name[ACTION_MAX_CHARS];
} ActionBtn;

typedef struct {
    CellType type;
    int actionsBtnsCount;
    union {
        Texture2D icon;
        const char *iconPath;  // For CELL_TYPE_IMAGE
        const char *text;      // For CELL_TYPE_TEXT
        int number;            // For CELL_TYPE_NUMBER
        double value;          // For CELL_TYPE_DOUBLE
        bool checkbox;         // For CELL_TYPE_CHECKBOX
        ActionBtn actionsBtns[ACTION_MAX_PER_ROW];         // For CELL_TYPE_ACTIONS
    };
} Cell;

typedef struct {
    Cell *cells;
} TableRow;

typedef struct {
        char columnName[ACTION_MAX_CHARS];
        CellType columnType;
        float columnWidthsPercentage;
} ColumnDefinition;

typedef struct {
    int id;
    float positionX;
    float positionY;
    float width;
    float height;
    int numRows;
    int numCells;
    float rowHeight;
    TableRow *rows;
    float scrollOffset;
    int visibleRows;
    ColumnDefinition **columnDefinitions;
    bool isDragging;
    float dragOffsetY;
    int highlightedRow;
    void (*OnRowClickCallback)(int tableId, int row, const char *columnName);
    int numCollsCnt;
} Table;


typedef struct {
  Color tableBgColor;
  Color headerBgColor;
  Color headerTextColor;
  Color headerDivLinesColor;
  Color rowColor;
  Color tableRowDivLinesColor;
  Color highlightRowColor;
  Color cellTextTypeColor;
  Color cellNumberTypeColor;
  Color cellDoubleTypeColor;
  Color cellCheckboxTypeColor;
  Color cellActionTypeColorBtnBg;
  Color cellActionTypeColorBtnFg;
  Color cellActionTypeColorBtnText;

  Color scrollbarColor;
} TableColors;


void DrawTable(Table *table, TableColors tblColors);
TableColors GetTableDefaultColorsScheme(bool inverse);
void UpdateTableScroll(Table *table, float delta);
void HandleScrollbar(Table *table);
void HighlightRow(Table *table);
void HandleRowClick(Table *table);
void OnRowClickCallback(int tableId, int row, const char *columnName);
bool IsMouseOverTable(Table *table);

Table CreateTable(
    int id,
    float positionX,
    float positionY,
    float width,
    float height,
    int numRows,
    int numCells,
    float rowHeight,
    TableRow *rows,
    float scrollOffset,
    int visibleRows,
    ColumnDefinition **columnDefinitions,
    bool isDragging,
    float dragOffsetY,
    int highlightedRow,
    void *callback
);
ColumnDefinition **CreateTableHeader(int numCols);
void AddColumnToTableHeaderColumn(Table *table, ColumnDefinition **columnDefinitions, char *columnName, CellType columnType, float columnWidthsPercentage);
TableRow *CreateTableRows(ColumnDefinition** columnDefinition, int numCols, int numRows);
void GenerateTableRows(TableRow* rows, int numRows, int numCols, ColumnDefinition **columnDefinitions);

void DisposeTableRows(int numRows, TableRow* rows);
void DisposeTableColumnDefinitions(int numCols, ColumnDefinition **columnDefinitions);

#endif /* RAYLIB_TABLE_UI_H */
