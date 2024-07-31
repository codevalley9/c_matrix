#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int err_code = OK;
  if (rows < 1 || columns < 1) {
    err_code = INCORRECT_MATRIX;
  } else {
    result->matrix = (double **)calloc(rows, sizeof(double));
    result->rows = rows;
    result->columns = columns;
    if (result->matrix != NULL) {
      for (int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
          s21_remove_matrix(result);
          err_code = INCORRECT_MATRIX;
        }
      }
    } else {
      s21_remove_matrix(result);
      err_code = INCORRECT_MATRIX;
    }
  }
  return err_code;
}

void s21_remove_matrix(matrix_t *A) {
  if (A) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (!s21_is_matrix_ok(A) || !s21_is_matrix_ok(B) || A->rows != B->rows ||
      A->columns != B->columns)
    return FAILURE;
  int err_code = SUCCESS;
  for (int i = 0; i < A->rows && err_code; i++) {
    for (int j = 0; j < A->columns && err_code; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) err_code = FAILURE;
    }
  }
  return err_code;
}

int s21_is_matrix_ok(matrix_t *matrix) {
  int err_code = 1;
  if ((matrix == NULL) || (matrix->matrix == NULL) || (matrix->columns < 1) ||
      (matrix->rows < 1)) {
    err_code = 0;
  }
  return err_code;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err_code = OK;
  if (s21_is_matrix_ok(A) && s21_is_matrix_ok(B)) {
    if (A->rows == B->rows && A->columns == B->columns) {
      s21_create_matrix(A->rows, A->columns, result);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
      }
    } else {
      err_code = CALCULATION_ERROR;
    }
  } else {
    err_code = INCORRECT_MATRIX;
  }
  return err_code;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err_code = OK;
  if (s21_is_matrix_ok(A) && s21_is_matrix_ok(B)) {
    if (A->rows == B->rows && A->columns == B->columns) {
      s21_create_matrix(A->rows, A->columns, result);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
      }
    } else {
      err_code = CALCULATION_ERROR;
    }
  } else {
    err_code = INCORRECT_MATRIX;
  }
  return err_code;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int err_code = OK;
  if (s21_is_matrix_ok(A)) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  } else {
    err_code = INCORRECT_MATRIX;
  }
  return err_code;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int err_code = OK;
  if (s21_is_matrix_ok(A) && s21_is_matrix_ok(B)) {
    if (A->columns == B->rows) {
      s21_create_matrix(A->rows, B->columns, result);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
          for (int k = 0; k < B->rows; k++) {
            result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
          }
        }
      }
    } else {
      err_code = CALCULATION_ERROR;
    }
  } else {
    err_code = INCORRECT_MATRIX;
  }
  return err_code;
}

static void s21_fill_transpose(matrix_t *A, matrix_t *result) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int err_code = OK;
  if (s21_is_matrix_ok(A)) {
    s21_create_matrix(A->columns, A->rows, result);
    s21_fill_transpose(A, result);
  } else {
    err_code = INCORRECT_MATRIX;
  }
  return err_code;
}

int s21_determinant(matrix_t *A, double *result) {
  int err_code = OK;
  if (A->rows == A->columns) {
    if (s21_is_matrix_ok(A)) {
      *result = s21_recursion_det(A);
    } else {
      err_code = INCORRECT_MATRIX;
    }
  } else {
    err_code = CALCULATION_ERROR;
  }
  return err_code;
}

double s21_recursion_det(matrix_t *A) {
  double result = 0;
  if (A->columns == 1) {
    result = A->matrix[0][0];
  } else if (A->columns == 2) {
    result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
  } else {
    if (A->rows != 1 && A->rows != 2) {
      result = 0;
      matrix_t temp_m = {0};
      s21_create_matrix(A->rows - 1, A->columns - 1, &temp_m);
      for (int i = 0; i < A->rows; i++) {
        s21_fill_matrix(0, i, A, &temp_m);
        result += pow(-1, i) * A->matrix[0][i] * s21_recursion_det(&temp_m);
      }
      s21_remove_matrix(&temp_m);
    }
  }
  return result;
}

void s21_fill_matrix(int rws, int clmns, matrix_t *A, matrix_t *result) {
  int r = 0;
  int c = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i == rws) {
      continue;
    }
    c = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == clmns) {
        continue;
      }
      result->matrix[r][c] = A->matrix[i][j];
      c++;
    }
    r++;
  }
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int err_code = OK;
  if (A->rows == A->columns) {
    if (A->rows == 1) {
      err_code = CALCULATION_ERROR;
    }
    if (s21_is_matrix_ok(A) && A->rows >= 2) {
      s21_create_matrix(A->rows, A->columns, result);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          matrix_t minor = {0};
          double det = 0;
          s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
          s21_fill_matrix(i, j, A, &minor);
          s21_determinant(&minor, &det);
          result->matrix[i][j] = pow(-1, (i + j)) * det;
          s21_remove_matrix(&minor);
        }
      }
    } else {
      err_code = INCORRECT_MATRIX;
    }
  } else {
    err_code = CALCULATION_ERROR;
  }
  return err_code;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (!s21_is_matrix_ok(A)) return INCORRECT_MATRIX;
  double det = 0;
  int err_code = s21_determinant(A, &det);
  if (!err_code && det != 0) {
    if (A->rows == 1) {
      err_code = s21_create_matrix(A->rows, A->columns, result);
      if (!err_code) result->matrix[0][0] = 1 / A->matrix[0][0];
    } else {
      matrix_t trancepose_m = {NULL, 0, 0};
      err_code = s21_transpose(A, &trancepose_m);
      if (!err_code) {
        matrix_t calc_m = {NULL, 0, 0};
        err_code = s21_calc_complements(&trancepose_m, &calc_m);
        if (!err_code) {
          err_code = s21_mult_number(&calc_m, 1 / det, result);
        }
        s21_remove_matrix(&calc_m);
      }
      s21_remove_matrix(&trancepose_m);
    }
  } else {
    s21_remove_matrix(result);
    err_code = CALCULATION_ERROR;
  }
  return err_code;
}
