#include "util/posix_logger.h"

using namespace leveldb;

int main() {
  Env *env_ = Env::Default();
  Logger *logger = nullptr;
  env_->NewLogger("./test.log", &logger);
  int n = 100;
  Log(logger, "even test [%d]", n);

  return 0;
}