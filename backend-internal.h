#ifndef _BACKEND_INTERNAL_H
#define _BACKEND_INTERNAL_H

#include <libpq-fe.h>
#include <stdbool.h>

#include "backend.h"

struct mouthpiece {
  PGconn *connection;
  settings settings;
  bool is_test;
};

mouthpiece *conch_test_connect(settings settings);

void conch_let_silence_fall(mouthpiece *mp);

#endif /* _BACKEND_INTERNAL_H */
