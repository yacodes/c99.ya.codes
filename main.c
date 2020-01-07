#include <stdio.h>
#include <stdint.h>
#include <dirent.h>

void read_file(const char* path, uint8_t* contents)
{
  FILE *fp = NULL;
  fp = fopen(path, "r");
  fseek(fp, 0, SEEK_END);
  uintmax_t file_length = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  fread(contents, 1, file_length, fp);
  fclose(fp);
  fp = NULL;
}

int main(void) {
  uint8_t layout[1024];
  read_file("./layout.html", layout);

  // 2. Read ./e directory contents
  // 3. Read ./e subdirectories contents
  // 4. Read ./e/**.html files into array of struct {name: "", date: "", title: "", content: ""}
  // 5. Wrap ./e/**.html files contents with layout
  // 6. Compose index.html file
  // 7. Create directory ./build
  // 8. Write all files into ./build directory

  DIR *folder;
  struct dirent *entry;
  int8_t files = 0;

  folder = opendir("./e");
  if(folder == NULL)
  {
    perror("Unable to read directory");
    return(1);
  }

  while((entry=readdir(folder)))
  {
    files++;
    printf("File %3d: %s\n", files, entry->d_name);
  }

  closedir(folder);

  printf("%s", layout);

  printf("\n\n &files = %p", (void *) &files);
  printf("\n%lu", sizeof(layout));

  return 0;
}
