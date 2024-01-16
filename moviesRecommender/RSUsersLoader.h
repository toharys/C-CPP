#ifndef SCHOOL_SOLUTION_USERFACTORY_H
#define SCHOOL_SOLUTION_USERFACTORY_H
#include "RSUser.h"
#include <fstream>
#include <sstream>
#define NA "NA"
#define CUT_KET '-'
#define MIN 1
#define MAX 10
#define FILE_MSG "invalid file path"
#define USAGE_MSG "feature must be an integer between 1 to 10"

class RSUsersLoader
{
private:


public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users
     * and their movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file
    (const std::string& users_file_path,
     std::unique_ptr<RecommenderSystem> rs)noexcept(false);
};


#endif //SCHOOL_SOLUTION_USERFACTORY_H
