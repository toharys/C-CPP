#include "RecommenderSystemLoader.h"

std::unique_ptr<RecommenderSystem>
        RecommenderSystemLoader::create_rs_from_movies_file
        (const std::string &movies_file_path) noexcept (false)
{
    std::ifstream movie_file (movies_file_path);
    if (!movie_file.is_open() || !movie_file.good())
    {
        throw std::runtime_error(FILE_MSG) ;
    }
    // initialize the res pntr
    std::unique_ptr<RecommenderSystem> res =
            std::make_unique<RecommenderSystem>();

    //reading from file
    std::string line;
    while(std::getline(movie_file, line))
    {
        std::istringstream iss(line);
    // gets the word and year
        std::string word;
        iss>>word;
        size_t cut_indx = word.find(CUT_KET);
        std::string movie_name = word.substr(0,cut_indx);
        int exit_year = stoi(word.substr(cut_indx+1));
    // gets the features as strings and build the  features vector
        std::vector<double> features;
        while (iss>>word)
        {
            double curr_feature = stod(word);
            if ((int)curr_feature!=curr_feature ||
                    curr_feature>MAX || curr_feature<MIN)
            {
                movie_file.close();
                throw std::runtime_error(USAGE_MSG);
            }
            features.push_back(curr_feature);
        }
        res->add_movie(movie_name,exit_year, features);
    }
    movie_file.close();
    return res;
}

