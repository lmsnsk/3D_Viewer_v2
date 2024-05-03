#include "../s21_3D_Viewer.h"

#define MAX_STRING_BUFFER 4096
#define MEM_ALLOC_STP 4096

char* skip_spaces(char* str) {
  char* c = str;
  while (*c != ' ') {
    if (*c == '\n' || *c == 0) break;
    c++;
  }
  while (*c == ' ') c++;
  return c;
}

void get_vertices(Data* data, char* buffer, int* coun_v) {
  char* str = buffer;
  Vertex* new_vertex_list = data->vertices_arr;
  if (*coun_v % MEM_ALLOC_STP == 0) {
    int v_size = sizeof(Vertex);
    new_vertex_list =
        realloc(data->vertices_arr, *coun_v * v_size + MEM_ALLOC_STP * v_size);
  }
  if (new_vertex_list != NULL) {
    str = skip_spaces(str);
    data->vertices_arr = new_vertex_list;
    int scan = sscanf(str, " %lf %lf %lf", &(data->vertices_arr)[*coun_v].x,
                      &(data->vertices_arr)[*coun_v].y,
                      &(data->vertices_arr)[*coun_v].z);
    if (scan >= 3) (*coun_v)++;
  }
}

void reading_face_string(Data* data, char* str, int* coun_f, int size) {
  int scan_vert = 0;
  for (int i = 0; *str != '\n' && *str != '\r' && *str != 0; i++) {
    if ((i && (i + 3) % 16 == 0) || i == 3) {
      unsigned* new_faces = (unsigned*)realloc(data->faces_arr[*coun_f].faces,
                                               i * size + 16 * size);
      if (new_faces != NULL) data->faces_arr[*coun_f].faces = new_faces;
    }
    scan_vert += sscanf(str, "%u", &(data->faces_arr)[*coun_f].faces[i]);
    str = skip_spaces(str);
  }
  data->faces_arr[*coun_f].vert_in_face = scan_vert;
  if (scan_vert >= 3) (*coun_f)++;
}

int get_faces(Data* data, char* buffer, int* coun_f, int* error) {
  Face* new_face_list = data->faces_arr;
  char* str = buffer;
  if (*coun_f % MEM_ALLOC_STP == 0) {
    int f_size = sizeof(Face);
    new_face_list =
        realloc(data->faces_arr, *coun_f * f_size + MEM_ALLOC_STP * f_size);
  }
  if (new_face_list != NULL) {
    str = skip_spaces(str);
    data->faces_arr = new_face_list;
    int size = sizeof(unsigned);
    data->faces_arr[*coun_f].faces = (unsigned*)calloc(3, size);
    if (data->faces_arr[*coun_f].faces != NULL) {
      reading_face_string(data, str, coun_f, size);
    } else {
      *error = 1;
    }
  }
  return *error;
}

int get_values(Data* data, char* buffer, int* coun_v, int* coun_f) {
  int error = 0;
  if (buffer[0] == 'v' && buffer[1] == ' ') {
    get_vertices(data, buffer, coun_v);
  } else if (buffer[0] == 'f' && buffer[1] == ' ') {
    error = get_faces(data, buffer, coun_f, &error);
  }

  return error;
}

int check_correct_faces(Data* data) {
  int error = 0;
  for (unsigned i = 0; i < data->faces_count; i++) {
    for (unsigned j = 0; j < data->faces_arr[i].vert_in_face; j++) {
      if (data->faces_arr[i].faces[j] > data->vertices_count) error = 1;
    }
  }
  return error;
}

int parcer(const char* filename, Data* data) {
  FILE* file;
  data->faces_count = 0;
  data->vertices_count = 0;
  data->faces_arr = NULL;
  data->vertices_arr = NULL;
  int error_reading_file = 0;

  if ((file = fopen(filename, "r")) == NULL) {
    error_reading_file = 1;
  } else {
    int coun_v = 0, coun_f = 0;
    data->vertices_arr = calloc(1, sizeof(Vertex));
    data->faces_arr = calloc(1, sizeof(Face));
    if (data->vertices_arr != NULL && data->faces_arr != NULL) {
      char buffer[MAX_STRING_BUFFER];
      while ((fgets(buffer, MAX_STRING_BUFFER, file)) != NULL) {
        error_reading_file = get_values(data, buffer, &coun_v, &coun_f);
        if (data->vertices_arr == NULL || data->faces_arr == NULL) {
          error_reading_file = 1;
          break;
        }
      }
    } else {
      error_reading_file = 1;
    }
    if (!error_reading_file) {
      data->faces_count = coun_f;
      data->vertices_count = coun_v;
      error_reading_file = check_correct_faces(data);
      if (!coun_f || coun_v < 3) error_reading_file = 1;
      fclose(file);
    }
  }
  return error_reading_file;
}

void free_data(Data* data) {
  for (unsigned i = 0; i < data->faces_count; i++)
    free(data->faces_arr[i].faces);
  free(data->vertices_arr);
  free(data->faces_arr);
  data->faces_count = 0;
  data->vertices_count = 0;
  data->faces_arr = NULL;
  data->vertices_arr = NULL;
}
