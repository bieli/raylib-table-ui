#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raylib_table_ui.h"


void DrawTable(Table *table) {
    // Offset for header
    float y = table->positionY - table->scrollOffset + table->rowHeight;
    float x = table->positionX;
    float w = table->width;
    float h = table->height;

    // Draw table rectangle
    DrawRectangleLines(x, table->positionY, w, h, DARKGRAY);

    // Draw header
    DrawRectangle(x, table->positionY, w, table->rowHeight, DARKGRAY);
    float colX = x;
    for (int j = 0; j < table->rows[0].numCells; j++) {
        float colWidth = table->colWidths[j] * w;
        DrawText(table->columnNames[j], colX + 10, table->positionY + 10 - 5, 20, RAYWHITE);
        DrawRectangleLines(colX, table->positionY, colWidth, table->rowHeight, GRAY);
        colX += colWidth;
    }

    // Draw rows
    for (int i = 0; i < table->numRows; i++) {
        float rowTop = y + i * table->rowHeight;
        if (rowTop >= table->positionY + table->rowHeight && rowTop <= table->positionY + h) {
            Color rowColor = LIGHTGRAY;

            // Highlight row if mouse is over it
            if (table->highlightedRow == i) {
                rowColor = Fade(LIGHTGRAY, 0.3f);
            }

            DrawRectangle(x, rowTop, w, table->rowHeight, rowColor);

            // Draw cells within the row
            colX = x;
            for (int j = 0; j < table->rows[i].numCells; j++) {
                float colWidth = table->colWidths[j] * w;

                switch (table->rows[i].cells[j].type) {
                    case CELL_TYPE_IMAGE: {
                        DrawTexture(table->rows[i].cells[j].icon, colX + 25, rowTop + 10 - 5, WHITE);
                        break;
                    }
                    case CELL_TYPE_TEXT: {
                        DrawText(table->rows[i].cells[j].text, colX + 10, rowTop + 10 - 5, 20, BLACK);
                        break;
                    }
                    case CELL_TYPE_NUMBER: {
                        char numberText[32];
                        snprintf(numberText, sizeof(numberText), "%d", table->rows[i].cells[j].number);
                        DrawText(numberText, colX + 10, rowTop + 10 - 5, 20, BLACK);
                        break;
                    }
                    case CELL_TYPE_DOUBLE: {
                        char valueText[32];
                        snprintf(valueText, sizeof(valueText), "%.2f", table->rows[i].cells[j].value);
                        DrawText(valueText, colX + 10, rowTop + 10 - 5, 20, BLACK);
                        break;
                    }
                    case CELL_TYPE_CHECKBOX: {
                        if (table->rows[i].cells[j].checkbox) {
                            DrawText("[X]", colX + 10, rowTop + 10 - 5, 20, BLACK);
                        } else {
                            DrawText("[_]", colX + 10, rowTop + 10 - 5, 20, BLACK);
                        }
                        break;
                    }
                }

                DrawRectangleLines(colX, rowTop, colWidth, table->rowHeight, GRAY);
                colX += colWidth;
            }
        }
    }

    // Draw scrollbar
    if (table->numRows > table->visibleRows) {
        float scrollHeight = h * (float)table->visibleRows / (float)table->numRows;
        float scrollY = table->positionY + table->scrollOffset / (table->numRows * table->rowHeight) * h;
        DrawRectangle(x + w + 2, scrollY, 10, scrollHeight, DARKGRAY);
    }
}

void UpdateTableScroll(Table *table, float delta) {
    table->scrollOffset += delta;
    if (table->scrollOffset < 0) table->scrollOffset = 0;
    if (table->scrollOffset > table->numRows * table->rowHeight - table->height + table->rowHeight)
        table->scrollOffset = table->numRows * table->rowHeight - table->height + table->rowHeight;
}

void HandleScrollbar(Table *table) {
    float w = table->width;
    float h = table->height;
    float scrollHeight = h * (float)table->visibleRows / (float)table->numRows;
    float scrollY = table->positionY + table->scrollOffset / (table->numRows * table->rowHeight) * h;
    float mouseX = GetMouseX();
    float mouseY = GetMouseY();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        mouseX >= table->positionX + w + 2 && mouseX <= table->positionX + w + 10 + 2 &&
        mouseY >= scrollY && mouseY <= scrollY + scrollHeight) {
        table->isDragging = true;
        table->dragOffsetY = mouseY - scrollY;
    }

    if (table->isDragging) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            float newY = mouseY - table->dragOffsetY;
            float maxScrollY = table->positionY + h - scrollHeight;
            if (newY < table->positionY) newY = table->positionY;
            if (newY > maxScrollY) newY = maxScrollY;
            table->scrollOffset = (newY - table->positionY) / h * (table->numRows * table->rowHeight);
        } else {
            table->isDragging = false;
        }
    }
}

void HighlightRow(Table *table) {
    // Offset for header
    float y = table->positionY + table->rowHeight;
    float x = table->positionX;
    float w = table->width;
    float h = table->height;
    float mouseX = GetMouseX();
    float mouseY = GetMouseY();

    // Reset highlighted row
    table->highlightedRow = -1;

    if (mouseX >= x && mouseX <= x + w && mouseY >= table->positionY + table->rowHeight && mouseY <= table->positionY + h) {
        int row = (int)((mouseY - y + table->scrollOffset) / table->rowHeight);
        if (row >= 0 && row < table->numRows) {
            table->highlightedRow = row;
        }
    }
}

void HandleRowClick(Table *table) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && table->highlightedRow != -1) {
        float mouseX = GetMouseX();
        float colX = table->positionX;
        for (int j = 0; j < table->rows[table->highlightedRow].numCells; j++) {
            float colWidth = table->colWidths[j] * table->width;
            if (mouseX >= colX && mouseX <= colX + colWidth) {
                table->OnRowClickCallback(table->id, table->highlightedRow, table->columnNames[j]);
                break;
            }
            colX += colWidth;
        }
    }
}

bool IsMouseOverTable(Table *table) {
    float mouseX = GetMouseX();
    float mouseY = GetMouseY();
    return mouseX >= table->positionX && mouseX <= table->positionX + table->width &&
           mouseY >= table->positionY && mouseY <= table->positionY + table->height;
}


TableRow* CreateTableRows(int numRows, int numCols) {
    TableRow *rows1 = malloc(numRows * sizeof(TableRow));
    for (int i = 0; i < numRows; i++) {
        rows1[i].numCells = numCols;
        rows1[i].cells = malloc(rows1[i].numCells * sizeof(Cell));

        rows1[i].cells[0].type = CELL_TYPE_IMAGE;
        rows1[i].cells[0].iconPath = "table-icon2.png";
        rows1[i].cells[0].icon = LoadTexture(rows1[i].cells[0].iconPath);

        rows1[i].cells[1].type = CELL_TYPE_TEXT;
        rows1[i].cells[1].text = malloc(32 * sizeof(char));
        snprintf((char *)rows1[i].cells[1].text, 32, "Row %d", i + 1);

        rows1[i].cells[2].type = CELL_TYPE_NUMBER;
        rows1[i].cells[2].number = i + 1;

        rows1[i].cells[3].type = CELL_TYPE_DOUBLE;
        rows1[i].cells[3].value = i + 1.1;
    }
    
    return rows1;
}

void DisposeTableRows(int numRows, TableRow* rows) {
    for (int i = 0; i < numRows; i++) {
        if (rows[i].cells[0].iconPath != NULL) {
          UnloadTexture(rows[i].cells[0].icon);
        }

        if (rows[i].cells[1].text != NULL) {
          free((char *)rows[i].cells[1].text);
        }
        if (rows[i].cells != NULL) {
          free(rows[i].cells);
        }
    }
    if (rows != NULL) {
      free(rows);
    }
}
