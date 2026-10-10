#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/raylib/clay_renderer_raylib.c"

#include <stdio.h>
#include <stdlib.h>

void HandleClayErrors(Clay_ErrorData e) {
    printf("%s\n", e.errorText.chars);
}

int main(void) {
    uint64_t size = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(size, malloc(size));
    Clay_Initialize(arena, (Clay_Dimensions){1000, 700}, (Clay_ErrorHandler){ HandleClayErrors });


    Clay_Raylib_Initialize(1000, 700, "Clay basics", FLAG_WINDOW_RESIZABLE);

    while (!WindowShouldClose()) {
        Clay_SetLayoutDimensions((Clay_Dimensions){ GetScreenWidth(), GetScreenHeight() });
        Vector2 mouse = GetMousePosition();
        Clay_SetPointerState((Clay_Vector2){ mouse.x, mouse.y }, IsMouseButtonDown(MOUSE_BUTTON_LEFT));

        //--------------------------------------------------------------------------------
        Clay_BeginLayout();

        CLAY({
            .id = CLAY_ID("Panel"),
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },  // fill the window
                .padding = CLAY_PADDING_ALL(24),
                .childAlignment = {.x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP}
            },
            .backgroundColor = {30, 30, 40, 255}
        }) {
            //childs
            CLAY({
                .id = CLAY_ID("Button"),
                .layout = { .sizing = { CLAY_SIZING_FIXED(200), CLAY_SIZING_FIXED(60) } },
                .backgroundColor = Clay_Hovered() ? (Clay_Color){90, 160, 255, 255}
                                                  : (Clay_Color){60, 120, 220, 255},
                .cornerRadius = CLAY_CORNER_RADIUS(8)
            }) {}
        }
        //--------------------------------------------------------------------------------
        Clay_RenderCommandArray commands = Clay_EndLayout();

        //exmp CLICK
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            Clay_PointerOver(Clay_GetElementId(CLAY_STRING("Button")))) {
            printf("Button clicked\n");
        }

        //renderer
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(commands, NULL);
        EndDrawing();
        //--------------------------------------------------------------------------------
    }

    Clay_Raylib_Close();
    return 0;
}