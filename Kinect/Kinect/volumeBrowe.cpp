//read the volume file from ReconstructMe and calcluate
//the girth of each layer.
//volume size 256x256x256
//vox size 1000/256


#include <reconstructmesdk/reme.h>
#include <conio.h>
#include <iostream>
#include <OpenNI.h>


using namespace std;



int main (int argc, char** argv)
{

	// Create a new context
	reme_context_t c;
	reme_context_create(&c);


	reme_options_t o;
	reme_options_create(c, &o);


	// Create a new volume
	reme_volume_t v;
	reme_volume_create(c, &v);

	// Retrieve num_slices
	int num_slices, resX, resY;

	// Choose the OpenCL device, Mid quality for 512 512 256 
	reme_context_bind_reconstruction_options(c, o);
	reme_options_set_int(c, o, "device_id", 0);
	reme_options_set_int(c, o, "volume.resolution.x", 512); 
	reme_options_set_int(c, o, "volume.resolution.y", 512);
	reme_options_set_int(c, o, "volume.resolution.z", 256);
	reme_context_compile(c);


	reme_options_get_int(c, o, "volume.resolution.x", &resX);
	reme_options_get_int(c, o, "volume.resolution.y", &resY);
	reme_options_get_int(c, o, "volume.resolution.z", &num_slices);
	printf("Volume size in x-dim is: %i\n", resX);
	printf("Volume size in y-dim is: %i\n", resY);
	printf("Volume size in z-dim is: %i\n", num_slices);

	const void *bytes;
	int length;
	void *load_buffer = 0;
	int load_buffer_length = 0;
	int count = 0;

	FILE *f = fopen("volume.bin", "rb");
	for (int i = 0; i < num_slices; i++) 
	{
		count ++;
		fread(&length, sizeof(int), 1, f);
		cout << length << " " << count<<endl;
		if (load_buffer_length != length) 
		{
			if (load_buffer != 0) 
			{
				free(load_buffer);
			}
			load_buffer = malloc(length);
			load_buffer_length = length;
		}
		fread(load_buffer, 1, length, f);
		reme_volume_slice_set_bytes(c, v, i, load_buffer, length);
	}

	// Create a new surface
	reme_surface_t m;
	reme_surface_create(c, &m);
	reme_surface_generate(c, m, v);

	// Visualize resulting surface
	reme_viewer_t viewer_surface;
	reme_viewer_create_surface(c, m, "This is ReconstructMeSDK", &viewer_surface);
	reme_viewer_wait(c, viewer_surface);
	reme_surface_destroy(c, &m);


	// Make sure to release all memory acquired
	reme_context_destroy(&c);



	return 0;
}