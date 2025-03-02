#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "raylib_table_ui.h"


void DrawTable(Table *table, TableColors tblColors) {
//printf("\t\t DrawTable START\n");
    // Offset for header
    float y = table->positionY - table->scrollOffset + table->rowHeight;
    float x = table->positionX;
    float w = table->width;
    float h = table->height;
//printf("\t\t DrawTable START 11\n");
    // Draw table rectangle
    DrawRectangleLines(x, table->positionY, w, h, tblColors.tableBgColor);
//printf("\t\t DrawTable START 22\n");
    // Draw header
    DrawRectangle(x, table->positionY, w, table->rowHeight, tblColors.headerBgColor);
    float colX = x;
//printf("\t\t DrawTable START 33\n");
    for (int j = 0; j < table->numCells; j++) {
//printf("\t\t DrawTable START 44\n");
        float colWidth = table->columnDefinitions[j]->columnWidthsPercentage * w;
printf("\t\t table->columnDefinitions[%d]->columnName: %s\n", j, table->columnDefinitions[j]->columnName);
        DrawText(table->columnDefinitions[j]->columnName, colX + 10, table->positionY + 10 - 5, 20, tblColors.headerTextColor);
        DrawRectangleLines(colX, table->positionY, colWidth, table->rowHeight, tblColors.headerDivLinesColor);
        colX += colWidth;
    }

    // Draw rows
    for (int i = 0; i < table->numRows; i++) {
        float rowTop = y + i * table->rowHeight;
        if (rowTop >= table->positionY + table->rowHeight && rowTop <= table->positionY + h) {
            Color rowColor = tblColors.rowColor;

            // Highlight row if mouse is over it
            if (table->highlightedRow == i) {
                rowColor = Fade(tblColors.highlightRowColor, 0.3f);
            }

            DrawRectangle(x, rowTop, w, table->rowHeight, rowColor);

            // Draw cells within the row
            colX = x;
            for (int j = 0; j < table->numCells; j++) {
                float colWidth = table->columnDefinitions[j]->columnWidthsPercentage * w;

                switch (table->rows[i].cells[j].type) {
                    case CELL_TYPE_IMAGE: {
                        DrawTexture(table->rows[i].cells[j].icon, colX + 25, rowTop + 10 - 5, WHITE);
                        break;
                    }
                    case CELL_TYPE_TEXT: {
printf("\t CELL_TYPE_TEXT: %s\n", table->rows[i].cells[j].text);
                        DrawText(table->rows[i].cells[j].text, colX + 10, rowTop + 10 - 5, 20, tblColors.cellTextTypeColor);
                        break;
                    }
                    case CELL_TYPE_NUMBER: {
                        char numberText[32];
                        snprintf(numberText, sizeof(numberText), "%d", table->rows[i].cells[j].number);
                        DrawText(numberText, colX + 10, rowTop + 10 - 5, 20, tblColors.cellNumberTypeColor);
                        break;
                    }
                    case CELL_TYPE_DOUBLE: {
                        char valueText[32];
                        snprintf(valueText, sizeof(valueText), "%.2f", table->rows[i].cells[j].value);
                        DrawText(valueText, colX + 10, rowTop + 10 - 5, 20, tblColors.cellDoubleTypeColor);
                        break;
                    }
                    case CELL_TYPE_CHECKBOX: {
                        if (table->rows[i].cells[j].checkbox) {
                            DrawText("[X]", colX + 10, rowTop + 10 - 5, 20, tblColors.cellCheckboxTypeColor);
                        } else {
                            DrawText("[_]", colX + 10, rowTop + 10 - 5, 20, tblColors.cellCheckboxTypeColor);
                        }
                        break;
                    }
                    case CELL_TYPE_ACTIONS: {
                            if (table->rows[i].cells[j].actionsBtnsCount > 0) {
                                int x_offset_spacing = -10;
                                for (int btnCnt = 0; btnCnt < table->rows[i].cells[j].actionsBtnsCount; btnCnt++) {
                                    if (table->rows[i].cells[j].actionsBtns[btnCnt].name != NULL) {
                                        DrawRectangleLines(x_offset_spacing + colX + 20 * (btnCnt + 1) + 2, rowTop + 10 - 5, 22, 22,
                                            tblColors.cellActionTypeColorBtnBg);
                                        DrawRectangleLines(x_offset_spacing + colX + 20 * (btnCnt + 1), rowTop + 10 - 5, 20, 20,
                                            tblColors.cellActionTypeColorBtnFg);
                                        DrawText(table->rows[i].cells[j].actionsBtns[btnCnt].name, x_offset_spacing + colX + 4 + 20 * (btnCnt + 1) + 2, rowTop + 12 - 5, 20,
                                            tblColors.cellActionTypeColorBtnText);
                                        x_offset_spacing += 10;
                                    }
                                  }
                            }
                        break;
                    }
                }

                DrawRectangleLines(colX, rowTop, colWidth, table->rowHeight, tblColors.tableRowDivLinesColor);
                colX += colWidth;
            }
        }
    }

    // Draw scrollbar
    if (table->numRows > table->visibleRows) {
        float scrollHeight = h * (float)table->visibleRows / (float)table->numRows;
        float scrollY = table->positionY + table->scrollOffset / (table->numRows * table->rowHeight) * h;
        DrawRectangle(x + w + 2, scrollY, 10, scrollHeight, tblColors.scrollbarColor);
    }
}

TableColors GetTableDefaultColorsScheme(bool inverse) {
    TableColors tblColors;

    tblColors.tableBgColor = inverse ? LIGHTGRAY : DARKGRAY;
    tblColors.headerBgColor = inverse ? LIGHTGRAY : DARKGRAY;
    tblColors.headerTextColor = inverse ? BLACK : RAYWHITE;
    tblColors.headerDivLinesColor = inverse ? DARKGRAY : GRAY;
    tblColors.rowColor = inverse ? DARKGRAY : LIGHTGRAY;
    tblColors.tableRowDivLinesColor = inverse ? LIGHTGRAY : GRAY;
    tblColors.highlightRowColor = inverse ? DARKGRAY : LIGHTGRAY;
    tblColors.cellTextTypeColor = inverse ? RAYWHITE : BLACK;
    tblColors.cellNumberTypeColor = inverse ? RAYWHITE : BLACK;
    tblColors.cellDoubleTypeColor = inverse ? RAYWHITE : BLACK;
    tblColors.cellCheckboxTypeColor = inverse ? RAYWHITE : BLACK;
    tblColors.scrollbarColor = inverse ? LIGHTGRAY : DARKGRAY;
    tblColors.cellActionTypeColorBtnBg = inverse ? RAYWHITE : BLACK;
    tblColors.cellActionTypeColorBtnFg = inverse ? BLACK : RAYWHITE;
    tblColors.cellActionTypeColorBtnText = inverse ? RAYWHITE : BLACK;

    return tblColors;
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
        for (int j = 0; j < table->numCells; j++) {
            float colWidth = table->columnDefinitions[j]->columnWidthsPercentage * table->width;
            if (mouseX >= colX && mouseX <= colX + colWidth) {
                table->OnRowClickCallback(table->id, table->highlightedRow, table->columnDefinitions[j]->columnName);
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

ColumnDefinition **CreateTableWithHeader(int numCols) {
    ColumnDefinition **columnDefinitions = malloc(numCols * sizeof(ColumnDefinition));
    return columnDefinitions;
}

void AddColumnToTableHeaderColumn(Table *table, ColumnDefinition **columnDefinitions, int columnId, char *columnName, CellType columnType, float columnWidthsPercentage) {
    ColumnDefinition *cd = malloc(sizeof(ColumnDefinition));
    strncpy(cd->columnName, columnName, sizeof(columnName));
    cd->columnType = columnType;
    cd->columnWidthsPercentage = columnWidthsPercentage;
    columnDefinitions[columnId] = cd;
}

TableRow *CreateTableRows(ColumnDefinition** columnDefinition, int numCols, int numRows) {
printf("\n\t CreateTableRows START - numRows: %d, numCols: %d\n", numRows, numCols);
    TableRow *rows = malloc(numRows * sizeof(TableRow));
    return rows;
}

void GenerateTableRows(TableRow* rows, int numRows, int numCols, ColumnDefinition **columnDefinitions) {
printf("\n\t GenerateTableRows START\n");
printf("\n\t GenerateTableRows TableRow *rows malloced \n");
    for (int i = 0; i < numRows; i++) {
printf("\t\t GenerateTableRows for i: %d \n", i);
        rows[i].cells = malloc(numCols * sizeof(Cell));

printf("\t\t GenerateTableRows for i: %d - rows[%d].cells = malloced for numCols: %d \n", i, i, numCols);

        for (int j = 0; j < numCols; j++) {
printf("\t\t\t GenerateTableRows for i: %d - for j: %d - columnDefinitions[j]->columnType: %d\n", i, j, columnDefinitions[j]->columnType);
            switch (columnDefinitions[j]->columnType) {
                case CELL_TYPE_IMAGE: {
printf("\t\t\t\t GenerateTableRows - CELL_TYPE_IMAGE \n");
                    rows[i].cells[j].iconPath = "table-icon2.png";
                    rows[i].cells[j].icon = LoadTexture(rows[i].cells[j].iconPath);
                    break;
                }
                case CELL_TYPE_TEXT: {
printf("\t\t\t\t GenerateTableRows - CELL_TYPE_TEXT \n");
                    rows[i].cells[j].text = malloc(32 * sizeof(char));
                    snprintf((char *)rows[i].cells[j].text, 32, "Row %d", i + 1);
printf("\t\t\t\t\t GenerateTableRows - CELL_TYPE_TEXT - rows[i].cells[j].text: '%s'\n", rows[i].cells[j].text);
                    break;
                }
                case CELL_TYPE_NUMBER: {
printf("\t\t\t\t GenerateTableRows - CELL_TYPE_NUMBER \n");
                    rows[i].cells[j].number = i + 1;
                    break;
                }
                case CELL_TYPE_DOUBLE: {
printf("\t\t\t\t GenerateTableRows - CELL_TYPE_DOUBLE \n");
                    rows[i].cells[j].value = i + 1.1;
                    break;
                }
                case CELL_TYPE_CHECKBOX: {
printf("\t\t\t\t GenerateTableRows - CELL_TYPE_CHECKBOX \n");
                    rows[i].cells[j].checkbox = i % 2;
                    break;
                }
                case CELL_TYPE_ACTIONS: {
printf("\t\t\t\t GenerateTableRows - CELL_TYPE_ACTIONS \n");
                    rows[i].cells[j].actionsBtnsCount = 3;
                    strncpy((char*) rows[i].cells[j].actionsBtns[0].name, "A", 2);
                    strncpy((char*) rows[i].cells[j].actionsBtns[1].name, "B", 2);
                    strncpy((char*) rows[i].cells[j].actionsBtns[2].name, "C", 2);
                    break;
                }
                //TODO: add default impl with panic
          }
        }
    }
printf("\n\n GenerateTableRows END with rows. \n");
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

void DisposeTableColumnDefinitions(int numCols, ColumnDefinition **columnDefinitions) {
    for (int i = 0; i < numCols; i++) {
        if (columnDefinitions[i] != NULL) {
          free(columnDefinitions[i]);
        }
    }
    if (columnDefinitions != NULL) {
      free(columnDefinitions);
    }
}
