#include <stdio.h>
#include <stdlib.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <mpg123.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <MP3 file>\n", argv[0]);
    return 1;
  }

  mpg123_handle *mh;
  unsigned char *buffer;
  size_t buffer_size;
  size_t done;
  int err;

  int channels, encoding;
  long rate;

  // Initialize mpg123 library
  mpg123_init();
  mh = mpg123_new(NULL, &err);
  buffer_size = BUF_SIZE;
  buffer = (unsigned char *)malloc(buffer_size * sizeof(unsigned char));

  // Open the MP3 file
  mpg123_open(mh, argv[1]);
  mpg123_getformat(mh, &rate, &channels, &encoding);

  // Set up PulseAudio
  pa_sample_spec ss;
  ss.format = PA_SAMPLE_S16LE;
  ss.channels = channels;
  ss.rate = rate;
  pa_simple *s = NULL;
  int pa_error;

  // Create a new playback stream
  if (!(s = pa_simple_new(NULL, "MP3Player", PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &pa_error)))
  {
    fprintf(stderr, __FILE__ ": pa_simple_new() failed: %s\n", pa_strerror(pa_error));
    goto finish;
  }

  // Decode and play
  while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
  {
    if (pa_simple_write(s, buffer, done, &pa_error) < 0)
    {
      fprintf(stderr, __FILE__ ": pa_simple_write() failed: %s\n", pa_strerror(pa_error));
      goto finish;
    }
  }

  // Make sure all buffered data is played
  pa_simple_drain(s, &pa_error);

finish:
  if (s)
    pa_simple_free(s);

  free(buffer);
  mpg123_close(mh);
  mpg123_delete(mh);
  mpg123_exit();

  return 0;
}
