# MP3 Player

This is a simple MP3 player program written in C.

## References
https://www.mpg123.de

## Prerequisites

Before running the program, make sure you have the following installed:

- C compiler (e.g., GCC)
- MP3 library (e.g., libmpg123)

## Getting Started

1. Clone the repository:

  ```bash
  git clone https://github.com/cd-stephen/pulse-audio-mp3.git
  ```

2. Compile the program:

  ```bash
  gcc mp3player.c -o mp3player -lmpg123
  ```

3. Run the program:

  ```bash
  ./mp3player
  ```

## Usage - NOT IMPLEMENTED YET

- Use the arrow keys to navigate through the playlist.
- Press the Enter key to play the selected song.
- Press the Space key to pause/resume playback.
- Press the Esc key to exit the program.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

## Code Explanation
 1. Include Directives: header files needed for the program
```c
#include <stdio.h>       // Includes the Standard Input Output header for basic I/O functions.
#include <stdlib.h>      // Includes the Standard Library header for memory allocation, process control, etc.
#include <pulse/simple.h>   // Includes the PulseAudio simple API header for audio playback.
#include <pulse/error.h>    // Includes the PulseAudio error handling functions.
#include <mpg123.h>      // Includes the mpg123 library for MP3 decoding.
```

2. Define Directives: Sets a constant 'BUF_SIZE' to 1024 - this will be used as the size for the buffer that holds the audio data.
```c
#define BUF_SIZE 1024
```

3. Entrypoint main and arg check
```c
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <MP3 file>\n", argv[0]);
    return 1;
}
```

4. Variable declarations
```c
mpg123_handle *mh;        // Handle pointer for the mpg123 decoder.
unsigned char *buffer;    // Buffer pointer to hold decoded audio data.
size_t buffer_size;       // Size of the buffer.
size_t done;              // Variable to hold the number of bytes decoded.
int err;                  // Variable to store error codes.
int channels, encoding;   // Variables to store audio channels and encoding format.
long rate;                // Variable to store the sample rate of the audio.
```

5. Init mpg123
```c
mpg123_init();
mh = mpg123_new(NULL, &err);
buffer_size = BUF_SIZE;
buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
```

6. Open file and get rate, channels and encoding. (mp3)
```c
mpg123_open(mh, argv[1]);
mpg123_getformat(mh, &rate, &channels, &encoding);
```

7. Configure Pulse-Audio for playback
```c
pa_sample_spec ss;
ss.format = PA_SAMPLE_S16LE;  // Set the audio format.
ss.channels = channels;       // Set the number of audio channels.
ss.rate = rate;               // Set the audio sample rate.
pa_simple *s = NULL;
int pa_error;
```

8. Create the playback stream
```c
if (!(s = pa_simple_new(NULL, "MP3Player", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &pa_error))) {
    fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(pa_error));
    goto finish;
}
```
9. Reads the MP3 file, decode, and write the decoded data to the audio playback stream created in step 8.
```c
while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
    if (pa_simple_write(s, buffer, done, &pa_error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(pa_error));
        goto finish;
    }
}
```
10. Drain the audio buffer. ensure everything in the buffer is processed (played) before the program terminates
```c
pa_simple_drain(s, &pa_error);
```

11. Cleanup Label and Code
```c
finish:
if (s)
    pa_simple_free(s);
```

12. Free memory and close mpg123
```c
free(buffer);
mpg123_close(mh);
mpg123_delete(mh);
mpg123_exit();
```

13. return 0
```c
return 0;
```


