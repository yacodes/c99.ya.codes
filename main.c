#include <stdio.h>
#include <stdint.h>
#include <dirent.h>

void read_file(const char* name, uint8_t* contents)
{
  FILE *fp = NULL;

  fp = fopen(name, "r");

  fseek(fp, 0, SEEK_END);

  uintmax_t fileLength = ftell(fp);

  fseek(fp, 0, SEEK_SET);

  fread(contents, 1, fileLength, fp);

  fclose(fp);

  fp = NULL;
}

int main(void) {
  // 1. Read layout file
  uint8_t layout[8192];
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
  int files = 0;

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

  return 0;
}
