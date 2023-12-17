# MP3 Player

This is a simple MP3 player program written in C.

## Prerequisites

Before running the program, make sure you have the following installed:

- C compiler (e.g., GCC)
- MP3 library (e.g., libmpg123)

## Getting Started

1. Clone the repository:

  ```bash
  git clone https://github.com/your-username/mp3player.git
  ```

2. Compile the program:

  ```bash
  gcc mp3player.c -o mp3player -lmpg123
  ```

3. Run the program:

  ```bash
  ./mp3player
  ```

## Usage

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


