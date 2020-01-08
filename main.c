#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void listdir(const char *name, const char *filepaths[], int *i)
{
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(name)))
    return;

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR) {
      char path[1024];
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;
      snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
      listdir(path, filepaths, i);
    } else {
      char *path = malloc(strlen(name) + strlen(entry->d_name) + 1);
      path = strcpy(path, name);
      path = strcat(path, "/");
      path = strcat(path, entry->d_name);
      filepaths[*i] = path;
      *i = *i + 1;
    }
  }

  closedir(dir);
}

char *read_file(const char *path)
{
  char *content = 0;
  long length;
  FILE *fp = fopen(path, "r");

  fseek(fp, 0, SEEK_END);
  length = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  content = malloc(length);
  fread(content, 1, length, fp);
  fclose(fp);
  fp = NULL;

  return content;
}

struct File {
  const char *name;
  const char *title;
  const char *content;
  const char *date;
};

int main(void) {
  /* char *layout = read_file("./layout.html"); */
  /* printf("%s", layout); */
  const char *filepaths[1024];
  int length = 0;
  listdir("./e", filepaths, &length);

  struct File *pages[length];
  for (int i = 0; i < length; i++) {
    struct File f = {read_file(filepaths[i]), "dd", "dfsd", "dsfsdf"};
    pages[i] = &f;
    printf("%s", pages[i]->name);
  }

  // 4. Read ./e/**.html files into array of struct {name: "", date: "", title: "", content: ""}
  // 5. Wrap ./e/**.html files contents with layout
  // 6. Compose index.html file
  // 7. Create directory ./build
  // 8. Write all files into ./build directory

  return 0;
}
