#ifndef RAYLIB_TABLE_UI_H
#define RAYLIB_TABLE_UI_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    CELL_TYPE_IMAGE,
    CELL_TYPE_TEXT,
    CELL_TYPE_NUMBER,
    CELL_TYPE_DOUBLE,
    CELL_TYPE_CHECKBOX
} CellType;

typedef struct {
    CellType type;
    union {
        Texture2D icon;
        const char *iconPath;  // For CELL_TYPE_IMAGE
        const char *text;      // For CELL_TYPE_TEXT
        int number;            // For CELL_TYPE_NUMBER
        double value;          // For CELL_TYPE_DOUBLE
        bool checkbox;         // For CELL_TYPE_CHECKBOX
    };
} Cell;

typedef struct {
    Cell *cells;
    int numCells;
} TableRow;

typedef struct {
    int id;
    float positionX;
    float positionY;
    float width;
    float height;
    int numRows;
    float rowHeight;    
    // Percentages
    float *colWidths;

    TableRow *rows;
    float scrollOffset;
    int visibleRows;
    const char **columnNames;
    bool isDragging;
    float dragOffsetY;
    int highlightedRow;
    void (*OnRowClickCallback)(int tableId, int row, const char *columnName);
} Table;

#endif /* RAYLIB_TABLE_UI_H */
