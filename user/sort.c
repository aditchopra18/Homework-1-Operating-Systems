#include "user.h"
#define UNSET 0
#define INIT_BUF_SIZE 10

typedef struct {
  char* buf;
  int char_count;
  int capacity;
} Buffer;

// To implement in Step 2
Buffer alloc_buffer(int capacity);
void free_buffer(Buffer* buffer);
void resize_buffer(Buffer* buffer, int capacity);
void read_into_buffer(int fd, Buffer* buffer);
int my_atoi(const char *str);
void bubble_sort(int* arr, int n);

// To implement in Step 3
void sort(Buffer* buffer, int reversed);

// Main function
int main(int argc, char* argv[]) {
  int reversed = 0, fd = 0;
  
  // Argument parsing with proper handling of '-r' flag and filenames
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-r") == 0) {
      reversed = 1;
    } else {
      // Only set fd if it's not already set, to avoid overriding the file descriptor
      if (fd == 0) {
        fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
          printf("Error: cannot open %s\n", argv[i]);
          exit();
        }
      }
    }
  }

  Buffer buffer = alloc_buffer(INIT_BUF_SIZE);
  read_into_buffer(fd, &buffer);
  sort(&buffer, reversed);

  // Close the file descriptor after it's used, not before
  if (fd != 0) {
    close(fd);
  }
  
  free_buffer(&buffer);
  exit();
}

Buffer alloc_buffer(int capacity) {
  // TODO: Step 2.1: Initialize Buffer with the correct values,
  // your solution should have NOTHING unset.
  Buffer buffer;
  buffer.buf = malloc(capacity);
  buffer.char_count = 0;
  buffer.capacity = capacity;
  return buffer;
}

void free_buffer(Buffer* buffer) {
  // TODO: Step 2.2: Implement basic Buffer helpers
  // This should free up the malloc'd memory used by the buffer.
  free(buffer->buf);
}

void resize_buffer(Buffer* buffer, int new_capacity) {
  // TODO: Step 2.3: Implement basic Buffer helpers
  // This should resize the allocated memory for the buffer to capacity.
 char* temp = realloc(buffer->buf, new_capacity);
  // Check for reallocation failure
  if (temp == 0) {
    printf("Error: realloc failed\n");
    free(buffer->buf); // Freeing up the memory
    exit();
  }
  buffer->buf = temp;
  buffer->capacity = new_capacity;
}

void read_into_buffer(int fd, Buffer* buffer) {
  // TODO: Step 2.4: Implement basic Buffer helpers
  // This should read everything from fd into the buffer until it reaches EOF.
  int n;
  while ((n = read(fd, buffer->buf + buffer->char_count, buffer->capacity - buffer->char_count)) > 0) {
    buffer->char_count += n;
    
    if (buffer->char_count == buffer->capacity) {
      resize_buffer(buffer, buffer->capacity * 2);
    }
  }

  // For debugging, you can uncomment this to check the content of the buffer.
/*
  printf("buf: %p, capacity: %d, char_count: %d\ncontent:\n", buffer->buf, buffer->capacity, buffer->char_count);
  write(1, buffer->buf, buffer->char_count);
  printf("END OF CONTENT\n");
*/
}

void sort(Buffer* buffer, int reversed) {
    if (buffer->char_count == 0) {
        // If there's no content in the buffer, just return without doing anything
        printf("\n");
        return;
    }

    int capacity = 1024; // Initial capacity for the numbers array
    int* numbers = malloc(capacity * sizeof(int));
    if (numbers == 0) {
        printf("Failed to allocate memory for numbers\n");
        exit();
    }
    int num_count = 0;

    char* token = buffer->buf;
    for (int i = 0; i <= buffer->char_count; i++) {
        if (buffer->buf[i] == ' ' || i == buffer->char_count) {
            buffer->buf[i] = 0; // Null-terminate the token
            if (token != buffer->buf + i) { // Check if token is not just an empty string
                if (num_count >= capacity) {
                    capacity *= 2;
                    numbers = realloc(numbers, capacity * sizeof(int));
                    if (numbers == 0) {
                        printf("Failed to realloc memory for numbers\n");
                        exit();
                    }
                }
                numbers[num_count++] = my_atoi(token);
            }
            token = buffer->buf + i + 1;
        }
    }

    // Only proceed if we have numbers to sort
    if (num_count > 0) {
        bubble_sort(numbers, num_count); // Sort the numbers

        // Print sorted numbers, handling the reversed flag
        if (reversed) {
            for (int i = num_count - 1; i >= 0; --i) {
                printf("%d ", numbers[i]);
            }
        } else {
            for (int i = 0; i < num_count; ++i) {
                printf("%d ", numbers[i]);
            }
        }
        printf("\n");
    }

    free(numbers); // Free the dynamically allocated numbers array
}

// 
int my_atoi(const char* str) {
    int res = 0;
    int sign = 1;
    
    // Check if the first character is indicating a negative number, if so set the sign to negative
    if (*str == '-') {
        sign = -1;
        str++;
    }
    
    // Iterate through each character of the string until we reach the end ('\0')
    while (*str) {
        // Convert character digit to integer and add it to the result
        res = res * 10 + (*str - '0');
        str++;
    }
    return sign * res;
}

// The sorting function
void bubble_sort(int* arr, int n) {
    if (n <= 1) {
        return;
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}