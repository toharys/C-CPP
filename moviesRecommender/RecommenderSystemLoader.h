
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H
#include <fstream>
#include <sstream>
#define FILE_MSG "invalid file path"
#define USAGE_MSG "feature must be an integer between 1 to 10"
#define MIN 1
#define MAX 10
#define CUT_KET '-'
#include "RecommenderSystem.h"

class RecommenderSystemLoader {

 private:

 public:
  RecommenderSystemLoader () = delete;
  /**
   * loads movies by the given format for movies with their feature's score
   * @param movies_file_path a path to the file of the movies
   * @return smart pointer to a RecommenderSystem which was created with
   * those movies
   */
  static std::unique_ptr<RecommenderSystem> create_rs_from_movies_file
	  (const std::string &movies_file_path) noexcept (false);
};

#endif //RECOMMENDERSYSTEMLOADER_H
