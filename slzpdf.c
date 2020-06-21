// Note: Visual C++ Runtime Library does not support %zu, but uses %Iu.
// Should you compile this for another platform, replace all `%Iu` with `%zu`.

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

FILE* f;
size_t n = 1;
long xref[1000];
size_t ref_catalog;
size_t ref_info = 0;

void begin_file(const char* filename) {
	f = fopen(filename, "wb");
	if (!f) exit(1);
	// ±×÷
	fputs("%PDF-1.3\n%\xc2\xb1\xc3\x97\xc3\xb7\n\n", f);
}

void end_file() {
	long startxref = ftell(f);
	fprintf(f, "xref\n0 %Iu\n", n);
	for (size_t i = 0; i < n; i++) {
		fprintf(f, "%010ld %s \n", xref[i], i ? "00000 n" : "65535 f");
	}
	fprintf(f, "trailer\n<</Root %Iu 0 R/Size %Iu", ref_catalog, n);
	if (ref_info) fprintf(f, "/Info %Iu 0 R", ref_info);
	fprintf(f, ">>\nstartxref\n%ld\n%%%%EOF\n", startxref);
	fclose(f);
}

long stream_start;
long stream_length_field;
size_t begin_object(const char* type) {
	if (type) {
		if (strcmp(type, "Catalog") == 0) ref_catalog = n;
		if (strcmp(type, "Info") == 0) {
			ref_info = n;
			type = NULL;
		}
	}
	xref[n] = ftell(f);
	fprintf(f, "%Iu 0 obj\n<<", n++);
	if (type) fprintf(f, "/Type/%s", type);
	stream_start = -1;
	stream_length_field = -1;
	return n - 1;
}

void end_object() {
	if (stream_start == -1) fputs(">>", f);
	fputs("\nendobj\n\n", f);
}

void begin_stream() {
	fputs("/Length ", f);
	stream_length_field = ftell(f);
	fputs("__________>>\nstream\n", f);
	stream_start = ftell(f);
}

void end_stream() {
	long length = ftell(f) - stream_start;
	fputs("\nendstream", f);
	fseek(f, stream_length_field, SEEK_SET);
	fprintf(f, "%10ld", length);
	fseek(f, 0, SEEK_END);
}

void copy_file(const char* filename) {
	static unsigned char* buffer = NULL;
	if (!buffer) buffer = malloc(33554432);
	FILE* fin = fopen(filename, "rb");
	if (!fin) {
		fprintf(stderr, "error in opening file %s\n", filename);
		exit(1);
	}
	do {
		fwrite(buffer, 1, fread(buffer, 1, sizeof(buffer), fin), f);
	} while (!feof(fin));
	fclose(fin);
}

const char* date_string(time_t t) {
	static char buffer[24];
	buffer[0] = 0;
	// Actually the range of %S is [00, 60], whereas SS is 00 through 59. But who cares.
	// Timezone data are not encoded.
	strftime(buffer, sizeof(buffer), "D:%Y%m%d%H%M%S", localtime(&t));
	return buffer;
}

// Gets the GIF size from the array of data passed to the function
bool get_gif_size(FILE* f, int* width, int* height) {
	if (fgetc(f) != 'G') return false;
	if (fgetc(f) != 'I') return false;
	if (fgetc(f) != 'F') return false;
	fgetc(f);
	fgetc(f);
	fgetc(f);
	*height = fgetc(f);
	*height |= fgetc(f) << 8;
	*width = fgetc(f);
	*width |= fgetc(f) << 8;
	return true;
}

// Gets the JPEG size from the array of data passed to the function, file reference: http://www.obrador.com/essentialjpeg/headerinfo.htm
bool get_jpeg_size(FILE* f, int* width, int* height) {
	if (fgetc(f) != 0xff) return false;
	if (fgetc(f) != 0xd8) return false;
	if (fgetc(f) != 0xff) return false;
	fgetc(f);
	unsigned short block_length = fgetc(f) << 8;
	block_length |= fgetc(f);
	// Retrieve the block length of the first block since the first block will not contain the size of file
	while (!feof(f)) {
		// Increase the file index to get to the next block
		fseek(f, ftell(f) + block_length - 2, SEEK_SET);
		if (fgetc(f) != 0xff) return false;
		// 0xffc0 is the "Start of frame" marker for baseline images.
		// 0xffc1 (extended), 0xffc2 (progressive) and 0xffc3 (lossless) are also possible.
		if ((fgetc(f) & 0xfc) == 0xc0) {
			// The structure of the 0xffc0 block is quite simple:
			// [0xffc0][u16 length][u8 precision][u16 x][u16 y]
			fgetc(f);
			fgetc(f);
			fgetc(f);
			*height = fgetc(f) << 8;
			*height |= fgetc(f);
			*width = fgetc(f) << 8;
			*width |= fgetc(f);
			return true;
		} else {
			block_length = fgetc(f) << 8; // Go to the next block
			block_length |= fgetc(f);
		}
	}
	// If this point is reached then no size was found
	return false;
}

int main(int count, char** filenames) {
	count--;
	filenames++;
	int page_width = 595;
	int page_height = 842;
	begin_file("slzpdf-output.pdf");

	begin_object("Catalog");
	fprintf(f, "/Pages 2 0 R");
	end_object();

	begin_object("Pages");
	fprintf(f, "/Count %d/Kids [", count);
	for (int i = 0; i < count; i++) {
		fprintf(f, "%d 0 R ", i * 3 + 3);
	}
	fprintf(f, "]/MediaBox [0 0 %d %d]", page_width, page_height);
	end_object();

	for (int i = 0; i < count; i++) {
		printf("[%d] %s\n", i, filenames[i]);
		int width, height;
		int box_x, box_y, box_width, box_height;
		const char* ext = strrchr(filenames[i], '.');
		if (ext) {
			ext++;
		} else {
			ext = "";
		}
		if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0) {
			FILE* fj = fopen(filenames[i], "rb");
			get_jpeg_size(fj, &width, &height);
			fclose(fj);
			box_x = 0;
			box_width = page_width;
			box_height = height * box_width / width;
			box_y = (page_height - box_height) / 2;
			if (box_y < 0) {
				box_y = 0;
				box_height = page_height;
				box_width = width * box_height / height;
				box_x = (page_width - box_width) / 2;
			}

			begin_object("Page");
			fprintf(f, "/Parent 2 0 R/Contents %d 0 R/Resources<</XObject<</a %d 0 R>> >>", i * 3 + 4, i * 3 + 5);
			end_object();

			begin_object(NULL);
			begin_stream();
			fprintf(f, "q %d 0 0 %d %d %d cm /a Do Q", box_width, box_height, box_x, box_y);
			end_stream();
			end_object();

			begin_object("XObject");
			fprintf(f, "/Subtype/Image/Filter/DCTDecode/ColorSpace/DeviceRGB/BitsPerComponent 8/Width %d/Height %d", width, height);
			begin_stream();
			copy_file(filenames[i]);
			end_stream();
			end_object();
		} else {
			begin_object("Page");
			fprintf(f, "/Parent 2 0 R/Contents %d 0 R/Resources<</Font<</a %d 0 R>> >>", i * 3 + 4, i * 3 + 5);
			end_object();

			begin_object(NULL);
			begin_stream();
			int font_size = 12;
			fprintf(f, "BT /a %d Tf %d TL 20 %d Td (", font_size, font_size, page_height - font_size - 20);
			{
				FILE* ft = fopen(filenames[i], "r");
				int ch;
				while ((ch = fgetc(ft)) != EOF) {
					if (ch == '\n') {
						fprintf(f, ") Tj\nT* (");
					} else {
						if (strchr("()\\", ch)) fputc('\\', f);
						fputc(ch, f);
					}
				}
				fclose(ft);
			}
			fprintf(f, ") Tj ET");
			end_stream();
			end_object();

			begin_object("Font");
			fprintf(f, "/Subtype/Type1/BaseFont/Courier");
			end_object();
		}
	}

	begin_object("Info");
	fprintf(f, "/Producer(SLZPDF 1.0)/CreationDate(%s)", date_string(time(NULL)));
	end_object();

	end_file();
	return 0;
}
