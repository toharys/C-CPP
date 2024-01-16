#include "RSUsersLoader.h"
#include "RecommenderSystem.h"

void build_movies_vector (const std::string& line,
                          std::vector<sp_movie>& movies)
{
    std::istringstream iss(line);
    std::string word;
    // reads the movies info and insert them by order to vector
    while(iss>>word)
    {
        size_t cut_indx = word.find(CUT_KET);
        std::string movie_name = word.substr(0,cut_indx);
        int exit_year = stoi(word.substr(cut_indx+1));
        movies.push_back(std::make_shared<Movie>(movie_name,exit_year));
    }
}


 std::vector<RSUser> RSUsersLoader::create_users_from_file
        (const std::string& users_file_path,
         std::unique_ptr<RecommenderSystem> rs)noexcept(false)
 {
     std::ifstream users_file (users_file_path);
     if (!users_file.is_open() || !users_file.good())
     {
         throw std::runtime_error(FILE_MSG) ;
     }
     // initialize the res pntr
     std::vector<RSUser> res;
     std::string line;
     //read the first line (the movies) and build sp_novie vector
     std::getline(users_file, line);
     std::vector<sp_movie> movies;
     build_movies_vector(line, movies);
    //read the users info
     sp_rs rec_sys = std::move(rs);
     while(std::getline(users_file, line))
     {
         // gets the user's ranks and build the rank map
         std::istringstream iss(line);
         std::string word;
         iss>>word;
         std::string user_name = word;
         rank_map ranks(0, sp_movie_hash, sp_movie_equal);
         int indx = 0;
         while (iss>>word)
         {
             if (word==NA) //if the movie unwatched (NA) continue
             {
                 indx++;
                 continue;
             }
             double score = stod(word);
             if ((int)score!=score || // validity check
                 score>MAX || score<MIN)
             {
                 users_file.close();
                 throw std::runtime_error(USAGE_MSG);
             }
             ranks.insert({movies[indx],score});
             indx++;
         }
         RSUser new_user = RSUser(user_name,ranks, rec_sys);
         res.push_back(new_user);
     }
     users_file.close();
     return res;
 }


