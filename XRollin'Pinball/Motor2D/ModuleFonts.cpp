#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"

#include<string.h>

// Constructor
ModuleFonts::ModuleFonts(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleFonts::~ModuleFonts()
{}

// Load new texture from file path
int ModuleFonts::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = App->textures->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].graphic == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	fonts[id].graphic = tex; // graphic: pointer to the texture
	fonts[id].rows = rows; // rows: rows of characters in the texture

	fonts[id].len = strlen(characters);

	fonts[id].row_chars = fonts[id].len / fonts[id].rows;

	for (int i = 0; i < strlen(characters); i++)
	{
		fonts[id].table[i] = characters[i];
	}

	uint wTex = 0, hTex = 0;
	App->textures->GetSize(tex, wTex, hTex);

	fonts[id].char_w = (wTex / fonts[id].len)*rows;
	fonts[id].char_h = hTex / rows;


	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void ModuleFonts::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		App->textures->Unload(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

// Render text using a bitmap font
void ModuleFonts::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].graphic == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect rect;
	uint len = strlen(text);

	rect.w = font->char_w;
	rect.h = font->char_h;

	int inTable_x = -1, inTable_y = -1;
	for (uint i = 0; i < len; ++i)
	{

		for (int k = 0; k < fonts[font_id].rows; k++) {
			for (int j = 0; j < fonts[font_id].row_chars; j++)
			{
				if (text[i] == fonts->table[j + (k * fonts[font_id].row_chars)]) {
					inTable_x = j;
					inTable_y = k;
					break;
				}
			}
		}
		float table_x;
		if (inTable_x != -1 && inTable_y != -1) {
			SDL_Rect charRec;
			charRec.x = inTable_x * font->char_w;
			charRec.y = inTable_y * font->char_h;
			charRec.w = font->char_w;
			charRec.h = font->char_h;
			if (charRec.x <= 128 && charRec.y < 32)
				App->renderer->Blit(font->graphic, x, y, &charRec);
			x += font->char_w;
		}
	}
}
