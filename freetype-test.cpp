#include <ft2build.h>
#include FT_FREETYPE_H

#include <hb.h>
#include <hb-ft.h>

#include <iostream>

//This file exists to check that programs that use freetype / harfbuzz link properly in this base code.
//You probably shouldn't be looking here to learn to use either library.

#define WIDTH 128
#define HEIGHT 96

unsigned char image[HEIGHT][WIDTH];

void draw_bitmap(FT_Bitmap* bitmap, FT_Int x, FT_Int y)
{
	FT_Int  i, j, p, q;
 	FT_Int  x_max = x + bitmap->width;
  	FT_Int  y_max = y + bitmap->rows;


  	/* for simplicity, we assume that `bitmap->pixel_mode' */
  /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

 	 for ( i = x, p = 0; i < x_max; i++, p++ )
  	{
    	for ( j = y, q = 0; j < y_max; j++, q++ )
    	{	
      	if ( i < 0      || j < 0       ||
        	   i >= WIDTH || j >= HEIGHT )
        	continue;

      	image[j][i] |= bitmap->buffer[q * bitmap->width + p];
    }
  }
}

void show_image( void )
{
  int  i, j;


  for ( i = 0; i < HEIGHT; i++ )
  {
    for ( j = 0; j < WIDTH; j++ )
      putchar( image[i][j] == 0 ? ' '
                                : image[i][j] < 128 ? '+'
                                                    : '*' );
    putchar( '\n' );
  }
}

// "Bayon-Regular.ttf"

int main(int argc, char **argv) {

	char* filename = argv[1];
	char* text = argv[2];

	//Initialize FreeType
	FT_Library library;
	FT_Face face;
	FT_Init_FreeType( &library );
	FT_New_Face(library, filename, 0, &face);
	FT_Set_Char_Size(face, 28 * 64, 0, 72, 0);

	//Harfbuzz from codepoint to glyph
	hb_buffer_t *buf = hb_buffer_create();
	hb_buffer_add_utf8(buf, text, -1, 0, -1);

	hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
	hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
	hb_buffer_set_language(buf, hb_language_from_string("en", -1));
	// hb_buffer_guess_segment_properties(buf);

	// hb_blob_t *blob = hb_blob_create_from_file(filename);
	// hb_face_t *face = hb_face_create(blob, 0);
	hb_font_t *font = hb_ft_font_create(face, NULL);

	hb_shape(font, buf, NULL, 0);

	unsigned int glyph_count;
	hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
	hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

	// hb_position_t cursor_x = 0;
	// hb_position_t cursor_y = 0;
	// for(unsigned int i = 0; i < glyph_count; i++)
	// {
	// 	hb_codepoint_t glyphid = glyph_info[i].codepoint;
	// 	hb_position_t x_offset = glyph_pos[i].x_offset;
	// 	hb_position_t y_offset = glyph_pos[i].y_offset;
	// 	hb_position_t x_advance = glyph_pos[i].x_advance;
	// 	hb_position_t y_advance = glyph_pos[i].y_advance;
	// 	cursor_x += x_advance;
	// 	cursor_y += y_advance;
	// }

	FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	// FT_UInt glyph_index;

	int n, num_chars;
	int target_height = 96;
	double angle = (0.0 / 360) * 3.14159 * 2;
	num_chars = (int)strlen(text);
	
	slot = face->glyph;

	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
	
	pen.x = 0 * 64;
	pen.y = (target_height - 32) * 64;

	for(n = 0; n < num_chars; n++)
	{
		face->glyph->glyph_index = glyph_info[n].codepoint;
		face->glyph->bitmap_left = glyph_pos[n].x_offset;
		face->glyph->bitmap_top = glyph_pos[n].y_offset;
		face->glyph->advance.x = glyph_pos[n].x_advance;
		face->glyph->advance.y = glyph_pos[n].y_advance;

		FT_Set_Transform(face, &matrix, &pen);

		FT_Load_Char(face, text[n], FT_LOAD_RENDER);

		draw_bitmap(&slot->bitmap, slot->bitmap_left, target_height - slot->bitmap_top);

		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}

	show_image();

	FT_Done_Face(face);
	FT_Done_FreeType(library);
	
	hb_buffer_destroy(buf);

	std::cout << "It worked?" << std::endl;

	std::cin.get();
}
