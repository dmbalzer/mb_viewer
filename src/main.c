#include <raylib.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#define RRES_IMPLEMENTATION
#include "rres.h"
#include "mb.h"

Texture* textures = NULL;

void process_drop(void)
{
	FilePathList fpl = LoadDroppedFiles();
	for ( int i = 0; i < fpl.count; i++ ) {
		if ( IsFileExtension(fpl.paths[i], ".png") ) {
			arrput(textures, LoadTexture(fpl.paths[i]));
		}
	}
	UnloadDroppedFiles(fpl);
}

void draw_textures(void)
{
	for ( int i = 0; i < arrlen(textures); i++ ) {
		DrawTexture(textures[i], 16, 16, WHITE);
	}
}

static unsigned char *LoadDataBuffer(rresResourceChunkData data, unsigned int rawSize)
{
    unsigned char *buffer = (unsigned char *)RRES_CALLOC((data.propCount + 1)*sizeof(unsigned int) + rawSize, 1);
    
    memcpy(buffer, &data.propCount, sizeof(unsigned int));
    for (int i = 0; i < data.propCount; i++) memcpy(buffer + (i + 1)*sizeof(unsigned int), &data.props[i], sizeof(unsigned int));
    memcpy(buffer + (data.propCount + 1)*sizeof(unsigned int), data.raw, rawSize);
    
    return buffer;
}

int main(void)
{
	InitWindow(960, 640, "Modbus Viewer");
	SetTargetFPS(30);
	Font font = LoadFontEx("assets/Roboto-Regular.ttf", 18, NULL, 0);
	void* ctx = mb_ctx_new("192.168.0.12", 1502);
	
	/* RRES */
	FILE* file = fopen("data/resources.rres", "wb");
	
	rresFileHeader header = {
		.id[0] = 'r',
		.id[1] = 'r',
		.id[2] = 'e',
		.id[3] = 's',
		.version = 100,
		.chunkCount = 1,
		.cdOffset = 0,
		.reserved = 0
	};
	
	fwrite(&header, sizeof(rresFileHeader), 1, file);
	
	rresResourceChunkInfo chunk_info = { 0 };
	rresResourceChunkData chunk_data = { 0 };

	/*RRES Chunk Image*/
	Image img = LoadImage("assets/pump_off.png");

	chunk_info.type[0] = 'I';
	chunk_info.type[1] = 'M';
	chunk_info.type[2] = 'G';
	chunk_info.type[3] = 'E';
	chunk_info.id = rresComputeCRC32("assets/pump_off.png", strlen("assets/pump_off.png"));
	chunk_info.compType = RRES_COMP_NONE;
	chunk_info.cipherType = RRES_CIPHER_NONE;
	chunk_info.flags = 0;
	chunk_info.baseSize = 5*sizeof(unsigned int) + GetPixelDataSize(img.width, img.height, img.format);
	chunk_info.packedSize = chunk_info.baseSize;
	chunk_info.nextOffset = 0;
	chunk_info.reserved = 0;
	
	chunk_data.propCount = 4;
	chunk_data.props = (unsigned int*)RRES_CALLOC(chunk_data.propCount, sizeof(unsigned int));
	chunk_data.props[0] = img.width;
	chunk_data.props[1] = img.height;
	chunk_data.props[2] = img.format;
	chunk_data.props[3] = img.mipmaps;
	chunk_data.raw = img.data;
	
	unsigned char* buffer = LoadDataBuffer(chunk_data, GetPixelDataSize(img.width, img.height, img.format));
	chunk_info.crc32 = rresComputeCRC32(buffer, chunk_info.packedSize);

	fwrite(&chunk_info, sizeof(rresResourceChunkInfo), 1, file);
	fwrite(buffer, 1, chunk_info.packedSize, file);
	
	memset(&chunk_info, 0, sizeof(rresResourceChunkInfo));
	RRES_FREE(chunk_data.props);
	RRES_FREE(buffer);
	UnloadImage(img);

	fclose(file);
	

	while ( !WindowShouldClose() ) {
		
		if ( IsFileDropped() ) process_drop();

		BeginDrawing();
		ClearBackground(RAYWHITE);

		draw_textures();
		
		EndDrawing();
	}

	for ( int i = 0; i < arrlen(textures); i++ ) {
		UnloadTexture(textures[i]);
	}

	mb_ctx_close(ctx);
	mb_ctx_free(ctx);
	UnloadFont(font);
	CloseWindow();
	return 0;
}
