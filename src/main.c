#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "markdown.h"
#include "html.h"
#include "buffer.h"

#define READ_UNIT 1024
#define OUTPUT_UNIT 64

char *replace_str(char *str, char *orig, char *rep)
{
  static char buffer[4096];
  char *p;

  if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}

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

int
main()
{
  // Save layout
  char *layout = read_file("./layout.html");

  // Get markdown files
  const char *filepaths[4096];
  int length = 0;
  listdir("./e", filepaths, &length);

  // Convert markdown files to html
  for (int i = 0; i < length; i++) {
    int ret;
    const char *file;
    FILE *in;
    struct buf *ib, *ob;
    struct sd_callbacks callbacks;
    struct html_renderopt options;
    struct sd_markdown *markdown;

    in = fopen(filepaths[i], "r");
    ib = bufnew(READ_UNIT);
    bufgrow(ib, READ_UNIT);
    while ((ret = fread(ib->data + ib->size, 1, ib->asize - ib->size, in)) > 0) {
      ib->size += ret;
      bufgrow(ib, ib->size + READ_UNIT);
    }

    if (in != stdin)
      fclose(in);

    /* performing markdown parsing */
    ob = bufnew(OUTPUT_UNIT);

    sdhtml_renderer(&callbacks, &options, 0);
    markdown = sd_markdown_new(0, 16, &callbacks, &options);

    sd_markdown_render(ob, ib->data, ib->size, markdown);
    sd_markdown_free(markdown);

    // Wrap with layout
    file = replace_str(layout, "{content}", (char *)ob->data);
    printf("%s", file);

    // Clean up
    bufrelease(ib);
    bufrelease(ob);
  }

  // 6. Compose index.html file
  // 7. Create directory ./build
  // 8. Write all files into ./build directory

  return 0;
}
