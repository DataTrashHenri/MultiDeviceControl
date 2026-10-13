#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/raylib/clay_renderer_raylib.c"

#include <stdio.h>
#include <stdlib.h>

void HandleClayErrors(Clay_ErrorData e) {
    printf("%s\n", e.errorText.chars);
}
const Clay_Color COLOR_BLUE = (Clay_Color) {111, 173, 162, 255};
int main(void) {
    uint64_t size = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(size, malloc(size));
    Clay_Initialize(arena, (Clay_Dimensions){1000, 700}, (Clay_ErrorHandler){ HandleClayErrors });


    Clay_Raylib_Initialize(1000, 700, "Clay basics", FLAG_WINDOW_RESIZABLE);

    // AI START
    // after Clay_Raylib_Initialize (needs the window to exist)
    Font fonts[1];

    // Load codepoints 32..255 so German characters (ä ö ü ß) work
    int cpCount = 256 - 32;
    int codepoints[224];
    for (int i = 0; i < cpCount; i++) codepoints[i] = 32 + i;

    fonts[0] = LoadFontEx("../host/clay/font.ttf", 48, codepoints, cpCount);


    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);

    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);
    printf("CWD: %s\n", GetWorkingDirectory());
    // AI END

    while (!WindowShouldClose()) {
        Clay_SetLayoutDimensions((Clay_Dimensions){ (int)GetScreenWidth(), (int)GetScreenHeight() });
        Vector2 mouse = GetMousePosition();
        Clay_SetPointerState((Clay_Vector2){ mouse.x, mouse.y }, IsMouseButtonDown(MOUSE_BUTTON_LEFT));

        //--------------------------------------------------------------------------------
        //AI start
        Vector2 wheel = GetMouseWheelMoveV();            // raylib; use your platform's equivalent

        float dx = wheel.x;
        if (dx == 0) dx = wheel.y;                       // normal wheel -> horizontal scroll
        // or only with Shift: if (IsKeyDown(KEY_LEFT_SHIFT)) dx = wheel.y;

        Clay_SetPointerState((Clay_Vector2){ GetMouseX(), GetMouseY() }, IsMouseButtonDown(MOUSE_LEFT_BUTTON));
        Clay_UpdateScrollContainers(
            true,
            (Clay_Vector2){ dx*2, 0 },                // scale it up, raw wheel values are ~1.0 per notch
            GetFrameTime()
        );
        //AI end
        Clay_BeginLayout();

        CLAY({
            .id = CLAY_ID("Main"),
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },  // fill the window
                .padding = CLAY_PADDING_ALL(8),
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childAlignment =  {CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_TOP},
                .childGap = 8
            },
            .backgroundColor = {30, 30, 40, 255}
        }) {
            //childs
            CLAY({
                .id = CLAY_ID("Titlebar"),
                .layout = { .sizing = { CLAY_SIZING_PERCENT(1), CLAY_SIZING_FIXED(60) }, .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }},
                .backgroundColor = (Clay_Color){60, 120, 220, 255},
                .cornerRadius = CLAY_CORNER_RADIUS(4),
            }) {
                CLAY_TEXT(CLAY_STRING("MDC - Multi Device Management"),CLAY_TEXT_CONFIG({
                    .fontId = 0,
                    .fontSize = 24,
                    .letterSpacing = 1,
                    .textColor = {255,255,255,255},
                }));
            }
            CLAY({
            .id = CLAY_ID("Client List"),
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .childGap = 4,
                .sizing = {CLAY_SIZING_GROW(0),CLAY_SIZING_FIXED(150) },
            },
            .clip = {.horizontal = true, .childOffset = Clay_GetScrollOffset()}
            }) {
                for (int i = 0; i < 3; i++) {
                    CLAY({
                         //.id = CLAY_ID("Client 1"),
                         .layout = { .sizing = {CLAY_SIZING_FIT(400), CLAY_SIZING_GROW(0)}},
                         .backgroundColor = (Clay_Color){10, 80, 30, 255},
                         .cornerRadius = CLAY_CORNER_RADIUS(8)
                         }) {}
                }
            }

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
        Clay_Raylib_Render(commands, fonts);
        EndDrawing();
        //--------------------------------------------------------------------------------
    }

    Clay_Raylib_Close();
    return 0;
}