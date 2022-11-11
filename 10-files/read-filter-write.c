#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>


int main(int argc, char* argv[]) {
  int8_t code = 0;

  int8_t input_file = open(argv[1], O_RDONLY);
  int8_t output_file1 = open(argv[2], O_WRONLY | O_CREAT, 0640);
  int8_t output_file2 = open(argv[3], O_WRONLY | O_CREAT, 0640);

  if (input_file == -1) {
    code = 1;
    goto exit;
  }

  if (output_file1 == -1 || output_file2 == -1) {
    code = 2;
    goto exit;
  }

  char input_byte;
  int8_t input_file_read = read(input_file, &input_byte, sizeof(char));
  int8_t output_file;
  while (input_file_read > 0) {
    if (input_byte <= '9' && input_byte >= '0') {
      output_file = output_file1;
    } else {
      output_file = output_file2;
    }

    int8_t output = write(output_file, &input_byte, sizeof(char));

    if (output == -1) {
      code = 3;
      goto exit;
    }
    input_file_read = read(input_file, &input_byte, sizeof(char));
  }

  if (input_file_read == -1) {
    code = 3;
    goto exit;
  }

  exit:
    close(input_file);
    close(output_file1);
    close(output_file2);
    return code;
}