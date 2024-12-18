#pragma once

#include <stdio.h>
#include <stdbool.h>

#define err(fmt, ...) do { fprintf(stderr, "[%s:%s:%d] " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); } while (0)
#define err_exit(fmt, ...) do { fprintf(stderr, "[%s:%s:%d] " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__); exit(EXIT_FAILURE); } while (0)
